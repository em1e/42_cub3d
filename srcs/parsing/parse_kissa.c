/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_kissa.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:26:56 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/03 10:54:31 by jajuntti         ###   ########.fr       */
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
#include "libft.h"

/*
Returns 1 if given path is a directory and 0 if it's not.
*/
int	is_directory(char *filepath)
{
	int	fd;

	fd = open(filepath, O_RDONLY | O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		return (1);
	}
	return (0);
}

void	check_file(t_cub3d *kissa, char *file, char *ext)
{
	int	len;
	int	ext_len;
	int	fd;

	len = ft_strlen(file);
	if (ext)
	{
		ext_len = ft_strlen(ext);
		while (ext_len)
		{
			if (file[len - ext_len] != *ext)
				quit_error(kissa, NULL, "map file missing \".cub\" extension");
			ext_len--;
			ext++;
		}
	}
	if (is_directory(file))
		quit_error(kissa, file, "file is a directory");
	fd = open(file, O_RDONLY);
	if (fd == -1)
		quit_perror(kissa, file, "unable to open file");
	close(fd);
}

void	get_texture(t_cub3d *kissa, char **texture, char *line)
{
	if (*texture)
		quit_error(kissa, NULL, "duplicate element in map file");
	line += 2;
	skip_space(&line);
	if (!line)
		quit_error(kissa, NULL, "element value missing");
	check_file(kissa, line, NULL);
	*texture = ft_strdup(line); // these leak, make sure they are being freed properly in clean_kissa
	if (!*texture)
		quit_error(kissa, NULL, "memory allocation failure");
}

void	set_rgb(t_cub3d *kissa, int *rgb, char **rgb_arr, int rgb_i)
{
	char	*ptr;
	int		i;
	int		value;

	i = 0;
	ptr = rgb_arr[rgb_i];
	skip_space(&ptr);
	if (!*ptr)
		quit_error(kissa, NULL, "wrong RGB format");
	while (ptr[i]&& ptr[i] != ' ')
	{
		if (ptr[i] && !ft_isdigit(ptr[i]))
			quit_error(kissa, NULL, "wrong RGB format");
		i++;
	}
	value = ft_atoi(ptr);
	if (value < 0 || value > 255)
		quit_error(kissa, NULL, "RGB value out of range");
	rgb[rgb_i] = value;
}

void	get_rgb(t_cub3d *kissa, int *rgb, char *line)
{
	char	**rgb_arr;
	int		i;

	i = 0;
	if (*rgb >= 0)
		quit_error(kissa, NULL, "duplicate element in map file");
	line++;
	skip_space(&line);
	if (!ft_isdigit(*line))
		quit_error(kissa, NULL, "wrong RGB format");
	rgb_arr = ft_split(line, ",");
	if (!rgb_arr)
		quit_error(kissa, NULL, "memory allocation failure");
	while (rgb_arr[i])
	{
		if (i > 2)
		{
			clean_array(rgb_arr);
			quit_error(kissa, NULL, "too many values in element");
		}
		set_rgb(kissa, rgb, rgb_arr, i);
		i++;
	}
	clean_array(rgb_arr);
}

int	is_map_line(char *line)
{
	while (*line)
	{
		if (!ft_strchr(" 01NOESW\n", *line))
			return (0);
		line++;
	}
	return (1);
}

void	check_line(t_cub3d *kissa, char *line, int *map_start)
{
	if (is_map_line(line))
	{
		if (!*map_start)
			*map_start = 1;
		kissa->map->height++;
		if (ft_strlen(line) > (size_t)kissa->map->width)
			kissa->map->width = ft_strlen(line);
		return ;
	}
	if (*map_start)
		quit_error(kissa, NULL, "incorrect element order");
	skip_space(&line);
	if (*line && !ft_strncmp(line, "NO", 2))
		get_texture(kissa, &kissa->view->no, line);
	else if (*line && !ft_strncmp(line, "SO", 2))
		get_texture(kissa, &kissa->view->so, line);
	else if (*line && !ft_strncmp(line, "WE", 2))
		get_texture(kissa, &kissa->view->we, line);
	else if (*line && !ft_strncmp(line, "EA", 2))
		get_texture(kissa, &kissa->view->ea, line);
	else if (*line && !ft_strncmp(line, "F", 1))
		get_rgb(kissa, kissa->view->f, line);
	else if (*line && !ft_strncmp(line, "C", 1))
		get_rgb(kissa, kissa->view->c, line);
}

char *trim_line(t_cub3d *kissa)
{
	char	*new_line;
	int		len;
	
	new_line = ft_strdup(kissa->map->line);
	len = ft_strlen(kissa->map->line);
	if (ft_strchr(kissa->map->line, '\n') && kissa->map->line[len - 1] == '\n')
	{
		free(new_line);
		new_line = ft_strtrim(kissa->map->line, "\n");
		printf("line = |%s|\n", new_line); // testing
	}
	return (new_line);
}

void	parse_kissa(t_cub3d *kissa)
{
	int		map_start;
	char	*trimmed_line;
	int		len;

	map_start = 0;
	len = 0;
	check_file(kissa, kissa->map->file, ".cub");
	kissa->fd = open(kissa->map->file, O_RDONLY);
	kissa->map->line = ft_get_next_line(kissa->fd);
	while (kissa->map->line)
	{
		if (map_start && !is_map_line(kissa->map->line)) // added the map char check in here too
			quit_error(kissa, NULL, "invalid map");
		else if (kissa->map->line[0] != '\n')
		{
			trimmed_line = trim_line(kissa);
			check_line(kissa, trimmed_line, &map_start);
			free(trimmed_line);
		}
		free(kissa->map->line);
		kissa->map->line = ft_get_next_line(kissa->fd);
	}
	close_fd(kissa);
}
