/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:07:09 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/29 10:39:44 by jajuntti         ###   ########.fr       */
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
void	clean_view(t_cub3d *kissa, t_view *view)
{
	int	i;

	i = 0;
	while (i <= MMRAD * 2)
	{
		if (view->floor_inst && view->floor_inst[i])
			free(view->floor_inst[i]);
		if (view->wall_inst && view->wall_inst[i])
			free(view->wall_inst[i]);
		if (view->cat_inst && view->cat_inst[i])
			free(view->cat_inst[i]);
		i++;
	}
	if (view->wall_inst)
		free(view->wall_inst);
	if (view->floor_inst)
		free(view->floor_inst);
	if (view->cat_inst)
		free(view->cat_inst);
	if (view->original_cat)
		mlx_delete_image(kissa->mlx, view->original_cat);
	if (view->mlx_start)
		mlx_delete_image(kissa->mlx, view->mlx_start);
	delete_views_mlx(kissa, view);
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
	Cleans all the memory allocated for the cub3d struct.
*/
void	clean_kissa(t_cub3d *kissa)
{
	close_fd(kissa);
	if (kissa)
	{
		if (kissa->view)
			clean_view(kissa, kissa->view);
		if (kissa->map)
			clean_map(kissa->map);
		if (kissa->player)
			clean_obj(kissa->player);
		if (kissa->ray_array)
			clean_ray_array(kissa, kissa->ray_array);
		if (kissa->cats)
			clean_cats(kissa, kissa->cats);
		clean_file_content(kissa);
		free(kissa);
	}
}
