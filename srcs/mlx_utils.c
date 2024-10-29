/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 04:40:55 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/29 13:17:25 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	convert_textures(t_cub3d *kissa)
{
	kissa->view->mlx_start = convert_png(kissa, START_SCREEN);
	kissa->view->mlx_victory = convert_png(kissa, VICTORY_SCREEN);
	kissa->view->mlx_dead = convert_png(kissa, DEAD_SCREEN);
	kissa->view->mlx_no = convert_png(kissa, kissa->no);
	kissa->view->mlx_we = convert_png(kissa, kissa->we);
	kissa->view->mlx_so = convert_png(kissa, kissa->so);
	kissa->view->mlx_ea = convert_png(kissa, kissa->ea);
	kissa->view->mlx_wall = convert_png(kissa, WALL_TEXTURE);
	kissa->view->mlx_floor = convert_png(kissa, FLOOR_TEXTURE);
	kissa->view->mlx_player = convert_png(kissa, PLAYER_TEXTURE);
	kissa->view->mlx_cat = convert_png(kissa, MINI_CAT_TEXTURE);
	kissa->view->original_cat = convert_png(kissa, SPRITE_CAT_TEXTURE);
}

void	delete_views_mlx(t_cub3d *kissa, t_view *view)
{
	if (view->mlx_victory)
		mlx_delete_image(kissa->mlx, view->mlx_victory);
	if (view->mlx_dead)
		mlx_delete_image(kissa->mlx, view->mlx_dead);
	if (view->ray)
		mlx_delete_image(kissa->mlx, view->ray);
	if (view->mlx_no)
		mlx_delete_image(kissa->mlx, view->mlx_no);
	if (view->mlx_so)
		mlx_delete_image(kissa->mlx, view->mlx_so);
	if (view->mlx_we)
		mlx_delete_image(kissa->mlx, view->mlx_we);
	if (view->mlx_ea)
		mlx_delete_image(kissa->mlx, view->mlx_ea);
	if (view->mlx_wall)
		mlx_delete_image(kissa->mlx, view->mlx_wall);
	if (view->mlx_floor)
		mlx_delete_image(kissa->mlx, view->mlx_floor);
	if (view->mlx_player)
		mlx_delete_image(kissa->mlx, view->mlx_player);
	if (view->mlx_cat)
		mlx_delete_image(kissa->mlx, view->mlx_cat);
	if (view->mlx_scene)
		mlx_delete_image(kissa->mlx, view->mlx_scene);
}

/*
	Converts a .png file into a texture and then converts that texture into an 
	MLX image.
*/
mlx_image_t	*convert_png(t_cub3d *kissa, char *file)
{
	mlx_texture_t	*texture;
	mlx_image_t		*img;

	texture = mlx_load_png(file);
	if (!texture)
		quit_perror(kissa, file, "mlx_load_png failed");
	img = mlx_texture_to_image(kissa->mlx, texture);
	if (!img)
		quit_perror(kissa, NULL, "mlx_texture_to_image failed");
	mlx_delete_texture(texture);
	return (img);
}

int	init_minimap_tile(t_cub3d *kissa, int i, int j, char c)
{
	mlx_image_t	*img;
	int			inst;

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
