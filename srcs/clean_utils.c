/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:09:28 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/29 10:40:03 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Cleans the ray struct.
*/
static void	clean_ray(t_cub3d *kissa, t_ray *ray)
{
	(void)kissa;
	if (ray->ray_len)
		free(ray->ray_len);
	if (ray->step_dir)
		free(ray->step_dir);
	if (ray->step_len)
		free(ray->step_len);
	if (ray->dir)
		free(ray->dir);
	if (ray->screen_start)
		free(ray->screen_start);
	if (ray->img_start)
		free(ray->img_start);
	free(ray);
}

void	clean_ray_array(t_cub3d *kissa, t_ray **ray_array)
{
	int	i;

	i = 0;
	while (i < RAYC)
	{
		clean_ray(kissa, ray_array[i]);
		i++;
	}
	free(ray_array);
	ray_array = NULL;
}

/*
	Cleans the file content of the cub3d struct.
*/
void	clean_file_content(t_cub3d *kissa)
{
	if (kissa->no)
		free(kissa->no);
	if (kissa->so)
		free(kissa->so);
	if (kissa->we)
		free(kissa->we);
	if (kissa->ea)
		free(kissa->ea);
}

void	clean_cats(t_cub3d *kissa, t_obj **cats)
{
	int	i;

	i = 0;
	while (i < kissa->total_cats)
	{
		if (cats[i])
			clean_obj(cats[i]);
		i++;
	}
	if (cats)
		free(cats);
	cats = NULL;
}
