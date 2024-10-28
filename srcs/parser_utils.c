/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:13:48 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/28 10:06:17 by vkettune         ###   ########.fr       */
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
	check_file(kissa, line, NULL);
	*texture = ft_strdup(line);
	if (!*texture)
		quit_perror(kissa, NULL, "memory allocation failure");
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
	while (ptr[i] && ptr[i] != ' ')
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
		quit_error(kissa, NULL, "duplicate element in scene file");
	line++;
	skip_space(&line);
	if (!ft_isdigit(*line))
		quit_error(kissa, NULL, "wrong RGB format");
	rgb_arr = ft_split(line, ",");
	if (!rgb_arr)
		quit_perror(kissa, NULL, "memory allocation failure");
	while (rgb_arr[i])
	{
		set_rgb(kissa, rgb, rgb_arr, i);
		i++;
	}
	if (i != 3)
	{
		clean_array(rgb_arr);
		quit_error(kissa, NULL, "wrong values in RGB element");
	}
	clean_array(rgb_arr);
}
