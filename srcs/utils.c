/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:04:19 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/03 11:12:10 by jajuntti         ###   ########.fr       */
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

void	skip_space(char **str)
{
	while (*str && **str == ' ')
		(*str)++;
}

void	close_fd(t_cub3d *kissa)
{
	if (!kissa || kissa->fd == -1)
		return ;
	close(kissa->fd);
	kissa->fd = -1;	
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
