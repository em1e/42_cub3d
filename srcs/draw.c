/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/08 08:54:07 by vkettune         ###   ########.fr       */
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
	// y = abs((i - kissa->map->height + 1) * kissa->map->tile_size);
	if (c == '1' \
		&& mlx_image_to_window(kissa->mlx, kissa->view->mlx_wall, x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	else if (c == '0' \
		&& mlx_image_to_window(kissa->mlx, kissa->view->mlx_floor, x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
}
