/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:35:39 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/30 07:57:21 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Cleanly exits with a custom message if window is closed.
*/
void	quit_hook(void *param)
{
	t_cub3d	*kissa;

	kissa = param;
	quit_success(kissa, "Bye!");
}

/*
	Checks if the player has moved, and updates the view accordingly, if no
	movement has occured, the function does nothing.
*/
void	update_hook(void *param)
{
	t_cub3d			*kissa;
	static double	timer;

	kissa = (t_cub3d *) param;
	if (kissa->paused)
		return ;
	timer += kissa->mlx->delta_time;
	if (timer < 1 / FPS)
		return ;
	kissa->time_adjust = timer;
	timer = 0;
	draw_scene(kissa);
	if (BONUS)
	{
		move_cats(kissa);
		refresh_minimap(kissa);
		if (kissa->total_cats != 0 && kissa->cats_caught == kissa->total_cats)
		{
			printf("PURRFECT! You found all the cats!\n");
			kissa->paused = true;
			draw_game_state(kissa, kissa->view->mlx_victory);
		}
	}
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
	if (timer < 1 / FPS)
		return ;
	timer = 0;
	if (direction == 0)
		direction = 1;
	animate_cat(kissa);
}
