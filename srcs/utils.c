/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:04:19 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/03 10:53:01 by jajuntti         ###   ########.fr       */
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