/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:25:06 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/30 07:56:21 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	handle_keys_pressed(t_cub3d *kissa, mlx_key_data_t keydata)
{
	float	dir_x;
	float	dir_y;
	int		rotate_flag;

	rotate_flag = 0;
	dir_x = 0;
	dir_y = 0;
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

/*
	Detects single or repeated presses of movement keys WASD (and arrow keys). 
	Directs the appropriate landing coordinate to move_to function.
*/
void	move_keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d	*kissa;

	kissa = param;
	if (kissa->paused && kissa->cats_caught == 0
		&& keydata.action == MLX_PRESS && keydata.key == MLX_KEY_ENTER)
	{
		kissa->paused = false;
		kissa->view->mlx_start->enabled = false;
	}
	if (kissa->paused)
		return ;
	if (BONUS && keydata.action == MLX_PRESS && keydata.key == MLX_KEY_SPACE)
	{
		catch_cats(kissa);
		return ;
	}
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
		handle_keys_pressed(kissa, keydata);
}

void	mouse_hook(double xpos, double ypos, void *param)
{
	t_cub3d			*kissa;
	static double	old_x;

	(void)ypos;
	kissa = param;
	if (kissa->paused)
		return ;
	if (xpos == old_x)
		return ;
	old_x = xpos;
	if (xpos > kissa->mlx->width * 0.5)
	{
		kissa->player->rot += ROT_SPEED * kissa->time_adjust * 0.25;
		if (kissa->player->rot > 2 * M_PI)
			kissa->player->rot -= 2 * M_PI;
	}
	else
	{
		kissa->player->rot -= ROT_SPEED * kissa->time_adjust * 0.25;
		if (kissa->player->rot < 0)
			kissa->player->rot += 2 * M_PI;
	}
	kissa->player->dir->x = cos(kissa->player->rot);
	kissa->player->dir->y = sin(kissa->player->rot);
}

/*
	Cleanly exits with a custom message if 'ESC' or 'Q' is pressed.
*/
void	escape_hook(void *param)
{
	t_cub3d	*kissa;

	kissa = param;
	if (mlx_is_key_down(kissa->mlx, MLX_KEY_ESCAPE)
		|| mlx_is_key_down(kissa->mlx, MLX_KEY_Q))
		quit_success(kissa, "Bye!");
}
