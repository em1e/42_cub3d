/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:38:06 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/24 07:51:28 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*

TASKS:

@ check bounceback

@ check map parsing

@ fix drawing east and north walls

@ DO THIS TODAY:
	~> add different cat textures onto the window

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
