/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:25:48 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/29 15:01:03 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_rays(t_cub3d *kissa)
{
	int	i;

	i = 0;
	kissa->ray_array = malloc(sizeof(t_ray *) * RAYC);
	if (!kissa->ray_array)
		quit_error(kissa, NULL, "memory allocation failure");
	while (i < RAYC)
	{
		kissa->ray_array[i] = new_ray(kissa);
		kissa->ray_array[i]->index = i;
		kissa->ray_array[i]->screen_start->x = i * MLX_WIDTH / RAYC;
		kissa->ray_array[i]->screen_start->y = 0;
		kissa->ray_array[i]->rot_diff = (RAYC / 2 - i) * (M_PI * 0.5 / RAYC);
		kissa->ray_array[i]->fisheye = cos(kissa->ray_array[i]->rot_diff);
		i++;
	}
}

/*
	Initializes the cub3d struct.
*/
void	init_kissa(t_cub3d *kissa)
{
	*kissa = (t_cub3d){0};
	kissa->fd = -1;
	kissa->paused = true;
	kissa->column_width = MLX_WIDTH / RAYC;
	kissa->wall_height = WALL_HEIGHT;
	kissa->north = M_PI * 0.5;
	kissa->east = 0;
	kissa->south = M_PI * 1.5;
	kissa->west = M_PI;
	kissa->c[0] = -1;
	kissa->f[0] = -1;
	kissa->map = new_map(kissa);
	kissa->view = new_view(kissa);
	kissa->player = new_obj(kissa, PLAYER_SPEED);
	init_rays(kissa);
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
	kissa->view->mlx_scene = mlx_new_image(kissa->mlx, kissa->mlx->width,
			kissa->mlx->height);
	if (!kissa->view->mlx_scene)
		quit_perror(kissa, NULL, "MLX42 failed");
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_scene, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_scene->instances, Z_SCENE);
}
