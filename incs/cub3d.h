/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:28:20 by vkettune          #+#    #+#             */
/*   Updated: 2024/09/25 23:03:18 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_scale
{
	int x;
	int y;
} t_scale;

typedef struct s_img
{
	char *no;
	char *so;
	char *we;
	char *ea;
} t_img;


typedef struct s_map
{
	int width;
	int height;
	char *file;
	// add more stuff when needed
} t_map;

typedef struct s_ray
{
	int i;
	int length;
	// add more stuff when needed
}	t_ray;

typedef struct s_cub3d
{
	t_ray ray;
	t_map minimap;
	t_map map;
	// add more stuff when needed
} t_cub3d;

int	main_parser(t_cub3d *kissa, char **argv);
int main(int ac, char **av);

#endif