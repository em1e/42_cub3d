/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/08 16:16:35 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
Draws the image for the provided character at the given coordinate, which is 
calculated using provided indexes and the tile size used.
*/
void	draw_tile(t_cub3d *kissa, char c, int i, int j)
{
	int		x;
	int		y;

	x = j * kissa->map->tile_size;
	y = i * kissa->map->tile_size;
	// (void)c;
	// something like this should use --------------------------
	// if (c == '1')
	// {
	// 	mlx_put_pixel(kissa->view->mlx_wall, (int)x, (int)y, 0x000000);
	// 	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_wall, x, y) < 0)
	// 		quit_perror(kissa, NULL, "MLX42 failed");
	// }
	// else if (c == '0')
	// {
	// 	mlx_put_pixel(kissa->view->mlx_floor, (int)x, (int)y, 0xFFFFFF);
	// 	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_floor, x, y) < 0)
	// 		quit_perror(kissa, NULL, "MLX42 failed");
	// }
	//----------------------------------------------------------------

	// y = abs((i - kissa->map->height + 1) * kissa->map->tile_size);
	
	if (c == '1' \
		&& mlx_image_to_window(kissa->mlx, kissa->view->mlx_wall, x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	else if (c == '0' \
		&& mlx_image_to_window(kissa->mlx, kissa->view->mlx_floor, x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
}

void shoot_ray(t_cub3d *kissa, t_obj *obj)
{
	double x = obj->x;
	double y = obj->y;
	double dx = cos(obj->rot);
	double dy = sin(obj->rot);

	kissa->view->ray = mlx_new_image(kissa->mlx, 1 * kissa->map->tile_size, 1 * kissa->map->tile_size);
	while (kissa->map->array[(int)y][(int)x] != '1')
	{
		mlx_put_pixel(kissa->view->ray, (int)x, (int)y, 0xFF0000); // Draw the pixel
		if (mlx_image_to_window(kissa->mlx, kissa->view->ray, 0, 0) < 0)
			quit_perror(kissa, NULL, "MLX42 failed");
		x += dx * STEP_SIZE;
		y += dy * STEP_SIZE;
		printf("AAAAAAAAA\n");
	}
}
