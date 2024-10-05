/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:04:19 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/05 11:29:42 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*clean_string(char *string)
{
	if (string != NULL)
		free(string);
	return (NULL);
}

void	clean_array(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i] != NULL)
	{
		array[i] = clean_string(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

void	close_fd(t_cub3d *kissa)
{
	if (!kissa || kissa->fd == -1)
		return ;
	close(kissa->fd);
	kissa->fd = -1;	
}

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


/*
Prints the map array of given kissa struct into standard output.
*/
void	print_map(t_cub3d *kissa)
{
	int	i;

	i = 0;
	while (i < kissa->map->height)
		ft_putendl_fd(kissa->map->array[i++], 1);
	ft_putchar_fd('\n', 1);
}

/*
Prints the visited  array of given kissa struct into standard output.
*/
void	print_floodfill(t_cub3d *kissa)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < kissa->map->height)
	{
		while (j < kissa->map->width)
		{
			if (kissa->map->visited[i][j] == 0)
				ft_putchar_fd('0', 1);
			else
				ft_putchar_fd('X', 1);
			j++;
		}
		j = 0;
		ft_putchar_fd('\n', 1);
		i++;
	}
	ft_putchar_fd('\n', 1);
}
