/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/04 16:06:33 by vkettune         ###   ########.fr       */
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
	if (c == '1' \
		&& mlx_image_to_window(kissa->mlx, kissa->view->mlx_wall, x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	else if (c == '0' \
		&& mlx_image_to_window(kissa->mlx, kissa->view->mlx_floor, x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	// else
	// 	&& mlx_image_to_window(kissa->mlx, kissa->img_player, x, y) < 0)
	// 	quit_perror(kissa, NULL, "MLX42 failed");
	// else if (c == 'C' \
	// 	&& mlx_image_to_window(kissa->mlx, kissa->img_token, x, y) < 0)
	// 	quit_perror(kissa, NULL, "MLX42 failed");
}

/*
Draws all tiles of the map.
*/
void	draw_mini_map(t_cub3d *kissa)
{
	int	i;
	int	j;
	i = (int)kissa->player->x - 3;
	while (i < (int)kissa->player->x + 3 && i > 0 && i < kissa->map->height - 1)
	{
		j = (int)kissa->player->y - 3;
		while (j < (int)kissa->player->y + 3 && j > 0 && j < kissa->map->width - 1)
		{
			draw_tile(kissa, kissa->map->array[i][j], i, j);
			j++;
		}
		i++;
	}
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_player,
			(int)kissa->player->x, (int)kissa->player->y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
}