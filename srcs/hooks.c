/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:35:39 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/11 06:27:50 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Cleanly exits with a custom message if 'ESC' or 'Q' is pressed.
*/
void	escape_hook(void *param)
{
	t_cub3d	*kissa;

	kissa = param;
	if (mlx_is_key_down(kissa->mlx, MLX_KEY_ESCAPE))
		quit_success(kissa, "Escape is not a victory!");
	if (mlx_is_key_down(kissa->mlx, MLX_KEY_Q))
		quit_success(kissa, "Quitting is not a victory!");
}

/*
	Cleanly exits with a custom message if window is closed.
*/
void	quit_hook(void *param)
{
	t_cub3d	*kissa;

	kissa = param;
	quit_success(kissa, "Closing the window is not a victory!");
}

/*
	Detects single or repeated presses of movement keys WASD (and arrow keys). 
	Directs the appropriate landing coordinate to move_to function.

	-------- keydata ------------
	MLX_PRESS: when key is pressed
	MLX_REPEAT: when key is held down

	-------- movement ----------
	MLX_KEY_W: up
	MLX_KEY_S: down
	MLX_KEY_A: left
	MLX_KEY_D: right
	MLX_KEY_LEFT: rotate left
	MLX_KEY_RIGHT: rotate right
*/
void	move_keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d	*kissa;
	float		dir_x;
	float		dir_y;
	int			rotate_flag;
	
	kissa = param;
	rotate_flag = 0;
	dir_x = 0;
	dir_y = 0;
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
	{
		if (keydata.key == MLX_KEY_W)
			dir_y += 1;
		if (keydata.key == MLX_KEY_S)
			dir_y -= 1;
		if (keydata.key == MLX_KEY_A)
			dir_x -= 1;
		if (keydata.key == MLX_KEY_D)
			dir_x += 1;
		if (keydata.key == MLX_KEY_LEFT)
			rotate_flag -= 1;
		if (keydata.key == MLX_KEY_RIGHT)
			rotate_flag += 1;
		move(kissa, kissa->player, dir_x, dir_y);
		rotate(kissa, kissa->player, rotate_flag);
	}
}

/*
	Checks if the player has moved, and updates the view accordingly, if no
	movement has occured, the function does nothing.
*/
void	update_hook(void *param)
{
	t_cub3d			*kissa;
	static t_vec	old_loc;
	static double	old_rot;

	kissa = (t_cub3d *) param;
	if (kissa->player->x == old_loc.x && kissa->player->y == old_loc.y
		&& kissa->player->rot == old_rot)
		return ;
	old_loc.x = kissa->player->x;
	old_loc.y = kissa->player->y;
	old_rot = kissa->player->rot;
	draw_scene(kissa);
	draw_mini_map(kissa);
	shoot_ray(kissa, kissa->player);
	dda_shoot_ray(kissa, kissa->player->rot, kissa->ray);
}
