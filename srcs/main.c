/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:38:06 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/19 10:50:53 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*

TASKS:

@ check map parsing
	---> ask around about map islands and how that shoul dbe handled
	---> fix parser to not give an error if there is an empty line inside map

@ add more png's and change all of the maps into valid .cub maps

@ fix drawing walls and dda
	---> make it so that walls aren't glitchy anymore
	---> make sure it doesn't segv when walking towards walls

@ fix minimap ray

@ add toggle for minimap

@ add starting screen

@ add bounceback

*/

/*
	Checks that the correct number of arguments are given.
*/
void	check_args(int argc)
{
	if (argc < 2)
		quit_error(NULL, NULL, "map is required");
	else if (argc > 2)
		quit_error(NULL, NULL, "too many arguments");
}

int	main(int argc, char **argv)
{
	t_cub3d	*kissa;

	check_args(argc);
	kissa = malloc(sizeof(t_cub3d));
	if (!kissa)
		quit_error(kissa, NULL, "memory allocation failure");
	init_kissa(kissa);
	kissa->map->file = argv[1];
	parse_kissa(kissa);
	init_map(kissa);
	init_mlx(kissa);
	play_game(kissa);
	printf("IT WORKS!!!\n");
	clean_kissa(kissa);
	return (0);
}
