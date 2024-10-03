/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:26:56 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/03 19:35:22 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* .cub maps have many rules and edge cases, here's the basic info on it:

These can be in any order, separated by x amount of spaces, etc.

NO = north facing wall texture path
SO = south facing wall texture path
WE = west facing wall texture path
EA = east facing wall texture path

F = floor color RBG code
C = ceiling color RBG code

map's has the be last in the file.

1 = walls
0 = space
N, S, E, W = player for that map, and in what direction
is the player looking at in the beginning

map checks for:
	- space or player on the map edge
	- multiple players (combination of the 4 letters)
	- no space or player in map
	- etc.
*/
#include "cub3d.h"

void	handle_map_line(t_cub3d *kissa, char *line)
{
	int	i;
	int	contains_zero_or_one;

	i = 0;
	contains_zero_or_one = 0;
	if (!*line)
		quit_error(kissa, NULL, "empty line within or after map element");
	while (line[i])
	{
		if (!ft_strchr(" 01NESW", line[i]))
			quit_error(kissa, NULL, "invalid line in map element");
		if (line[i] == '0' || line[i] == '1')
			contains_zero_or_one = 1;
		i++;
	}
	if (!contains_zero_or_one)
		quit_error(kissa, NULL, "invalid line in map element");
	if (ft_strlen(line) > (size_t)kissa->map->width)
		kissa->map->width = ft_strlen(line);
	kissa->map->height++;
}

static void	check_view_elements(t_cub3d *kissa)
{
	if (!kissa->view->no)
		quit_error(kissa, NULL, "North texture not defined before map element");
	if (!kissa->view->ea)
		quit_error(kissa, NULL, "East texture not defined before map element");
	if (!kissa->view->so)
		quit_error(kissa, NULL, "South texture not defined before map element");
	if (!kissa->view->we)
		quit_error(kissa, NULL, "West texture not defined before map element");
	if (*kissa->view->c < 0)
		quit_error(kissa, NULL, "ceiling color not defined before map element");
	if (*kissa->view->f < 0)
		quit_error(kissa, NULL, "floor color not defined before map element");
}

void	handle_element(t_cub3d *kissa, char *line, int line_i)
{
	skip_space(&line);
	if (!*line)
		quit_error(kissa, NULL, "scene file contains spaces on an empty line");
	if (!ft_strncmp(line, "NO", 2))
		get_texture(kissa, &kissa->view->no, line);
	else if (!ft_strncmp(line, "SO", 2))
		get_texture(kissa, &kissa->view->so, line);
	else if (!ft_strncmp(line, "WE", 2))
		get_texture(kissa, &kissa->view->we, line);
	else if (!ft_strncmp(line, "EA", 2))
		get_texture(kissa, &kissa->view->ea, line);
	else if (!ft_strncmp(line, "F", 1))
		get_rgb(kissa, kissa->view->f, line);
	else if (!ft_strncmp(line, "C", 1))
		get_rgb(kissa, kissa->view->c, line);
	else
	{
		check_view_elements(kissa);
		kissa->map->first_map_line = line_i;
	}
}

void	parse_kissa(t_cub3d *kissa)
{
	int	line_i;

	line_i = 0;
	check_file(kissa, kissa->map->file, ".cub");
	kissa->fd = open(kissa->map->file, O_RDONLY);
	kissa->map->line = ft_get_next_line(kissa->fd);
	while (kissa->map->line)
	{
		if (kissa->map->line[ft_strlen(kissa->map->line) - 1] == '\n')
			kissa->map->line[ft_strlen(kissa->map->line) - 1] = 0;
		if (!kissa->map->first_map_line && *kissa->map->line)
			handle_element(kissa, kissa->map->line, line_i);
		if (kissa->map->first_map_line)
			handle_map_line(kissa, kissa->map->line);
		free(kissa->map->line);
		kissa->map->line = ft_get_next_line(kissa->fd);
		line_i++;
	}
	close_fd(kissa);
}
