/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:48:49 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/23 14:11:43 by jajuntti         ###   ########.fr       */
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
		obj->rot = SOUTH;
	else if (rot_char == 'E')
		obj->rot = EAST;
	else if (rot_char == 'S')
		obj->rot = NORTH;
	else
		obj->rot = WEST;
}

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
		|| kissa->map->array[(int)new_y][(int)new_x] == 'C')
	{
		// printf("new x %f, new y %f\n", new_x, new_y);
		return (1);
	}
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

	// using the delta time was recommended for performance reasns by others,
	// lets look into that at some point
	// * kissa->mlx->delta_time
	new_x = obj->x
	+ (dir_y * obj->speed * obj->dir->x)
	+ (dir_x * obj->speed * -obj->dir->y);
	new_y = obj->y
	+ (dir_y * obj->speed * obj->dir->y)
	+ (dir_x * obj->speed * obj->dir->x);

	if (bounceback(kissa, obj, new_x, new_y))
		return (1); // add bounceback
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
	{
		obj->rot += amount;
		if (obj->rot > 2 * M_PI)
			obj->rot -= 2 * M_PI;
	}
	else
	{
		obj->rot -= amount;
		if (obj->rot < 0)
			obj->rot += 2 * M_PI;
	}
	obj->dir->x = cos(obj->rot);
	obj->dir->y = sin(obj->rot);
}

void	draw_start(t_cub3d *kissa)
{
	int	start_x;
	int	start_y;

	start_x = 0;
	start_y = 0;
	if (kissa->view->mlx_start->width >= MLX_WIDTH || kissa->view->mlx_start->height >= MLX_HEIGHT)
		mlx_resize_image(kissa->view->mlx_start, MLX_WIDTH, MLX_HEIGHT);
	else
	{
		start_x = (MLX_WIDTH - kissa->view->mlx_start->width) / 2;
		start_y = (MLX_HEIGHT - kissa->view->mlx_start->height) / 2;
	}
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_start, start_x, start_y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_start->instances, Z_START);
}

/*
	Initializes the game loop and the hooks for the game.
*/
void	play_game(t_cub3d *kissa)
{
	printf("game started\n");
	draw_start(kissa);
	setup_minimap(kissa, 0, 0);
	draw_background(kissa);
	draw_scene(kissa);
	mlx_loop_hook(kissa->mlx, escape_hook, kissa);
	mlx_close_hook(kissa->mlx, quit_hook, kissa);
	mlx_key_hook(kissa->mlx, (mlx_keyfunc)move_keyhook, kissa);
	mlx_cursor_hook(kissa->mlx, (mlx_cursorfunc)mouse_hook, kissa);
	mlx_loop_hook(kissa->mlx, update_hook, kissa);
	mlx_loop_hook(kissa->mlx, &anim_update_hook, kissa);
	mlx_loop(kissa->mlx);
}