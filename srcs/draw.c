/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:43:44 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/12 16:40:17 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	NOTE:
	The issue we had with line len not printing correctly was a memory issue.
	when I tried printing kissa->ray->line_len then it returned 0 after one use.

	so ray_array is now malloced.

	seems to be correctly calculating the line lens
*/
static void	get_ray_lens(t_cub3d *kissa)
{
	int	i;
	float	start_rot;
	float	rot;
	float	rot_change;

	rot_change = M_PI / 2 / RAYC;
	start_rot = kissa->player->rot - M_PI / 4;
	if (start_rot < 0)
		start_rot += 2 * M_PI;
	// printf("Player rot %f - start rot %f\n", kissa->player->rot, start_rot);
	i = 0;
	while (i < RAYC)
	{
		rot = start_rot + i * rot_change;
		if (rot > 2 * M_PI)
			rot -= 2 * M_PI;
		cast_ray(kissa, rot, kissa->ray_array[i]);
		
		// printf("A Ray %d rot %f and len %f\n", i, rot, kissa->ray->line_len);
		// printf("B Ray %d rot %f and len %f\n", i, rot, kissa->view->ray_array[i]);
		i++;
	}
}

int32_t	rgb_to_pixel(int *rgb)
{
	return (rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | 255);
}

void	draw_background(t_cub3d *kissa)
{
	mlx_image_t	*bg;
	uint32_t	i;
	uint32_t	j;

	bg = kissa->view->mlx_bg;
	//printf("Image height %d and width %d\n", bg->height, bg->width);
	i = 0;
	while (i < bg->height / 2)
	{
		j = 0;
		while (j < bg->width)
		{
			//printf("Setting pixels %d and %d\n", i, j);
			mlx_put_pixel(bg, j, i, rgb_to_pixel(kissa->c));
			mlx_put_pixel(bg, j, i + bg->height / 2, rgb_to_pixel(kissa->f));
			j++;
		}
		i++;
	}
	if (mlx_image_to_window(kissa->mlx, bg, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(bg->instances, Z_BACKGROUND);
}

void	draw_texture(t_cub3d *kissa, t_ray *ray, int ray_i)
{
	float	scale;
	int	column_start_x;
	int	y;
	int	x;

	column_start_x = ray_i * MLX_WIDTH / RAYC;
	scale = kissa->wall_height / ray->line_len;
	ray->wall_hit->y = scale / 2 + MLX_HEIGHT / 2;
	y = ray->wall_hit->y;
	// printf("WANT TO DRAW scale %f, column_start (x) %d, height start (y) %d\n", scale, column_start_x, y);
	while (y > ray->wall_hit->y - scale)
	{
		x = column_start_x;
		while (x < column_start_x + MLX_WIDTH / RAYC)
		{
			mlx_put_pixel(kissa->view->mlx_scene, x, y, 0xFF00FFFF);
			x++;
		}
		y--;
	}
}

void	reset_scene_image(t_cub3d *kissa)
{
	if (kissa->view->mlx_scene)
		mlx_delete_image(kissa->mlx, kissa->view->mlx_scene);
	kissa->view->mlx_scene = mlx_new_image(kissa->mlx, kissa->mlx->width, kissa->mlx->height);
	if (!kissa->view->mlx_scene)
		quit_perror(kissa, NULL, "MLX42 failed");
}

void	draw_scene(t_cub3d *kissa)
{
	int	i;

	i = 0;
	get_ray_lens(kissa);
	reset_scene_image(kissa);
	while (i < RAYC)
	{
		draw_texture(kissa, kissa->ray_array[i], i);
		i++;
	}
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_scene, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_scene->instances, Z_SCENE);
}
