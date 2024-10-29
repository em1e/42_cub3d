/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:49 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/29 13:06:53 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Checks if the given coordinates are a wall.
	Returns 1 if the coordinates are a wall, 0 if not.
*/
int	is_wall(t_cub3d *kissa, float x, float y)
{
	if (kissa->map->array[(int)floor(y)][(int)floor(x)] == '1')
		return (1);
	return (0);
}

int	bounceback(t_cub3d *kissa, t_obj *obj, float new_x, float new_y)
{
	(void)obj;
	if (new_x < 0 || new_x >= kissa->map->width
		|| new_y < 0 || new_y >= kissa->map->height
		|| is_wall(kissa, new_x, new_y)
		|| is_wall(kissa, new_x + BUMPER_SIZE, new_y)
		|| is_wall(kissa, new_x - BUMPER_SIZE, new_y)
		|| is_wall(kissa, new_x, new_y + BUMPER_SIZE)
		|| is_wall(kissa, new_x, new_y - BUMPER_SIZE)
		|| kissa->map->array[(int)new_y][(int)new_x] == 'C'
		|| (kissa->map->array[(int)new_y][(int)new_x] == 'P' && obj->size != 0))
		return (1);
	return (0);
}

/*
Moves the player object in the direction of the player object by the given

dir_x: 1 for right, -1 for left, 0 for no movement
dir_y: 1 for up, -1 for down, 0 for no movement
obj: the player object in cub3d kissa
*/
int	move(t_cub3d *kissa, t_obj *obj, int dir_x, int dir_y)
{
	float	new_x;
	float	new_y;

	new_x = obj->x
		+ (dir_y * kissa->time_adjust * obj->speed * obj->dir->x)
		+ (dir_x * kissa->time_adjust * obj->speed * -obj->dir->y);
	new_y = obj->y
		+ (dir_y * kissa->time_adjust * obj->speed * obj->dir->y)
		+ (dir_x * kissa->time_adjust * obj->speed * obj->dir->x);
	if (bounceback(kissa, obj, new_x, new_y))
		return (1);
	obj->x = new_x;
	obj->y = new_y;
	return (0);
}

/*
	Rotates the player object in the direction of the player object by the given
	rotation.

	rot: 1 for right, -1 for left, 0 for no rotation
	obj: the player object in cub3d kissa
*/
void	rotate(t_cub3d *kissa, t_obj *obj, int rot, float amount)
{
	(void)kissa;
	if (!rot)
		return ;
	if (rot > 0)
		obj->rot += amount * kissa->time_adjust;
	else
		obj->rot -= amount * kissa->time_adjust;
	obj->rot = fix_rot(obj->rot);
	obj->dir->x = cos(obj->rot);
	obj->dir->y = sin(obj->rot);
}

/*
	Initializes the game loop and the hooks for the game.
*/
void	play_game(t_cub3d *kissa)
{
	calcuate_tile_count(kissa);
	place_cats(kissa, 0, 0, 0);
	draw_game_state(kissa, kissa->view->mlx_start);
	setup_minimap(kissa, 0, 0);
	draw_scene(kissa);
	mlx_loop_hook(kissa->mlx, escape_hook, kissa);
	mlx_close_hook(kissa->mlx, quit_hook, kissa);
	mlx_key_hook(kissa->mlx, (mlx_keyfunc)move_keyhook, kissa);
	mlx_cursor_hook(kissa->mlx, (mlx_cursorfunc)mouse_hook, kissa);
	mlx_loop_hook(kissa->mlx, update_hook, kissa);
	mlx_loop_hook(kissa->mlx, &anim_update_hook, kissa);
	printf("Game started! There are %d cats to catch!\n", kissa->total_cats);
	mlx_loop(kissa->mlx);
}
