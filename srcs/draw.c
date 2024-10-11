/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:43:44 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/11 14:55:20 by jajuntti         ###   ########.fr       */
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
	// float	end_x[240];
	// float	end_y[240];

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
		cast_ray(kissa, rot, kissa->ray);
		kissa->view->ray_array[i] = kissa->ray->line_len;
		
		// if we want to calculate it through start and end point,
		// and draw the lines that way
		// end_x[i] = kissa->ray->x;
		// end_y[i] = kissa->ray->y;
		
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

void	draw_scene(t_cub3d *kissa)
{
	get_ray_lens(kissa);
}
