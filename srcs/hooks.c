/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:35:39 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/04 15:38:10 by vkettune         ###   ########.fr       */
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
*/
void	move_keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d	*kissa;
	float		x;
	float		y;

	kissa = param;
	x = kissa->player->x;
	y = kissa->player->y;
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
	{
		if (keydata.key == MLX_KEY_W) // up
			move(kissa, kissa->player, 0, 1);
		else if (keydata.key == MLX_KEY_S) // down
			move(kissa, kissa->player, 0, -1);
		else if (keydata.key == MLX_KEY_A) // left
			move(kissa, kissa->player, -1, 0);
		else if (keydata.key == MLX_KEY_D) // right
			move(kissa, kissa->player, 1, 0);
		else if (keydata.key == MLX_KEY_LEFT)
			rotate(kissa->player, 0);
		else if (keydata.key == MLX_KEY_RIGHT)
			rotate(kissa->player, 1);
	}
}
