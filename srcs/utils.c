/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:04:19 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/29 09:57:48 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Closes the kissa->fd if it's open and sets it to -1.
*/
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

/*
	Checks if the given file is a valid file and not a directory.
*/
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
				quit_error(kissa, file, "incorrect file extension");
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

mlx_instance_t	*get_tile(t_view *view, int x, int y, char tile)
{
	mlx_image_t	*image;
	int			inst;

	inst = 0;
	image = 0;
	if (tile == '1')
	{
		image = view->mlx_wall;
		inst = view->wall_inst[y][x];
	}
	else if (tile == '0')
	{
		image = view->mlx_floor;
		inst = view->floor_inst[y][x];
	}
	else if (tile == 'C')
	{
		image = view->mlx_cat;
		inst = view->cat_inst[y][x];
	}
	if (image == 0)
		return (0);
	return (&image->instances[inst]);
}
