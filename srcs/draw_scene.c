/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:43:44 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/10 14:50:22 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	get_ray_lens(t_cub3d *kissa)
{
	int	i;
	float	start_rot;
	float	rot;
	float	rot_change;

	rot_change = M_PI / 2 / RAYC;
	start_rot = kissa->player->rot - M_PI / 4;
	if (start_rot < 0)
		start_rot += 2 * M_PI;
	printf("Player rot %f - start rot %f\n", kissa->player->rot, start_rot);
	i = 0;
	while (i < RAYC)
	{
		rot = start_rot + i * rot_change;
		if (rot > 2 * M_PI)
			rot -= 2 * M_PI;
		dda_shoot_ray(kissa, rot, kissa->ray);
		kissa->view->ray_array[i] = kissa->ray->line_len;
		printf("Ray %d rot %f and len %f\n", i, rot, kissa->view->ray_array[i]);
		i++; 
	}
}

void	draw_scene(t_cub3d *kissa)
{
	get_ray_lens(kissa);
	
}
