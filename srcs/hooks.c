/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:35:39 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/23 07:50:41 by vkettune         ###   ########.fr       */
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
	if (!kissa->start && keydata.action == MLX_PRESS && keydata.key == MLX_KEY_ENTER)
	{
		kissa->start = true;
		kissa->view->mlx_start->enabled = false;
	}
	if (!kissa->start)
		return ;
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
		rotate(kissa, kissa->player, rotate_flag, ROT_SPEED);
	}
}

/*
	Checks if the player has moved, and updates the view accordingly, if no
	movement has occured, the function does nothing.
*/
void	update_hook(void *param)
{
	t_cub3d			*kissa;
	static double		timer;
	// remove these if you can after taking away printfs ---------------
	static t_vec	old_loc;
	static double	old_rot;
	// ------------------------------------------------------------------

	kissa = (t_cub3d *) param;
	if (!kissa->start)
		return ;
	
	timer += kissa->mlx->delta_time;
	if (timer < (double) 0.05)
		return ;
	timer = 0;
	draw_scene(kissa);
	
	if (kissa->player->x == old_loc.x && kissa->player->y == old_loc.y
		&& kissa->player->rot == old_rot)
		return ;
	old_loc.x = kissa->player->x;
	old_loc.y = kissa->player->y;
	old_rot = kissa->player->rot;
	refresh_minimap(kissa);
}

void	mouse_hook(double xpos, double ypos, void *param)
{
	t_cub3d	*kissa;
	static double	old_x;
	(void)ypos;

	kissa = param;
	if (xpos == old_x)
		return ;
	old_x = xpos;
	if (xpos > MLX_WIDTH / 2)
	{
		kissa->player->rot += ROT_SPEED / 15;
		if (kissa->player->rot > 2 * M_PI)
			kissa->player->rot -= 2 * M_PI;
	}
	else
	{
		kissa->player->rot -= ROT_SPEED / 15;
		if (kissa->player->rot < 0)
			kissa->player->rot += 2 * M_PI;
	}
	kissa->player->dir->x = cos(kissa->player->rot);
	kissa->player->dir->y = sin(kissa->player->rot);
}

void	anim_update_hook(void *param)
{
	static double		timer;
	t_cub3d				*kissa;
	static int			direction;

	kissa = param;
	if (!kissa->start)
		return ;
	timer += kissa->mlx->delta_time;
	if (timer < (double) DELAY)
		return ;
	timer = 0;
	if (direction == 0)
		direction = 1;
	animate_cat(kissa);
}
