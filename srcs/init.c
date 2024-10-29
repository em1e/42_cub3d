/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:25:48 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/29 10:06:33 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_rays(t_cub3d *kissa)
{
	int	i;

	i = 0;
	kissa->ray_array = malloc(sizeof(t_ray*) * RAYC);
	if (!kissa->ray_array)
		quit_error(kissa, NULL, "memory allocation failure");
	while (i < RAYC)
	{
		kissa->ray_array[i] = new_ray(kissa);
		kissa->ray_array[i]->index = i;
		kissa->ray_array[i]->screen_start->x = i * MLX_WIDTH / RAYC;
		kissa->ray_array[i]->rot_diff = (RAYC / 2  - i) * RAYDIFF;
		kissa->ray_array[i]->fishey_adjust = cos(kissa->ray_array[i]->rot_diff);
		i++;
	}
}

/*
	Initializes the cub3d struct.
*/
void	init_kissa(t_cub3d *kissa)
{
	kissa->fd = -1;
	kissa->paused = true;
	kissa->tile_count = 0;
	kissa->total_cats = 0;
	kissa->cats_caught = 0;
	kissa->cats = NULL;
	kissa->column_width = MLX_WIDTH / RAYC;
	kissa->wall_height = WALL_HEIGHT;
	kissa->map = NULL;
	kissa->view = NULL;
	kissa->no = NULL;
	kissa->so = NULL;
	kissa->ea = NULL;
	kissa->we = NULL;
	kissa->ray_array = NULL;
	kissa->c[0] = -1;
	kissa->f[0] = -1;
	kissa->map = new_map(kissa);
	kissa->view = new_view(kissa);
	kissa->player = new_obj(kissa, PLAYER_SPEED);
	init_rays(kissa);
}
