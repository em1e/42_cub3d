/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:38:06 by vkettune          #+#    #+#             */
/*   Updated: 2024/09/25 23:02:51 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


int	arg_error(int argc)
{
	if (argc != 2)
	{
		if (argc < 2)
			printf("error: map is required\n");
		else
			printf("error: too many arguments\n");
		return (1);
	}
	return (0);
}


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
int main(int argc, char **argv)
{
	t_cub3d kissa;

	if (arg_error(argc))
		return (1);
	
	main_parser(&kissa, argv);
	kissa.map.file = argv[1];
	printf("IT WORKS!!!\n");
	return (0);
}
