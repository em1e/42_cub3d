/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:49 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/10 11:14:45 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Sets the initial rotation of the player object based on the given character.

	possible values:
		N: 90 degrees / 1.5 * M_PI
		E: 0 degrees / 0
		S: 270 degrees / 1.5 * M_PI
		W: 180 degrees / M_PI
*/
void	set_rot(t_obj *obj, char rot_char)
{
	if (rot_char == 'N')
		obj->rot = NORTH;
	else if (rot_char == 'E')
		obj->rot = EAST;
	else if (rot_char == 'S')
		obj->rot = SOUTH;
	else
		obj->rot = WEST;
}

/*
	Checks if the given coordinates are a wall.
	
	Returns 1 if the coordinates are a wall, 0 if not.
*/
int	is_wall(t_cub3d *kissa, float x, float y)
{
	if (kissa->map->array[(int)y][(int)x] == '1')
		return (1);
	return (0);
}

/*
Moves the player object in the direction of the player object by the given

dir_x: 1 for right, -1 for left, 0 for no movement
dir_y: 1 for up, -1 for down, 0 for no movement
obj: the player object in cub3d kissa
*/
void	move(t_cub3d *kissa, t_obj *obj, int dir_x, int dir_y)
{
	float	new_x;
	float	new_y;
	t_vec	*old_pos;
	
	old_pos = new_vec(kissa);
	// these could be put into it's own "new_player_pos" function --------------
	
	// using the delta time was recommended for performance reasns by others,
	// lets look into that at some point
	// * kissa->mlx->delta_time
	new_x = obj->x
	+ (dir_y * MOVE_SPEED * kissa->player->dir->x)
	+ (dir_x * MOVE_SPEED * -kissa->player->dir->y);
	new_y = obj->y
	+ (dir_y * MOVE_SPEED * kissa->player->dir->y)
	+ (dir_x * MOVE_SPEED * kissa->player->dir->x);

	if (new_x < 0 || new_x >= kissa->map->width
		|| new_y < 0 || new_y >= kissa->map->height)
		return ;
	// ------------------------------------------------------------------------
	old_pos->x = obj->x;
	old_pos->y = obj->y;
	
	if (old_pos)
		free(old_pos);

	if (is_wall(kissa, new_x, new_y))
		return ; // add bounceback
	obj->x = new_x;
	obj->y = new_y;
	printf("Player pos = (%f, %f)\n", obj->x, obj->y);
}

/*
	Rotates the player object in the direction of the player object by the given
	rotation.

	rot: 1 for right, -1 for left, 0 for no rotation
	obj: the player object in cub3d kissa
*/
void	rotate(t_cub3d *kissa, t_obj *obj, int rot)
{
	(void)kissa;
	if (!rot)
		return ;
	if (rot > 0)
	{
		obj->rot += ROT_SPEED;
		if (obj->rot > 2 * M_PI)
			obj->rot -= 2 * M_PI;
	}
	else
	{
		obj->rot -= ROT_SPEED;
		if (obj->rot < 0)
			obj->rot += 2 * M_PI;
	}
	obj->dir->x = cos(obj->rot);
	obj->dir->y = sin(obj->rot);
	printf("Player dir = %f\n", obj->rot);
}

/*
	Initializes the game loop and the hooks for the game.
*/
void	play_game(t_cub3d *kissa)
{
	(void)kissa;
	printf("game started\n");
	draw_mini_map(kissa);
	mlx_loop_hook(kissa->mlx, escape_hook, kissa);
	mlx_close_hook(kissa->mlx, quit_hook, kissa);
	mlx_key_hook(kissa->mlx, (mlx_keyfunc)move_keyhook, kissa);
	mlx_loop_hook(kissa->mlx, update_hook, kissa);
	mlx_loop(kissa->mlx);
}