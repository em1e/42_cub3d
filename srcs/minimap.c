/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:33:57 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/08 15:14:13 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
Draws all tiles of the map.
*/
void	draw_mini_map(t_cub3d *kissa)
{
	int	i;
	int	j;
	
	i = 0;
	while (i < kissa->map->height)
	{
		j = 0;
		while (j <= kissa->map->width)
		{
			printf("%c", kissa->map->array[i][j]);
			draw_tile(kissa, kissa->map->array[i][j], i, j);
			j++;
		}
		printf("\n");
		i++;
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
	get_player(kissa->view)->y = new_y * kissa->map->tile_size;
	get_player(kissa->view)->x = new_x * kissa->map->tile_size;
}

// void	update_minimap(t_cub3d *kissa)
// {
// 	reset_minimap();
// 	load_minimap_grid();

// should we use pixels instead of images? if not then we might have to delete images and put new ones on the window
// or have one of each (floor and wall texture) for each minimap tile, and just hide the ones we don't need
// according to this newly loaded minimap grid

// 	draw_minimap(map);
// }
