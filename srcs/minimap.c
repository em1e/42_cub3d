/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:33:57 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/18 10:12:56 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Returns 1 if coordinate ray_x and ray_y are within the minimap radius
	from player pos (6, 6), else returns 0.
*/
static int	check_radius(t_cub3d *kissa, float ray_x, float ray_y)
{
	int center_x;
	int center_y;

	center_x = 6 * kissa->map->tile_size + kissa->map->tile_size / 2;
	center_y = 6 * kissa->map->tile_size + kissa->map->tile_size / 2;
	if (sqrt(pow((ray_x - center_x), 2) + pow((ray_y - center_y), 2)) <= MMRAD * kissa->map->tile_size)
		return (1);
	return (0);
}

/*
	Shoots a ray from the player object in the direction of the player object.
	While the ray is not hitting a wall, the ray is drawn on the screen.

	0xFF0000FF = red
	0x00FF00FF = green
	0x0000FFFF = blue
	0x00FFFFFF = white
	0x000000FF = black
	0xFF00FFFF = purple
	0xFFFF00FF = yellow

*/
static void	shoot_ray(t_cub3d *kissa, t_obj *obj)
{
	int tile = kissa->map->tile_size;
	int radius = MMRAD * 2;
	int center_x = 6 * tile + tile / 2;
	int center_y = 6 * tile + tile / 2;
	float ray_x = center_x;
	float ray_y = center_y;
	float player_x = obj->x * tile;
	float player_y = obj->y * tile;

	kissa->view->ray = mlx_new_image(kissa->mlx, tile * 100, tile * 100);
	if (!kissa->view->ray)
		quit_perror(kissa, NULL, "MLX42 image creation failed");
	while (check_radius(kissa, ray_x, ray_y) && (ray_x - center_x) + radius / 2 * tile >= 0
		&& (ray_y - center_y) + radius / 2 * tile >= 0
		&& kissa->map->array[(int)floor(player_y / tile)][(int)floor(player_x / tile)] != '1')
	{
		if (ray_x < 0 || ray_x >= kissa->map->width * tile || ray_y < 0 || ray_y >= kissa->map->height * tile)
				break;
		mlx_put_pixel(kissa->view->ray, (ray_x - center_x + radius * tile / 2) + tile / 2,
			(ray_y - center_y + radius * tile / 2) + tile / 2, 0xFF0000FF);
		ray_x += obj->dir->x;
		ray_y += obj->dir->y;
		player_x += obj->dir->x;
		player_y += obj->dir->y;
	}
	if (mlx_image_to_window(kissa->mlx, kissa->view->ray, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 image to window failed");
}


/*
	Draws the image for the provided character at the given coordinate, which is 
	calculated using provided indexes and the tile size used.
*/
static void	toggle_map_tile(t_cub3d *kissa, char c, int x, int y)
{
	if (c == '1' && get_tile(kissa->view, y, x, c)->enabled == 0)
	{
		get_tile(kissa->view, y, x, '1')->enabled = 1;
		get_tile(kissa->view, y, x, '0')->enabled = 0;
	}
	else if (c == '0' && get_tile(kissa->view, y, x, c)->enabled == 0)
	{
		get_tile(kissa->view, y, x, '0')->enabled = 1;
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
	mlx_delete_image(kissa->mlx, kissa->view->ray);
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
	shoot_ray(kissa, kissa->player);
}

void	populate_minimap_instances(t_cub3d *kissa)
{
	int		i;
	t_view	*view;

	view = kissa->view;

	i = 0;
	view->wall_inst = ft_calloc(sizeof(int*), MMRAD * 2 + 1);
	view->floor_inst = ft_calloc(sizeof(int*), MMRAD * 2 + 1);
	if (!view->wall_inst || !view->floor_inst)
		quit_error(kissa, NULL, "memory allocation failure");
	while (i <= MMRAD * 2)
	{
		view->wall_inst[i] = ft_calloc(sizeof(int), MMRAD * 2 + 1);
		view->floor_inst[i] = ft_calloc(sizeof(int), MMRAD * 2 + 1);
		if (!view->wall_inst[i] || !view->floor_inst[i])
			quit_error(kissa, NULL, "memory allocation failure");
		i++;
	}
}

void	setup_minimap(t_cub3d *kissa, int i, int j)
{
	populate_minimap_instances(kissa);
	while (i <= MMRAD * 2)
	{
		j = 0;
		while (j <= MMRAD * 2)
		{
			kissa->view->wall_inst[i][j] = mlx_image_to_window(kissa->mlx,
				kissa->view->mlx_wall, j * kissa->map->tile_size, i * kissa->map->tile_size);
			if (kissa->view->wall_inst[i][j] < 0)
				quit_perror(kissa, NULL, "MLX42 failed");
			mlx_set_instance_depth(get_tile(kissa->view, j, i, '1'), Z_MINIMAP);
			kissa->view->floor_inst[i][j] = mlx_image_to_window(kissa->mlx,
				kissa->view->mlx_floor, j * kissa->map->tile_size, i * kissa->map->tile_size);
			if (kissa->view->floor_inst[i][j] < 0)
				quit_perror(kissa, NULL, "MLX42 failed");
			mlx_set_instance_depth(get_tile(kissa->view, j, i, '0'), Z_MINIMAP);
			get_tile(kissa->view, j, i, '0')->enabled = 0;
			get_tile(kissa->view, j, i, '1')->enabled = 0;
			j++;
		}
		i++;
	}
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_player, 
		MMRAD * kissa->map->tile_size, MMRAD * kissa->map->tile_size) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_player->instances, Z_MINIMAP);
}
