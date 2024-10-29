/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:57:32 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/29 13:50:20 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int32_t	rgb_to_pixel(int *rgb)
{
	return (rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | 255);
}

/*
	flag = 0 checks for y rotation
	flag = 1 checks for x rotation
	rot is kissa->player->rot
	this function checks if the current rotation going in a
	positive or negative direction, in either the x or y axis
*/
int	check_dir(t_cub3d *kissa, float rot, int flag)
{
	if (flag == 0)
	{
		if (rot > kissa->west)
			return (-1);
		else
			return (1);
	}
	else if (flag == 1)
	{
		if (rot > kissa->north && rot < kissa->south)
			return (-1);
		else
			return (1);
	}
	return (-10);
}

float	fix_rot(float rot)
{
	if (rot > 2 * M_PI)
		rot -= 2 * M_PI;
	if (rot < 0)
		rot += 2 * M_PI;
	return (rot);
}

float	calc_distance(float x1, float y1, float x2, float y2)
{
	float	a;
	float	b;

	a = x1 - x2;
	b = y1 - y2;
	return (sqrt(a * a + b * b));
}

/*
	Cleans the given array of strings and sets it to NULL.
*/
void	clean_array(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}
