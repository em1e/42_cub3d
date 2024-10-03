/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:04:19 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/03 03:48:24 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*clean_string(char *string)
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
	// this was segfaulting ---------------
	// if (!array)
	// 	return ;
	// while (*array != NULL)
	// {
	// 	free(*array);
	// 	*array = NULL;
	// 	(*array)++;
	// }
	// free(array);
	// array = NULL;
	// -------------------------------------
}

void	skip_space(char **str)
{
	while (*str && **str == ' ')
		(*str)++;
}

