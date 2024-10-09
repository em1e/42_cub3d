/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:49 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/09 14:42:40 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	t_vec	*old_pos;
	
	old_pos = new_vec(kissa);
	// using the delta time was recommended for performance reasns by others,
	// lets look into that at some point
	// * kissa->mlx->delta_time
	new_x = obj->x
	+ (dir_y * MOVE_SPEED * kissa->player->dir->x)
	+ (dir_x * MOVE_SPEED * kissa->player->dir->y);
	new_y = obj->y
	+ (dir_y * MOVE_SPEED * -kissa->player->dir->y)
	+ (dir_x * MOVE_SPEED * kissa->player->dir->x);

	if (new_x < 0 || new_x >= kissa->map->width
		|| new_y < 0 || new_y >= kissa->map->height)
		return ;

	old_pos->x = obj->x;
	old_pos->y = obj->y;
	
	if (old_pos)
		free(old_pos);

	if (is_wall(kissa, new_x, new_y))
		return ; // add bounceback

	obj->x = new_x;
	obj->y = new_y;
	move_player_texture(kissa, new_x, new_y);
	shoot_ray(kissa, obj);
	printf("Player pos = (%f, %f)\n", obj->x, obj->y);
	// ----------------------------------------------
}

void	rotate(t_cub3d *kissa, t_obj *obj, int rot)
{
	(void)kissa;
	if (!rot)
		return ;
	if (rot < 0)
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