/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:43:44 by vkettune          #+#    #+#             */
/*   Updated: 2024/11/11 08:29:53 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

uint32_t	get_wall_pixel(t_cub3d *kissa, t_ray *ray, int x, int y)
{
	int			pixel_index;
	uint8_t		*pixel;
	uint32_t	color;
	float		scale_factor;

	color = 0;
	scale_factor = (float)ray->wall_tex->height / ray->scaled_height;
	if (ray->wall_tex == kissa->view->mlx_no
		|| ray->wall_tex == kissa->view->mlx_ea)
		x = kissa->column_width - x -1;
	x = ray->img_start->x + x;
	x = x * scale_factor;
	y = y * scale_factor;
	if ((uint32_t)x >= ray->wall_tex->width)
		x = x % ray->wall_tex->width;
	if (ray->wall_tex == kissa->view->mlx_no
		|| ray->wall_tex == kissa->view->mlx_ea)
		x = ray->wall_tex->width - x - 1;
	if (y >= (int)ray->wall_tex->height)
		return (0);
	pixel_index = (y * ray->wall_tex->width + x) * (32 / 8);
	pixel = ray->wall_tex->pixels + pixel_index;
	color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | 255;
	return (color);
}

void	draw_column(t_cub3d *kissa, t_ray *ray)
{
	int			y;
	int			x;
	uint32_t	pixel;

	y = 0;
	while (y < MLX_HEIGHT)
	{
		x = ray->screen_start->x;
		while (x < ray->screen_start->x + kissa->column_width)
		{
			if (y < ray->screen_start->y)
				pixel = rgb_to_pixel(kissa->c);
			else if (y >= ray->screen_start->y
				&& y < ray->screen_start->y + ray->scaled_height)
				pixel = get_wall_pixel(kissa, ray, x - ray->screen_start->x,
						ray->img_start->y + y - ray->screen_start->y);
			else
				pixel = rgb_to_pixel(kissa->f);
			if (pixel)
				mlx_put_pixel(kissa->view->mlx_scene, (uint32_t)x, (uint32_t)y,
					pixel);
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
	while (i < kissa->total_cats)
	{
		cat = kissa->cats[i];
		if (cat->seen_by)
		{
			if (cat->view_dir >= M_PI * 0.25 && cat->view_dir < M_PI * 0.75)
				cat->cat_i = 1;
			else if (cat->view_dir >= M_PI * 0.75
				&& cat->view_dir < M_PI * 1.25)
				cat->cat_i = 0;
			else if (cat->view_dir >= M_PI * 1.25
				&& cat->view_dir < M_PI * 1.75)
				cat->cat_i = 2;
			else
				cat->cat_i = 3;
			if (!cat->caught)
				draw_cat(kissa, cat, cat->seen_by);
			cat->seen_by = NULL;
		}
		i++;
	}
}

void	draw_scene(t_cub3d *kissa)
{
	int		i;
	t_ray	*ray;

	i = 0;
	while (i < RAYC)
	{
		ray = kissa->ray_array[i];
		ray->rot = fix_rot(kissa->player->rot - ray->rot_diff);
		cast_ray(kissa, ray);
		i++;
	}
	i = 0;
	while (i < RAYC)
		draw_column(kissa, kissa->ray_array[i++]);
	if (BONUS)
		draw_cats(kissa);
}

void	draw_game_state(t_cub3d *kissa, mlx_image_t *img)
{
	mlx_resize_image(img, MLX_WIDTH, MLX_HEIGHT);
	if (mlx_image_to_window(kissa->mlx, img, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(img->instances, Z_START);
}
