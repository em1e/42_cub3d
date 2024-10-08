/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:07:09 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/05 16:57:24 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	clean_view(t_view *view)
{
	// if (view->scene)
	// 	free(view->scene);
	free(view);
}

void	clean_obj(t_obj *obj)
{
	if (obj->dir)
		free(obj->dir);
	free(obj);
}

void	clean_kissa(t_cub3d *kissa)
{
	close_fd(kissa);
	if (kissa)
	{
		if (kissa->map)
			clean_map(kissa->map);
		if (kissa->view)
			clean_view(kissa->view);
		if (kissa->player)
			clean_obj(kissa->player);
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
		free(kissa);
	}
}
