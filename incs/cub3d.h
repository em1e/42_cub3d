/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:28:20 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/02 15:50:49 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <stdlib.h>

typedef struct	s_point
{
	int	x;
	int	y;
} t_scale;

typedef struct	s_view
{
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	int		f[3];
	int		c[3];

} t_view;


typedef struct	s_map
{
	int		width;
	int		height;
	char	*file;
	char	*line;
	// add more stuff when needed
} t_map;

typedef struct	s_ray
{
	int	i;
	int	length;
	// add more stuff when needed
}	t_ray;

typedef struct	s_cub3d
{
	
	t_ray	ray;
	t_map	*map;
	t_view	*view;
	int		fd;
	// add more stuff when needed
} t_cub3d;


void	parse_kissa(t_cub3d *kissa);

// main.c
void	clean_kissa(t_cub3d *kissa);

// quit.c
void	quit_perror(t_cub3d *kissa, char *file, char *error_message);
void	quit_error(t_cub3d *kissa, char *file, char *error_message);

// utils.c
void	clean_array(char **array);
void	skip_space(char **str);

#endif