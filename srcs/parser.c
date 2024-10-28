/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:26:56 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/28 11:15:58 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Checks that the given file has the correct extension.

	NOTE:
	We are not checking for if the map has islands, a couple people brought that
	to my attention and we should probably add that check.

	e.g.

	111111111
	100000001
	100000001
	111111111

	1111111111111111111
	1000000000000000001
	1111000000000000001
	1000000000000000001
	1000000000000000011
	100000000N000000001
	1000000000000000001
	1000000000000001111
	1000000101010000001
	1111111111111111111

	in this case apparently only the lower map would be considered? I'm not sure
	if that's correct or out of scope for this project.

	or

	111111111
	100000001
	10000N001
	111111111

	1111111111111111111
	1000000000000000001
	1111000000000000001
	1000000000000000001
	1000000000000000011
	1000000000000000001
	1000000000000000001
	1000000000000001111
	1000000101010000001
	1111111111111111111

	while in this case only the upper map would be considered I guess
*/
void	handle_map_line(t_cub3d *kissa, char *line)
{
	int	i;
	int	contains_one;

	i = 0;
	contains_one = 0;
	if (!*line)
		quit_error(kissa, NULL, "empty line within or after map element");
	while (line[i])
	{
		if (!ft_strchr(" 01NESW", line[i]))
			quit_error(kissa, NULL, "invalid line in map element");
		if (line[i] == '1')
			contains_one = 1;
		i++;
	}
	if (!contains_one)
		quit_error(kissa, NULL, "invalid line in map element");
	if (ft_strlen(line) > (size_t)kissa->map->width)
		kissa->map->width = ft_strlen(line);
	kissa->map->height++;
}

/*
	Checks that all view elements are defined before the map element.
*/
static void	check_view_elements(t_cub3d *kissa)
{
	if (!kissa->no)
		quit_error(kissa, NULL, "North texture not defined before map element");
	if (!kissa->ea)
		quit_error(kissa, NULL, "East texture not defined before map element");
	if (!kissa->so)
		quit_error(kissa, NULL, "South texture not defined before map element");
	if (!kissa->we)
		quit_error(kissa, NULL, "West texture not defined before map element");
	if (*kissa->c < 0)
		quit_error(kissa, NULL, "ceiling color not defined before map element");
	if (*kissa->f < 0)
		quit_error(kissa, NULL, "floor color not defined before map element");
}

/*
	Handles the given line by checking if it's an element or a map line.
	
	It handles the elements by calling the appropriate function. When it gets to
	the first map line, it checks that all other content is there before
	handling the map.
*/
void	handle_element(t_cub3d *kissa, char *line, int line_i)
{
	skip_space(&line);
	if (!*line)
		quit_error(kissa, NULL, "scene file contains spaces on an empty line");
	if (!ft_strncmp(line, "NO", 2))
		get_texture(kissa, &kissa->no, line);
	else if (!ft_strncmp(line, "SO", 2))
		get_texture(kissa, &kissa->so, line);
	else if (!ft_strncmp(line, "WE", 2))
		get_texture(kissa, &kissa->we, line);
	else if (!ft_strncmp(line, "EA", 2))
		get_texture(kissa, &kissa->ea, line);
	else if (!ft_strncmp(line, "F", 1))
		get_rgb(kissa, kissa->f, line);
	else if (!ft_strncmp(line, "C", 1))
		get_rgb(kissa, kissa->c, line);
	else
	{
		check_view_elements(kissa);
		kissa->map->first_map_line = line_i;
	}
}

/*
	Parses the given file so that it can be utilized later.
*/
void	parse_kissa(t_cub3d *kissa)
{
	int	line_i;

	line_i = 0;
	check_file(kissa, kissa->map->file, ".cub");
	kissa->fd = open(kissa->map->file, O_RDONLY);
	kissa->map->line = ft_get_next_line(kissa->fd);
	if (!kissa->map->line)
		quit_error(kissa, NULL, "nothing in scene file");
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
	if (!kissa->map->first_map_line)
		quit_error(kissa, NULL, "no map in scene file");
	close_fd(kissa);
}
