/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:38:06 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/04 15:47:29 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* initial ideas on how the structure of
		what this project could look like:
-----------------------------------------
	
	init
		- init variables
		- parse map
		 	- parse .cub file
				- split values into struct
			- handle possible map errors
		- create minimap
		- raycasting
	start game
		- calculate and scale screen
		- handle images
			- image error handling
		- put images onto the screen
		- ongoing 
			- calculate and scale images onto the screen when moving
			- keys being pressed
			- tracking of the mouse
			- movement updates (minimap and 3d space)
	end game
		- free everything
*/

#include "cub3d.h"

void	check_args(int argc)
{
	if (argc < 2)
		quit_error(NULL, NULL, "map is required");
	else if (argc > 2)
		quit_error(NULL, NULL, "too many arguments");
}

int	main(int argc, char **argv)
{
	t_cub3d *kissa;

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
	printf("IT WORKS!!!\n"); // testing
	clean_kissa(kissa);
	return (0);
}
