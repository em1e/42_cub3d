/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:04:18 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/29 10:05:20 by jajuntti         ###   ########.fr       */
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
	map->height = 0;
	map->width = 0;
	map->first_map_line = 0;
	map->line = NULL;
	map->file = NULL;
	map->array = NULL;
	map->visited = NULL;
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
	ray->x = 0;
	ray->y = 0;
	ray->line_len = 0;
	ray->side = -1;
	ray->ray_len = NULL;
	ray->step_dir = NULL;
	ray->step_len = NULL;
	ray->dir = NULL;
	ray->screen_start = NULL;
	ray->img_start = NULL;
	ray->ray_len = new_vec(kissa);
	ray->step_dir = new_vec(kissa);
	ray->step_len = new_vec(kissa);
	ray->dir = new_vec(kissa);
	ray->screen_start = new_vec(kissa);
	ray->img_start = new_vec(kissa);
	ray->wall_tex = NULL;
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
	view->floor_inst = NULL;
	view->cat_inst = NULL;
	view->wall_inst = NULL;
	view->mlx_scene = NULL;
	view->original_cat = NULL;
	view->mlx_start = NULL;
	view->mlx_victory = NULL;
	view->mlx_dead = NULL;
	view->ray = NULL;
	view->mlx_no = NULL;
	view->mlx_we = NULL;
	view->mlx_so = NULL;
	view->mlx_ea = NULL;
	view->mlx_wall = NULL;
	view->mlx_floor = NULL;
	view->mlx_player = NULL;
	view->mlx_cat = NULL;
	view->mlx_scene = NULL;
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
	obj->x = 0;
	obj->y = 0;
	obj->rot = 0;
	obj->start_dir = 0;
	obj->cat_i = 0;
	obj->cat_j = 0;
	obj->type = 0;
	obj->caught = 0;
	obj->size = 0;
	obj->dir = NULL;
	obj->seen_by = NULL;
	obj->speed = speed;
	obj->dir = new_vec(kissa);
	return (obj);
}
