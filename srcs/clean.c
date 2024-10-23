/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:07:09 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/23 10:24:55 by jajuntti         ###   ########.fr       */
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

	(void)map;
	i = 0;
	while (i <= MMRAD * 2)
	{
		if (view->floor_inst && view->floor_inst[i])
			free(view->floor_inst[i]);
		if (view->wall_inst && view->wall_inst[i])
			free(view->wall_inst[i]);
		i++;
	}
	if (view->wall_inst)
		free(view->wall_inst);
	if (view->floor_inst)
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

void	clean_ray_array(t_ray **ray_array)
{
	int	i;

	i = 0;
	while (i < RAYC)
	{
		clean_ray(ray_array[i]);
		i++;
	}
	free(ray_array);
	ray_array = NULL;
}

void	clean_cats(t_cub3d *kissa, t_obj **cats)
{
	int	i;

	i = 0;
	while (i < kissa->cat_count)
	{
		clean_obj(cats[i]);
		i++;
	}
	free(cats);
	cats = NULL;
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
		if (kissa->ray_array)
			clean_ray_array(kissa->ray_array);
		if (kissa->cats)
			clean_cats(kissa, kissa->cats);
		clean_file_content(kissa);
		mlx_close_window(kissa->mlx);
		free(kissa);
	}
}
