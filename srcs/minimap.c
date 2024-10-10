/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:33:57 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/10 05:09:39 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Draws the image for the provided character at the given coordinate, which is 
	calculated using provided indexes and the tile size used.
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
	kissa->view->player_inst = mlx_image_to_window(kissa->mlx,
			kissa->view->mlx_player,
			(int)kissa->player->x * kissa->map->tile_size,
			(int)kissa->player->y * kissa->map->tile_size);
	if (kissa->view->player_inst < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
}

/*
	Gets the player instance from the view struct.
*/
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

/*
	Moves the player texture to the new coordinates.
*/
void	move_player_texture(t_cub3d *kissa, float new_x, float new_y)
{
	get_player(kissa->view)->y = new_y * kissa->map->tile_size;
	get_player(kissa->view)->x = new_x * kissa->map->tile_size;
}

/*
	Resets the minimap through clearing the old array and calculating the new
	cut down map around the player
*/
// void	update_minimap(t_cub3d *kissa)
// {
// 	reset_minimap();
// 	load_minimap_grid();
// 	draw_minimap(map);
// }
