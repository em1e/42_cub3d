/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:04:18 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/29 14:57:25 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Initializes a new map struct.
*/
t_map	*new_map(t_cub3d *kissa)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		quit_error(kissa, NULL, "memory allocation failure");
	*map = (t_map){0};
	return (map);
}

/*
	Initializes a new vec struct.
*/
t_vec	*new_vec(t_cub3d *kissa)
{
	t_vec	*vec;

	vec = malloc(sizeof(t_vec));
	if (!vec)
		quit_error(kissa, NULL, "memory allocation failure");
	*vec = (t_vec){0};
	vec->x = 0;
	vec->y = 0;
	return (vec);
}

/*
	Initializes a new ray struct.
*/
t_ray	*new_ray(t_cub3d *kissa)
{
	t_ray	*ray;

	ray = malloc(sizeof(t_ray));
	if (!ray)
		quit_error(kissa, NULL, "memory allocation failure");
	*ray = (t_ray){0};
	ray->side = -1;
	ray->ray_len = new_vec(kissa);
	ray->step_dir = new_vec(kissa);
	ray->step_len = new_vec(kissa);
	ray->dir = new_vec(kissa);
	ray->screen_start = new_vec(kissa);
	ray->img_start = new_vec(kissa);
	return (ray);
}

/*
	Initializes a new view struct.
*/
t_view	*new_view(t_cub3d *kissa)
{
	t_view	*view;

	view = malloc(sizeof(t_view));
	if (!view)
		quit_error(kissa, NULL, "memory allocation failure");
	*view = (t_view){0};
	return (view);
}

/*
	Initializes a new obj struct.
*/
t_obj	*new_obj(t_cub3d *kissa, float speed)
{
	t_obj	*obj;

	obj = malloc(sizeof(t_obj));
	if (!obj)
		quit_error(kissa, NULL, "memory allocation failure");
	*obj = (t_obj){0};
	obj->speed = speed;
	obj->dir = new_vec(kissa);
	return (obj);
}
