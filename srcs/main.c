/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:38:06 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/03 19:31:30 by jajuntti         ###   ########.fr       */
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

t_map	*new_map(t_cub3d *kissa)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		quit_error(kissa, NULL, "memory allocation failure");
	map->height = 0;
	map->width = 0;
	map->first_map_line = 0;
	map->line = NULL;
	map->file = NULL;
	map->array = NULL;
	map->visited = NULL;
	return (map);
}

t_view	*new_view(t_cub3d *kissa)
{
	t_view	*view;

	view = malloc(sizeof(t_view));
	if (!view)
		quit_error(kissa, NULL, "memory allocation failure");
	view->no = NULL;
	view->so = NULL;
	view->ea = NULL;
	view->we = NULL;
	view->c[0] = -1;
	view->f[0] = -1;
	return (view);
}

void	init_kissa(t_cub3d *kissa)
{
	kissa->fd = -1;
	kissa->map = NULL;
	kissa->view = NULL;
	kissa->map = new_map(kissa);
	kissa->view = new_view(kissa);
}

void	clean_kissa(t_cub3d *kissa)
{
	close_fd(kissa);
	if (kissa->map)
	{
		printf("it does clean map\n"); // testing
		if (kissa->map->line)
			free(kissa->map->line);
		if (kissa->map->array)
			clean_array(kissa->map->array);
		if (kissa->map->visited)
			clean_array(kissa->map->visited);
		free(kissa->map);
	}
	if (kissa->view)
	{	
		printf("it does clean view\n"); // testing
		if (kissa->view->no)
			free(kissa->view->no);
		if (kissa->view->so)
			free(kissa->view->so);
		if (kissa->view->we)
			free(kissa->view->we);
		if (kissa->view->ea)
			free(kissa->view->ea);
		free(kissa->view);
	}
}

void	start_game(t_cub3d *kissa)
{
	(void)kissa;
	printf("game started\n");
	// draw_map(kissa);
	// mlx_loop_hook(kissa->mlx, escape_hook, kissa);
	// mlx_close_hook(kissa->mlx, quit_hook, kissa);
	// mlx_key_hook(kissa->mlx, (mlx_keyfunc)move_keyhook, kissa);
	// mlx_loop(kissa->mlx);
}

int	main(int argc, char **argv)
{
	t_cub3d kissa;

	check_args(argc);
	init_kissa(&kissa);
	kissa.map->file = argv[1];
	parse_kissa(&kissa);
	init_map(&kissa);
	init_mlx(&kissa);
	start_game(&kissa);
	printf("IT WORKS!!!\n"); // testing
	clean_kissa(&kissa);
	return (0);
}
