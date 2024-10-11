/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:33:57 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/11 11:35:23 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void	draw_map_line(t_cub3d *kissa, int line_i, int to_skip)
{
	int	column_i;
	int	x;
	int	y;

	column_i = to_skip;
	y = kissa->player->y + line_i - MMRAD;
	while (column_i <= MMRAD * 2 - to_skip)
	{
		if (line_i == MMRAD && column_i == MMRAD)
			column_i++;
		x = kissa->player->x + column_i - MMRAD;
		if (y < 0 || y >= kissa->map->height || x < 0 || x >= kissa->map->width)
			draw_tile(kissa, '1', line_i, column_i);
		else
			draw_tile(kissa, kissa->map->array[y][x], line_i, column_i);
		column_i++;
	}
}

/*
	Draws the image for the provided character at the given coordinate, which is 
	calculated using provided indexes and the tile size used.
*/
void	draw_mini_map(t_cub3d *kissa)
{
	int line_i;

	line_i = 0;
	mlx_delete_image(kissa->mlx, kissa->view->ray);
	while (line_i <= MMRAD * 2)
	{
		if (line_i == 0 || line_i == MMRAD * 2)
			draw_map_line(kissa, line_i, 3);
		else if (line_i < 3 || line_i > MMRAD * 2 - 3)
			draw_map_line(kissa, line_i, 1);
		else
			draw_map_line(kissa, line_i, 0);
		line_i++;
	}
}
