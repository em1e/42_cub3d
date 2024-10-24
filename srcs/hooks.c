/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:35:39 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/24 07:46:59 by vkettune         ###   ########.fr       */
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
	if (kissa->paused && kissa->cats_caught == 0 && keydata.action == MLX_PRESS && keydata.key == MLX_KEY_ENTER)
	{
		kissa->paused = false;
		kissa->view->mlx_start->enabled = false;
	}
	if (kissa->paused)
		return ;
	if (keydata.action == MLX_PRESS && keydata.key == MLX_KEY_SPACE)
	{
		// make sure to only catch cats that are in view
			// -> remmeber to reset disctance each time rays are casted
			// -> if cat->distance is 0 then don't catch the cat
		catch_cats(kissa);
		return ;
	}
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
		kissa->map->array[(int)kissa->player->y][(int)kissa->player->x] = '0';
		move(kissa, kissa->player, dir_x, dir_y);
		kissa->map->array[(int)kissa->player->y][(int)kissa->player->x] = 'P';
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

	kissa = (t_cub3d *) param;
	if (kissa->paused)
		return ;
	timer += kissa->mlx->delta_time;
	if (timer < (double) DELAY)
		return ;
	timer = 0;
	draw_scene(kissa);
	move_cats(kissa);
	refresh_minimap(kissa);
	if (kissa->cats_caught == kissa->cat_count)
	{
		kissa->paused = true;
		draw_game_state(kissa, kissa->view->mlx_victory);
	}
}

void	mouse_hook(double xpos, double ypos, void *param)
{
	t_cub3d	*kissa;
	static double	old_x;
	(void)ypos;

	kissa = param;
	if (kissa->paused)
		return ;
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
	if (kissa->paused)
		return ;
	timer += kissa->mlx->delta_time;
	if (timer < (double) DELAY)
		return ;
	timer = 0;
	if (direction == 0)
		direction = 1;
	animate_cat(kissa);
}
