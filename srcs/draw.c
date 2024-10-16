/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:43:44 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/16 17:10:54 by jajuntti         ###   ########.fr       */
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
static void	cast_rays(t_cub3d *kissa)
{
	int	i;
	float	start_rot;
	float	fishey;
	t_ray	*ray;

	start_rot = kissa->player->rot - FOV / 2;
	if (start_rot < 0)
		start_rot += 2 * M_PI;
	// printf("Player rot %f - start rot %f\n", kissa->player->rot, start_rot);
	i = 0;
	while (i < RAYC)
	{
		ray = kissa->ray_array[i];
		ray->rot = start_rot + i * RAYDIFF;
		if (ray->rot > 2 * M_PI)
			ray->rot -= 2 * M_PI;
		cast_ray(kissa, ray->rot, ray);
		fishey = fabs(cos((kissa->player->rot - ray->rot)));
		// if (ray->side)
		// 	ray->scaled_height = kissa->wall_height / (ray->y - ray->step->y);
		// else
		// 	ray->scaled_height = kissa->wall_height / (ray->x - ray->step->x);
		// if (ray->line_len <= 0)
		// 	ray->scaled_height = MLX_HEIGHT - 1;
		// else if (ray->line_len > 0 && ray->line_len < 200)
		// 	ray->scaled_height = floor(kissa->wall_height / ray->line_len);
		// else
		// 	ray->scaled_height = 1;
		ray->scale = 1 / ray->line_len / fishey;
		ray->scaled_height = floor(kissa->wall_height * ray->scale);
		ray->screen_start->x = i * MLX_WIDTH / RAYC;
		ray->screen_start->y = ray->scaled_height / 2 + MLX_HEIGHT / 2;
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

uint32_t	get_imgs_pixel(t_cub3d *kissa, t_ray *ray, int x, int y)
{
	(void)kissa;
	int			pixel_index;
	uint8_t		*pixel;
	uint32_t	color;

	x *= WALL_HEIGHT / ray->scaled_height;
	y *= WALL_HEIGHT / ray->scaled_height;
	if ((uint32_t)x >= ray->wall_tex->width)
	{
		x = x % ray->wall_tex->width;
		// printf("Ray goes to next wall block!\n");
	}
	if ((uint32_t)x >= ray->wall_tex->width || (uint32_t)y >= ray->wall_tex->height)
		printf("Going to segfault at %dx / %d and %dy / %d (scale = %f)\n", x, ray->wall_tex->width, y, ray->wall_tex->height, ray->scale);
	pixel_index = (y * ray->wall_tex->width + x) * (32 / 8);
	pixel = ray->wall_tex->pixels + pixel_index;
	color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | 255; // manually building the color lets you set 255 for alpha
	return (color);
	//find_pixel = (y * ray->wall_tex->width + x) * (32 / 8);
	//pixel = ray->wall_tex->pixels + find_pixel;
	//return (*(uint32_t *)pixel);
}

void	draw_texture(t_cub3d *kissa, t_ray *ray, int flag)
{
	int	y;
	int	x;
	int	img_x_start;
	int	img_y_start;
	
	img_y_start = 0;
	// mlx_resize_image(ray->wall_tex, ray->scaled_height, ray->scaled_height);
	if (flag)
		printf("start (%f, %f), end (%f, %f)\n", kissa->player->x, kissa->player->y, ray->x, ray->y);
	if (ray->side)
	{
		// printf("X ray->y = %f ray->x = %f\n", ray->y, ray->x);
		img_x_start = floor(ray->scaled_height * (ray->x - floor(ray->x)));
	}
	else
	{
		// printf("Y ray->y = %f ray->x = %f\n", ray->y, ray->x);
		img_x_start = floor(ray->scaled_height * (ray->y - floor(ray->y)));
	}
	if (ray->scaled_height > MLX_HEIGHT)
	{
		img_y_start = (int)floor((ray->scaled_height - MLX_HEIGHT) / 2);
		// printf("Y offset is %d for scaled_height %f\n", img_y_start, ray->scaled_height);
		ray->scaled_height = MLX_HEIGHT - 1;
	}
	y = 0;
	while (y < ray->scaled_height)
	{
		x = 0;
		while (x < MLX_WIDTH / RAYC)
		{
			if (flag && ((!x && !y) || (x == MLX_WIDTH / RAYC / 2 && y == floor(ray->scaled_height / 2)) || (x == MLX_WIDTH / RAYC - 1 && y == ray->scaled_height - 1)))
				printf("\tPutting pixel to screen (%f, %f) from image (%d, %d)\n", ray->screen_start->x + x, ray->screen_start->y - y, img_x_start + x, img_y_start + y);
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
	int	flag;

	i = 0;
	cast_rays(kissa);
	reset_scene_image(kissa);
	while (i < RAYC)
	{
		flag = 0;
		if (i == RAYC / 2 - 1 || i == RAYC / 2 || i == RAYC / 2 + 1)
			flag = 1;
		if (flag)
			printf("Drawing ray [%d], direction %f, len %f, scale %f, \n", i, kissa->ray_array[i]->rot, kissa->ray_array[i]->line_len, kissa->ray_array[i]->scale);
		draw_texture(kissa, kissa->ray_array[i], flag);
		i++;
	}
	printf("-----\n");
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_scene, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_scene->instances, Z_SCENE);
}
