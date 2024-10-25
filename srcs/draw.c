/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:43:44 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/25 10:23:36 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	cast_rays(t_cub3d *kissa)
{
	int	i;
	float	start_rot;
	t_ray	*ray;

	i = 0;
	while (i < kissa->total_cats)
		kissa->cats[i++]->seen_by = NULL;
	start_rot = fix_rot(kissa->player->rot - FOV / 2);
	i = 0;
	while (i < RAYC)
	{
		ray = kissa->ray_array[i];
		ray->rot = fix_rot(start_rot + i * RAYDIFF);
		cast_ray(kissa, ray);
		i++;
	}
}

int32_t	rgb_to_pixel(int *rgb)
{
	return (rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | 255);
}

uint32_t	get_wall_pixel(t_cub3d *kissa, t_ray *ray, int x, int y)
{
	int			pixel_index;
	uint8_t		*pixel;
	uint32_t	color;


	x = ray->img_start->x + x;
	x = x * ray->scale_factor;
	y = y * ray->scale_factor;
	if ((uint32_t)x >= ray->wall_tex->width)
		x = x % ray->wall_tex->width;
	if (ray->wall_tex == kissa->view->mlx_no || ray->wall_tex == kissa->view->mlx_ea)
		x = ray->wall_tex->width - x - 1;
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
	uint32_t	floor;
	uint32_t	ceiling;
	
	floor = rgb_to_pixel(kissa->c);
	ceiling = rgb_to_pixel(kissa->f);
	y = 0;
	while (y < MLX_HEIGHT)
	{
		x = ray->screen_start->x;
		while (x < ray->screen_start->x + kissa->column_width)
		{
			if (y < ray->screen_start->y)
				pixel = floor;
			else if (y >= ray->screen_start->y + ray->scaled_height - ray->offset)
				pixel = ceiling;
			else
				pixel = get_wall_pixel(kissa, ray, x - ray->screen_start->x, ray->img_start->y + y - ray->screen_start->y);
			mlx_put_pixel(kissa->view->mlx_scene, (uint32_t)x, (uint32_t)y, pixel);
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

void	draw_scene(t_cub3d *kissa)
{
	int	i;

	i = 0;
	cast_rays(kissa);
	while (i < RAYC)
		draw_column(kissa, kissa->ray_array[i++]);
	draw_cats(kissa);
}
