/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:33:57 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/29 11:18:43 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	puts direction pixels on screen from the player object in the direction of 
	the player object.

	0xFF0000FF = red
	0x00FF00FF = green
	0x0000FFFF = blue
	0x00FFFFFF = white
	0x000000FF = black
	0xFF00FFFF = purple
	0xFFFF00FF = yellow

*/
void	draw_direction(t_cub3d *kissa)
{
	t_vec	*point;
	int		distance;

	point = new_vec(kissa);
	point->x = 5 * kissa->map->tile_size + kissa->map->tile_size / 2;
	point->y = 5 * kissa->map->tile_size + kissa->map->tile_size / 2;
	distance = 0;
	if (kissa->view->ray)
		mlx_delete_image(kissa->mlx, kissa->view->ray);
	kissa->view->ray = mlx_new_image(kissa->mlx, MLX_WIDTH, MLX_HEIGHT);
	while (distance <= kissa->map->tile_size / 2)
	{
		point->x = (5 * kissa->map->tile_size + kissa->map->tile_size / 2) 
			+ distance * cos(kissa->player->rot);
		point->y = (5 * kissa->map->tile_size + kissa->map->tile_size / 2) 
			+ distance * sin(kissa->player->rot);
		mlx_put_pixel(kissa->view->ray, point->x, point->y, 0x000000FF);
		distance++;
	}
	free(point);
	mlx_image_to_window(kissa->mlx, kissa->view->ray, 0, 0);
	mlx_set_instance_depth(kissa->view->ray->instances, Z_MINIMAP + 1);
}

/*
	Draws the image for the provided character at the given coordinate, which 
	is calculated using provided indexes and the tile size used.
*/
static void	toggle_map_tile(t_cub3d *kissa, char c, int x, int y)
{
	if (c == '1' && get_tile(kissa->view, y, x, c)->enabled == 0)
	{
		get_tile(kissa->view, y, x, '1')->enabled = 1;
		get_tile(kissa->view, y, x, '0')->enabled = 0;
		get_tile(kissa->view, y, x, 'C')->enabled = 0;
	}
	else if (c == '0' && get_tile(kissa->view, y, x, c)->enabled == 0)
	{
		get_tile(kissa->view, y, x, '0')->enabled = 1;
		get_tile(kissa->view, y, x, '1')->enabled = 0;
		get_tile(kissa->view, y, x, 'C')->enabled = 0;
	}
	else if (c == 'C' && get_tile(kissa->view, y, x, c)->enabled == 0)
	{
		get_tile(kissa->view, y, x, 'C')->enabled = 1;
		get_tile(kissa->view, y, x, '0')->enabled = 0;
		get_tile(kissa->view, y, x, '1')->enabled = 0;
	}
}

static void	refresh_map_line(t_cub3d *kissa, int line_i, int to_skip)
{
	int	column_i;
	int	x;
	int	y;

	column_i = to_skip;
	y = kissa->player->y + line_i - MMRAD;
	while (column_i <= MMRAD * 2 - to_skip)
	{
		if (line_i == MMRAD && column_i == MMRAD)
			column_i++;
		x = kissa->player->x + column_i - MMRAD;
		if (y < 0 || y >= kissa->map->height || x < 0 || x >= kissa->map->width)
			toggle_map_tile(kissa, '1', line_i, column_i);
		else
			toggle_map_tile(kissa, kissa->map->array[y][x], line_i, column_i);
		column_i++;
	}
}

/*
	Draws the image for the provided character at the given coordinate, which is 
	calculated using provided indexes and the tile size used.
*/
void	refresh_minimap(t_cub3d *kissa)
{
	int line_i;

	line_i = 0;
	while (line_i <= MMRAD * 2)
	{
		if (line_i == 0 || line_i == MMRAD * 2)
			refresh_map_line(kissa, line_i, 3);
		else if (line_i < 3 || line_i > MMRAD * 2 - 3)
			refresh_map_line(kissa, line_i, 1);
		else
			refresh_map_line(kissa, line_i, 0);
		line_i++;
	}
	draw_direction(kissa);
}

void	populate_minimap_instances(t_cub3d *kissa)
{
	int		i;
	t_view	*view;

	view = kissa->view;

	i = 0;
	view->wall_inst = ft_calloc(sizeof(int*), MMRAD * 2 + 1);
	view->floor_inst = ft_calloc(sizeof(int*), MMRAD * 2 + 1);
	view->cat_inst = ft_calloc(sizeof(int*), MMRAD * 2 + 1);
	if (!view->wall_inst || !view->floor_inst)
		quit_error(kissa, NULL, "memory allocation failure");
	while (i <= MMRAD * 2)
	{
		view->wall_inst[i] = ft_calloc(sizeof(int), MMRAD * 2 + 1);
		view->floor_inst[i] = ft_calloc(sizeof(int), MMRAD * 2 + 1);
		view->cat_inst[i] = ft_calloc(sizeof(int), MMRAD * 2 + 1);
		if (!view->wall_inst[i] || !view->floor_inst[i] || !view->cat_inst[i])
			quit_error(kissa, NULL, "memory allocation failure");
		i++;
	}
}

static int	init_minimap_tile_image(t_cub3d *kissa, int i, int j, char c)
{
	mlx_image_t	*img;
	int					inst;

	img = NULL;
	if (c == '1')
		img = kissa->view->mlx_wall;
	else if (c == '0')
		img = kissa->view->mlx_floor;
	else if (c == 'C')
		img = kissa->view->mlx_cat;
	if (!img)
		quit_error(kissa, NULL, "something wrong in code");
	inst = mlx_image_to_window(kissa->mlx,
		img, j * kissa->map->tile_size, i * kissa->map->tile_size);
	if (inst < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(&img->instances[inst], Z_MINIMAP);
	img->instances[inst].enabled = 0;
	return (inst);
}

void	setup_minimap(t_cub3d *kissa, int i, int j)
{
	populate_minimap_instances(kissa);
	while (i <= MMRAD * 2)
	{
		j = 0;
		while (j <= MMRAD * 2)
		{
			kissa->view->wall_inst[i][j] = init_minimap_tile_image(kissa, i, j, '1');
			kissa->view->floor_inst[i][j] = init_minimap_tile_image(kissa, i, j, '0');
			kissa->view->cat_inst[i][j] = init_minimap_tile_image(kissa, i, j, 'C');
			j++;
		}
		i++;
	}
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_player, 
		MMRAD * kissa->map->tile_size, MMRAD * kissa->map->tile_size) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_player->instances, Z_MINIMAP);
}
