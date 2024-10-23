/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:38:06 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/23 05:21:26 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*

TASKS:

@ check map parsing

@ add more png's and change all of the maps into valid .cub maps

@ fix drawing walls and dda
	---> make sure it doesn't segv when walking towards walls

@ add bounceback

@ DO THIS TODAY:
	~> add x amount of randomly placed cats according to how many empty tiles the map has.
	~> create function that finds and draws the cat on mlx_scene according to the rotation and dir
		of cat and player.
	~> make the cat wonder around randomly. start with it walking straight until it hits a wall and then turns 90 or 180 degrees
	~> add different cat textures onto the window (CAT into the struct and increment that)
	~> fix minimap ray

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
