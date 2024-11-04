/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:13:48 by jajuntti          #+#    #+#             */
/*   Updated: 2024/11/04 11:13:50 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	skip_space(char **str)
{
	while (*str && **str == ' ')
		(*str)++;
}

void	get_texture(t_cub3d *kissa, char **texture, char *line)
{
	int	i;

	i = 0;
	if (*texture)
		quit_error(kissa, NULL, "duplicate element in scene file");
	line += 2;
	skip_space(&line);
	if (!line)
		quit_error(kissa, NULL, "scene element value missing");
	while (line[i] && line[i] != ' ')
	{
		i++;
		if (line[i] == ' ')
			line[i] = 0;
	}
	check_file(kissa, line, ".png");
	*texture = ft_strdup(line);
	if (!*texture)
		quit_perror(kissa, NULL, "memory allocation failure");
}

int	check_commas(char *rgb_line)
{
	int	commas;

	commas = 0;
	while(*rgb_line)
	{
		if (*rgb_line == ',')
			commas++;
		if (commas > 2)
			return (1);
		rgb_line++;
	}
	if (commas != 2)
		return (1);
	return (0);
}

void	set_rgb(t_cub3d *kissa, int *rgb, char **rgb_arr, int rgb_i)
{
	char	*ptr;
	int		i;
	int		value;
	int		flag;

	i = 0;
	flag = 0;
	ptr = rgb_arr[rgb_i];
	skip_space(&ptr);
	if (!*ptr)
		clean_quit(kissa, rgb_arr, "empty RGB value");
	while (ptr[i])
	{
		if ((ptr[i] != ' ' && !ft_isdigit(ptr[i])) || (flag && ptr[i] != ' '))
			clean_quit(kissa, rgb_arr, "wrong RGB format");
		if (ptr[i] == ' ')
			flag = i;
		else if (i > 2)
			clean_quit(kissa, rgb_arr, "too many digits in RGB value");
		i++;
	}
	value = ft_atoi(ptr);
	if (value < 0 || value > 255)
		clean_quit(kissa, rgb_arr, "RGB value out of range");
	rgb[rgb_i] = value;
}

void	get_rgb(t_cub3d *kissa, int *rgb, char *line)
{
	char	**rgb_arr;
	int		i;

	i = 0;
	if (*rgb >= 0)
		quit_error(kissa, NULL, "duplicate element in scene file");
	if (check_commas(line))
		quit_error(kissa, NULL, "too many/few commas in RGB value");
	line++;
	skip_space(&line);
	rgb_arr = ft_split(line, ",");
	if (!rgb_arr)
		quit_perror(kissa, NULL, "memory allocation failure");
	if (!rgb_arr[0] || !rgb_arr[1] || !rgb_arr[2] || rgb_arr[3])
		quit_error(kissa, NULL, "wrong RGB format");
	while (rgb_arr[i])
	{
		set_rgb(kissa, rgb, rgb_arr, i);
		i++;
	}
	clean_array(rgb_arr);
}
