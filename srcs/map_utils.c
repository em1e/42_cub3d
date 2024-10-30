/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:59:32 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/30 12:51:28 by jajuntti         ###   ########.fr       */
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

char	*get_cat_name(int	name)
{
	char *names[16];

	names[0] = "Purrlock Holmes";
	names[1] = "Nacho Nibbles";
	names[2] = "Snickers Napsalot";
	names[3] = "Mittens McFluff";
	names[4] = "Sir Pounce-a-Lot";
	names[5] = "Waffles Whiskerface";
	names[6] = "Pickles McSnuggle";
	names[7] = "Marshmallow Puff";
	names[8] = "Captain Snugglepants";
	names[9] = "Churro Whiskers";
	names[10] = "Tiny Toebeans";
	names[11] = "Fuzzbert Meowington";
	names[12] = "Sushi Roll";
	names[13] = "Jellybean Paws";
	names[14] = "Whisker Doodle";
	names[15] = "Princess Purrfect";
	return (names[name]);
}

void	init_cat_pos(t_cub3d *kissa, int cat, int x, int y)
{
	int	type;

	type = cat;
	kissa->cats[cat] = new_obj(kissa, CAT_SPEED);
	kissa->cats[cat]->size = WALL_HEIGHT * CAT_SIZE_FACTOR;
	kissa->cats[cat]->type = type;
	while (kissa->cats[cat]->type >= 8)
	{
		type -= 8;
		kissa->cats[cat]->type = type;
	}
	kissa->cats[cat]->name = get_cat_name(cat);
	kissa->cats[cat]->x = (float)x + 0.5;
	kissa->cats[cat]->y = (float)y + 0.5;
	set_rot(kissa, kissa->cats[cat], 'E');
	kissa->cats[cat]->dir->x = cos(kissa->cats[cat]->rot);
	kissa->cats[cat]->dir->y = sin(kissa->cats[cat]->rot);
}
