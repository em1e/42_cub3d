/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:49 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/08 09:04:28 by vkettune         ###   ########.fr       */
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
	
	old_pos->x = obj->x;
	old_pos->y = obj->y;
	
	// obj->dir->x = new_x - old_pos->x;
	// obj->dir->y = new_y - old_pos->y;
	
	if (old_pos)
		free(old_pos);
	
	// this is old code --------------------------
	// float	new_x;
	// float	new_y;
	
	// new_x = obj->x + dir_x * MOVE_SPEED * sin(obj->rot);
	// new_y = obj->y + dir_y * MOVE_SPEED * -cos(obj->rot);
	if (is_wall(kissa, new_x, new_y))
		return ; // add bounceback
	
	obj->x = new_x;
	obj->y = new_y;
	move_player_texture(kissa, new_x, new_y);
	printf("Player pos = (%f, %f)\n", obj->x, obj->y);
	// ----------------------------------------------
}

// void tilt_player_texture(t_cub3d *kissa)
// {
// 	// these calculations are wrong
// 	// and we don't need this
// 	double cos_theta = cos(kissa->player->dir);
// 	double sin_theta = sin(kissa->player->dir);
// 	float x = kissa->player->x;
// 	float y = kissa->player->y;

// 	int new_x = (int)(x * cos_theta - y * sin_theta);
// 	int new_y = (int)(x * sin_theta + y * cos_theta);

// 	printf("old = (%d, %d)\n", (int)x, (int)y);
// 	printf("new = (%d, %d)\n", new_x, new_y);
// 	get_player(kissa->view)->x = new_x;
// 	get_player(kissa->view)->y = new_y;
// 	// get_player(kissa->view)->x = new_x * kissa->map->tile_size;
// 	// get_player(kissa->view)->y = new_y * kissa->map->tile_size;
// }

void	rotate(t_cub3d *kissa, t_obj *obj, int rot)
{
	(void)kissa;
	if (!rot)
		return ;
	if (rot)
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
	obj->dir->x = sin(obj->rot);
	obj->dir->y = cos(obj->rot);
	
	// tilt_player_texture(kissa); // tries to add rotation to minimap
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