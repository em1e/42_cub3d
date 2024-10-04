/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/04 19:23:04 by vkettune         ###   ########.fr       */
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
	
/*
Draws all tiles of the map.
*/
void	draw_mini_map(t_cub3d *kissa)
{
	int	i;
	int	j;
	
	i = kissa->map->height - 1;
	while (i >= 0)
	{
		j = 0;
		while (j <= kissa->map->width && i >= 0)
		{
			draw_tile(kissa, kissa->map->array[i][j], i, j);
			j++;
		}
		i--;
	}
	kissa->view->player_inst = mlx_image_to_window(kissa->mlx, kissa->view->mlx_player,
		(int)kissa->player->x * kissa->map->tile_size,
		(int)kissa->player->y * kissa->map->tile_size);
	if (kissa->view->player_inst < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
}

mlx_instance_t	*get_player(t_view *view)
{
	mlx_image_t	*image;
	int			inst;

	image = view->mlx_player;
	if (image == 0)
		return (0);
	inst = view->player_inst;
	return (&image->instances[inst]);
}

void	move_player_texture(t_cub3d *kissa, float new_x, float new_y)
{
	get_player(kissa->view)->y = (int)new_y * kissa->map->tile_size;
	get_player(kissa->view)->x = (int)new_x * kissa->map->tile_size;
}
