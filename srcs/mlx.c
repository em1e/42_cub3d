/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 04:40:55 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/29 12:10:56 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	convert_textures(t_cub3d *kissa)
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

/*
Initializes MLX and stores the required images.
*/
void	init_mlx(t_cub3d *kissa)
{
	kissa->map->tile_size = 21;
	mlx_set_setting(MLX_STRETCH_IMAGE, 1);
	kissa->mlx = mlx_init(MLX_WIDTH, MLX_HEIGHT, "KISSA^3", true);
	if (!kissa->mlx)
		quit_perror(kissa, NULL, "mlx_init failed");
	convert_textures(kissa);
	kissa->view->mlx_scene = mlx_new_image(kissa->mlx, kissa->mlx->width, kissa->mlx->height);
	if (!kissa->view->mlx_scene)
		quit_perror(kissa, NULL, "MLX42 failed");
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_scene, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_scene->instances, Z_SCENE);
}
