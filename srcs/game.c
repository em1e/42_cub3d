/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:49 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/04 19:26:06 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_dir(t_obj *obj, char dir_char)
{
	if (dir_char == 'N')
		obj->dir = NORTH;
	else if (dir_char == 'E')
		obj->dir = EAST;
	else if (dir_char == 'S')
		obj->dir = SOUTH;
	else
		obj->dir = WEST;
}

int	is_wall(t_cub3d *kissa, float x, float y)
{
	if (kissa->map->array[(int)y][(int)x] == '1')
		return (1);
	return (0);
}

void	move(t_cub3d *kissa, t_obj *obj, int dir_x, int dir_y)
{
	float	new_x;
	float	new_y;
	
	new_x = obj->x + dir_x * MOVE_SPEED * sin(obj->dir);
	new_y = obj->y + dir_y * MOVE_SPEED * sin(obj->dir);
	if (is_wall(kissa, new_x, new_y))
		return ; // add bounceback
	obj->x = new_x;
	obj->y = new_y;
	move_player_texture(kissa, new_x, new_y);
	printf("Player pos = (%f, %f)\n", obj->x, obj->y);
}

void	rotate(t_obj *obj, int dir)
{
	if (dir)
	{
		obj->dir += ROT_SPEED;
		if (obj->dir > 2 * M_PI)
			obj->dir -= 2 * M_PI;
	}
	else
	{
		obj->dir -= ROT_SPEED;
		if (obj->dir < 0)
			obj->dir += 2 * M_PI;
	}
	// add rotation of minimap player
	printf("Player dir = %f\n", obj->dir);
}

void	play_game(t_cub3d *kissa)
{
	(void)kissa;
	printf("game started\n");
	draw_mini_map(kissa);
	mlx_loop_hook(kissa->mlx, escape_hook, kissa);
	mlx_close_hook(kissa->mlx, quit_hook, kissa);
	mlx_key_hook(kissa->mlx, (mlx_keyfunc)move_keyhook, kissa);
	mlx_loop(kissa->mlx);
}