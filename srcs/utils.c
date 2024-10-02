/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:04:19 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/02 15:18:23 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clean_array(char **array)
{
	if (!array)
		return ;
	while (*array)
	{
		free(*array);
		*array = NULL;
		(*array)++;
	}
	free (array);
	array = NULL;
}

void	skip_space(char **str)
{
	while (*str && **str == ' ')
		(*str)++;
}