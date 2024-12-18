/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 04:12:10 by vkettune          #+#    #+#             */
/*   Updated: 2024/11/04 12:46:20 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	copy_short_line(t_cub3d *kissa, char **line)
{
	char	*new_line;

	new_line = ft_calloc(kissa->map->width + 1, sizeof(char));
	if (!new_line)
		quit_perror(kissa, NULL, "memory allocation error");
	ft_strlcpy(new_line, *line, ft_strlen(*line) + 1);
	free(*line);
	*line = new_line;
}

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
		if (i >= kissa->map->first_map_line)
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = 0;
			if ((int)ft_strlen(line) < kissa->map->width)
				copy_short_line(kissa, &line);
			kissa->map->array[i - kissa->map->first_map_line] = line;
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
	char	**array;

	array = kissa->map->array;
	this = array[i][j];
	if (!this)
		return ;
	if (this != '0' && kissa->player->start_dir)
		quit_error(kissa, NULL, "map element has more than one start");
	if (this != '0')
		init_player_pos(kissa, this, i, j);
	if (i == 0 || i == kissa->map->height -1 \
		|| j == 0 || j == kissa->map->width -1)
		quit_error(kissa, NULL, "map element is not surrounded by walls");
	if (array[i - 1][j] == ' ' || array[i][j + 1] == ' '
		|| array[i + 1][j] == ' ' || array[i][j - 1] == ' ')
		quit_error(kissa, NULL, "map element is not surrounded by walls");
	if (array[i - 1][j] == 0 || array[i][j + 1] == 0
		|| array[i + 1][j] == 0 || array[i][j - 1] == 0)
		quit_error(kissa, NULL, "map element is not surrounded by walls");
}

/*
	Checks that the map only has one starting point for the player and that the 
	map is surrounded by walls. The latter is done my making sure that empty (0) 
	or player start (NESW) tiles are not on the edges of the map and that they are 
	not next to undefined tiles (' ').
*/
static void	check_map(t_cub3d *kissa)
{
	int	i;
	int	j;

	i = 0;
	while (i < kissa->map->height)
	{
		j = 0;
		while (j < kissa->map->width)
		{
			if (kissa->map->array[i][j]
				&& ft_strchr("0NESW", kissa->map->array[i][j]))
				check_tile(kissa, i, j);
			j++;
		}
		i++;
	}
	if (!kissa->player->start_dir)
		quit_error(kissa, NULL, "map element missing player start");
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
	replace_spaces(kissa);
	print_map(kissa);
}
