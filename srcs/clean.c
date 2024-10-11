/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:07:09 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/11 07:54:13 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Cleans the map struct.
*/
void	clean_map(t_map *map)
{
	if (map->line)
		free(map->line);
	if (map->array)
		clean_array(map->array);
	if (map->visited)
		clean_array(map->visited);
	free(map);
}

/*
	Cleans the view struct.
*/
void	clean_view(t_map *map, t_view *view)
{
	int	i;

	i = 0;
	// if (view->scene)
	// 	free(view->scene);
	if (view->ray_array)
		free(view->ray_array);
	while (i < map->height)
	{
		free(view->floor_inst[i]);
		free(view->wall_inst[i]);
		view->wall_inst[i] = NULL;
		i++;
	}
	free(view->wall_inst);
	free(view->floor_inst);
	free(view);
}

/*
	Cleans the obj struct.
*/
void	clean_obj(t_obj *obj)
{
	if (obj->dir)
		free(obj->dir);
	free(obj);
}

/*
	Cleans the ray struct.
*/
void	clean_ray(t_ray *ray)
{
	if (ray->ray_len)
		free(ray->ray_len);
	if (ray->step)
		free(ray->step);
	if (ray->step_size)
		free(ray->step_size);
	if (ray->dir)
		free(ray->dir);
	free(ray);
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
	if (kissa->wall_tex)
		free(kissa->wall_tex);
	if (kissa->floor_tex)
		free(kissa->floor_tex);
	if (kissa->player_tex)
		free(kissa->player_tex);
}

/*
	Cleans all the memory allocated for the cub3d struct.
*/
void	clean_kissa(t_cub3d *kissa)
{
	close_fd(kissa);
	if (kissa)
	{
		if (kissa->view)
			clean_view(kissa->map, kissa->view);
		if (kissa->map)
			clean_map(kissa->map);
		if (kissa->player)
			clean_obj(kissa->player);
		if (kissa->ray)
			clean_ray(kissa->ray);
		clean_file_content(kissa);
		free(kissa);
	}
}
