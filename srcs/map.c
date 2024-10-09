/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 04:12:10 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/09 11:12:02 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


/*
Opens the map file, callocs for the map array, and stores each row from the 
file into the array using ft_get_next_line.
*/
static void	fill_map(t_cub3d *kissa)
{
	int		i;
	char	*line;

	i = 0;
	kissa->fd = open(kissa->map->file, O_RDONLY);
	if (kissa->fd == -1)
		quit_perror(kissa, kissa->map->file, "error opening scene file");
	line = ft_get_next_line(kissa->fd);
	while (line)
	{
		if (!line)
			quit_perror(kissa, NULL, "error reading scene file");
		if (i >= kissa->map->first_map_line)
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = 0;
			kissa->map->array[i - kissa->map->first_map_line] = line;
			// kissa->map->array[kissa->map->height - (i - kissa->map->first_map_line) - 1] = line;
		}
		else
			free(line);
		i++;
		line = ft_get_next_line(kissa->fd);
	}
	close_fd(kissa);
}

/*
Gives an error if:
- the map tile is a player start and one has already been found
- the map tile is '0' or a player start, and is located on the edge of the map
- the tile up, right, down, or left of the given tile is not defined ' '.
*/
static void	check_tile(t_cub3d *kissa, int i, int j)
{
	char	this;
	
	this = kissa->map->array[i][j];
	if (this != '0' && kissa->player->start_dir)
		quit_error(kissa, NULL, "map element has more than one start");
	if (this != '0')
	{
		kissa->player->start_dir = this;
		set_rot(kissa->player, this);
		kissa->player->x = (float)j;
		kissa->player->y = (float)i;
		// kissa->player->y = (float)(kissa->map->height - 1 - i);
		kissa->player->dir->x = -cos(kissa->player->rot);
		kissa->player->dir->y = sin(kissa->player->rot);
		kissa->map->array[i][j] = '0';
	}
	if (i == 0 || i == kissa->map->height -1 \
		|| j == 0 || j == kissa->map->width -1)
		quit_error(kissa, NULL, "map is not surrounded by walls");
	if (kissa->map->array[i - 1][j] == ' ' || kissa->map->array[i][j + 1] == ' '
		|| kissa->map->array[i + 1][j] == ' '|| kissa->map->array[i][j - 1] == ' ')
		quit_error(kissa, NULL, "map is not surrounded by walls");
}

/*
Checks that the map only has one starting point for the player and that the 
map is surrounded by walls. The latter is done my making sure that empty (0) 
or player start (NESW) tiles are not on the edges of the map and that they are 
not next to undefined tiles (' ').
*/
static void	check_map(t_cub3d *kissa)
{
	int		i;
	int		j;

	i = 0;
	while (i < kissa->map->height)
	{
		j = 0;
		while (j < kissa->map->width)
		{
			if (ft_strchr("0NESW", kissa->map->array[i][j]))
				check_tile(kissa, i, j);
			j++;
		}
		i++;
	}
}

/*
Checks the mapfile, fills the map and performs checks to ensure map is valid.
*/
void	init_map(t_cub3d *kissa)
{
	kissa->map->array = ft_calloc(kissa->map->height + 1, sizeof(char *));
	if (!kissa->map->array)
		quit_perror(kissa, NULL, "memory allocation error");
	fill_map(kissa);
	check_map(kissa);
	//init_visited(kissa);
	print_map(kissa);
}
