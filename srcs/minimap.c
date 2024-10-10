/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:33:57 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/10 13:48:38 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void	draw_map_line(t_cub3d *kissa, int line_i, int to_skip)
{
	int	column_i;
	int	x;
	int	y;

	column_i = to_skip;
	y = kissa->player->y + line_i - MM_RADIUS;
	while (column_i <= MM_RADIUS * 2 - to_skip)
	{
		if (line_i == MM_RADIUS && column_i == MM_RADIUS)
			column_i++;
		x = kissa->player->x + column_i - MM_RADIUS;
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
	mlx_delete_image(kissa->mlx, kissa->view->mlx_floor);
	mlx_delete_image(kissa->mlx, kissa->view->mlx_wall);
	kissa->view->mlx_wall = convert_png(kissa, kissa->wall_tex);
	kissa->view->mlx_floor = convert_png(kissa, kissa->floor_tex);
	while (line_i <= MM_RADIUS * 2)
	{
		if (line_i == 0 || line_i == MM_RADIUS * 2)
			draw_map_line(kissa, line_i, 3);
		else if (line_i < 3 || line_i > MM_RADIUS * 2 - 3)
			draw_map_line(kissa, line_i, 1);
		else
			draw_map_line(kissa, line_i, 0);
		line_i++;
	}	
	// while (i <= MM_RADIUS * 2)
	// {
	// 	j = kissa->player->x - MM_RADIUS;
	// 	while (j <= kissa->map->width)
	// 	{
	// 		printf("%c", kissa->map->array[i][j]);
	// 		draw_tile(kissa, kissa->map->array[i][j], i, j);
	// 		j++;
	// 	}
	// 	printf("\n");
	// 	i++;
	// }
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
