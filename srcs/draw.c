/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:43:44 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/19 11:27:08 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	cast_rays(t_cub3d *kissa)
{
	int	i;
	float	start_rot;
	t_ray	*ray;

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
		ray->scale = 1 / (ray->line_len * cos(fabs(kissa->player->rot - ray->rot)));
		ray->scaled_height = kissa->wall_height * ray->scale;
		ray->screen_start->x = i * MLX_WIDTH / RAYC;
		ray->screen_start->y = ray->scaled_height / 2 + MLX_HEIGHT / 2;
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

uint32_t	get_imgs_pixel(t_cub3d *kissa, t_ray *ray, int x, int y)
{
	(void)kissa;
	int			pixel_index;
	uint8_t		*pixel;
	uint32_t	color;

	x *= WALL_HEIGHT / ray->scaled_height;
	y *= WALL_HEIGHT / ray->scaled_height;
	if ((uint32_t)x >= ray->wall_tex->width)
		x = x % ray->wall_tex->width;
	pixel_index = (y * ray->wall_tex->width + x) * (32 / 8);
	pixel = ray->wall_tex->pixels + pixel_index;
	color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | 255;
	return (color);
}

void	draw_texture(t_cub3d *kissa, t_ray *ray)
{
	int	y;
	int	x;
	int	img_x_start;
	int	img_y_start;
	
	img_y_start = 0;
	if (ray->rot == kissa->player->rot)
		printf("\tstart (%f, %f), end (%f, %f)\n", kissa->player->x, kissa->player->y, ray->x, ray->y);
	if (ray->side)
		img_x_start = floor(ray->scaled_height * (ray->x - floor(ray->x)));
	else
		img_x_start = floor(ray->scaled_height * (ray->y - floor(ray->y)));
	if (ray->scaled_height > MLX_HEIGHT)
	{
		img_y_start = (int)floor((ray->scaled_height - MLX_HEIGHT) / 2);
		ray->scaled_height = MLX_HEIGHT - 1;
	}
	y = 0;
	while (y < ray->scaled_height)
	{
		x = 0;
		while (x < MLX_WIDTH / RAYC)
		{
			if (ray->rot == kissa->player->rot && ((!x && !y) || (x == MLX_WIDTH / RAYC / 2 && y == floor(ray->scaled_height / 2)) || (x == MLX_WIDTH / RAYC - 1 && y == ray->scaled_height - 1)))
				printf("\t\tPutting pixel to screen (%f, %f) from image (%d, %d)\n", ray->screen_start->x + x, ray->screen_start->y - y, img_x_start + x, img_y_start + y);
			mlx_put_pixel(kissa->view->mlx_scene, ray->screen_start->x + x, ray->screen_start->y - y, get_imgs_pixel(kissa, ray, img_x_start + x, img_y_start + y));
			x++;
		}
		y++;
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
		if (i == RAYC / 2)
			printf("\tDrawing ray [%d], direction %f, len %f, scale %f, \n", i, kissa->ray_array[i]->rot, kissa->ray_array[i]->line_len, kissa->ray_array[i]->scale);
		draw_texture(kissa, kissa->ray_array[i]);
		i++;
	}
	printf("-----\n");
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_scene, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_scene->instances, Z_SCENE);
}
