/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:59:32 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/29 15:01:36 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Sets the initial rotation of the player object based on the given character.

	possible values:
		N: 90 degrees / 1.5 * M_PI
		E: 0 degrees / 0
		S: 270 degrees / 1.5 * M_PI
		W: 180 degrees / M_PI
*/
static void	set_rot(t_cub3d *kissa, t_obj *obj, char rot_char)
{
	if (rot_char == 'N')
		obj->rot = kissa->south;
	else if (rot_char == 'E')
		obj->rot = kissa->east;
	else if (rot_char == 'S')
		obj->rot = kissa->north;
	else
		obj->rot = kissa->west;
}

/*
	Initializes the player object with the given coordinates and rotation.
	This is called when the player start (NESW) is found in the map.
*/
void	init_player_pos(t_cub3d *kissa, char this, int i, int j)
{
	kissa->player->start_dir = this;
	set_rot(kissa, kissa->player, this);
	kissa->player->x = (float)j + 0.5;
	kissa->player->y = (float)i + 0.5;
	kissa->player->dir->x = cos(kissa->player->rot);
	kissa->player->dir->y = sin(kissa->player->rot);
	kissa->map->array[i][j] = 'P';
}

/*
	Replaces all spaces in the map with '1' to make sure the map is surrounded by
	walls.
*/
void	replace_spaces(t_cub3d *kissa)
{
	int	i;
	int	j;

	i = 0;
	while (i < kissa->map->height)
	{
		j = 0;
		if (i < kissa->map->height)
		{
			while (j < kissa->map->width)
			{
				if (kissa->map->array[i][j] == ' ')
					kissa->map->array[i][j] = '1';
				j++;
			}
		}
		i++;
	}
}

/*
	calculates how many tiles are in the map
*/
void	calcuate_tile_count(t_cub3d *kissa)
{
	int	y;
	int	x;

	y = 0;
	while (y < kissa->map->height)
	{
		x = 0;
		while (x < kissa->map->width)
		{
			if (!is_wall(kissa, x, y))
				kissa->tile_count++;
			x++;
		}
		y++;
	}
}

void	init_cat_pos(t_cub3d *kissa, int cat, int x, int y)
{
	kissa->cats[cat] = new_obj(kissa, CAT_SPEED);
	kissa->cats[cat]->size = WALL_HEIGHT * CAT_SIZE_FACTOR;
	kissa->cats[cat]->type = cat + 3;
	kissa->cats[cat]->x = (float)x + 0.5;
	kissa->cats[cat]->y = (float)y + 0.5;
	set_rot(kissa, kissa->cats[cat], 'E');
	kissa->cats[cat]->dir->x = cos(kissa->cats[cat]->rot);
	kissa->cats[cat]->dir->y = sin(kissa->cats[cat]->rot);
}
