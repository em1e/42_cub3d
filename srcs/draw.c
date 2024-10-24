/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:43:44 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/24 06:29:21 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	cast_rays(t_cub3d *kissa)
{
	int	i;
	float	start_rot;
	t_ray	*ray;

	i = 0;
	while (i < kissa->cat_count)
		kissa->cats[i++]->seen_by = NULL;
	start_rot = kissa->player->rot - FOV / 2;
	if (start_rot < 0)
		start_rot += 2 * M_PI;
	i = 0;
	while (i < RAYC)
	{
		ray = kissa->ray_array[i];
		ray->rot = start_rot + i * RAYDIFF;
		if (ray->rot > 2 * M_PI)
			ray->rot -= 2 * M_PI;
		cast_ray(kissa, ray);
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
	i = 0;
	while (i < bg->height / 2)
	{
		j = 0;
		while (j < bg->width)
		{
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

uint32_t	get_imgs_pixel(mlx_image_t *img, int x, int y, float scale_factor)
{
	int			pixel_index;
	uint8_t		*pixel;
	uint32_t	color;

	x = floor(x * scale_factor);
	y = floor(y * scale_factor);
	if ((uint32_t)x >= img->width)
		x = x % img->width;
	pixel_index = (y * img->width + x) * (32 / 8);
	pixel = img->pixels + pixel_index;
	color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | 255;
	return (color);
}

void	draw_column(t_cub3d *kissa, t_ray *ray)
{
	int			y;
	int			x;
	uint32_t	pixel;
	
	y = 0;
	while (y < ray->scaled_height - ray->offset)
	{
		x = 0;
		while (x < MLX_WIDTH / RAYC)
		{
			pixel = get_imgs_pixel(ray->wall_tex, ray->img_start->x + x, ray->img_start->y + y, WALL_HEIGHT / ray->scaled_height);
			mlx_put_pixel(kissa->view->mlx_scene, (uint32_t)ray->screen_start->x + x, (uint32_t)ray->screen_start->y + y, pixel);
			x++;
		}
		y++;
	}
}

void	draw_cats(t_cub3d *kissa)
{
	int		i;
	t_obj	*cat;

	i = 0;
	while (i < kissa->cat_count)
	{
		cat = kissa->cats[i];
		if (cat->seen_by)
		{
			if (cat->view_dir >= M_PI / 4 && cat->view_dir < M_PI / 4 * 3)
				cat->cat_i = 1;
			else if (cat->view_dir >= M_PI / 4 * 3 && cat->view_dir < M_PI / 4 * 5)
				cat->cat_i = 0;
			else if (cat->view_dir >= M_PI / 4 * 5 && cat->view_dir < M_PI / 4 * 7)
				cat->cat_i = 2;
			else
				cat->cat_i = 3;
			if (!cat->caught)
				draw_cat(kissa, cat, cat->seen_by);
		}
		i++;
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
	cast_rays(kissa);
	reset_scene_image(kissa);
	while (i < RAYC)
	{
		draw_column(kissa, kissa->ray_array[i]);
		i++;
	}
	draw_cats(kissa);
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_scene, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_scene->instances, Z_SCENE);
}
