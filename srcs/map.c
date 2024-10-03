/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 04:12:10 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/03 11:46:22 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
Draws the image for the provided character at the given coordinate, which is 
calculated using provided indexes and the tile size used.
*/
// void	draw_tile(t_cub3d *kissa, char c, int i, int j)
// {
// 	int		x;
// 	int		y;

// 	x = j * kissa->map->tile_size;
// 	y = i * kissa->map->tile_size;
// 	if (c == '1' \
// 		&& mlx_image_to_window(kissa->mlx, kissa->img_wall, x, y) < 0)
// 		quit_perror(kissa, NULL, "MLX42 failed");
// 	else if (c == '0' \
// 		&& mlx_image_to_window(kissa->mlx, kissa->img_empty, x, y) < 0)
// 		quit_perror(kissa, NULL, "MLX42 failed");
// 	else if (c == 'E' \
// 		&& mlx_image_to_window(kissa->mlx, kissa->img_exit, x, y) < 0)
// 		quit_perror(kissa, NULL, "MLX42 failed");
// 	else if (c == 'P' \
// 		&& mlx_image_to_window(kissa->mlx, kissa->img_player, x, y) < 0)
// 		quit_perror(kissa, NULL, "MLX42 failed");
// 	else if (c == 'C' \
// 		&& mlx_image_to_window(kissa->mlx, kissa->img_token, x, y) < 0)
// 		quit_perror(kissa, NULL, "MLX42 failed");
// }

/*
Draws all tiles of the map.
*/
// void	draw_map(t_cub3d *kissa)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (i < kissa->map->height)
// 	{
// 		while (j < kissa->map->width)
// 		{
// 			draw_tile(kissa, kissa->map->array[i][j], i, j);
// 			j++;
// 		}
// 		j = 0;
// 		i++;
// 	}
// }

/*
Opens the map file, callocs for the map array, and stores each row from the 
file into the array using ft_get_next_line.
*/
void	fill_map(char *file, t_cub3d *kissa)
{
	int		i;
	char	*line;

	i = 0;
	kissa->fd = open(file, O_RDONLY);
	if (kissa->fd == -1)
		quit_perror(kissa, file, "Error reading map file");
	kissa->map->array = ft_calloc(kissa->map->height + 1, sizeof(char *));
	if (!kissa->map)
		quit_perror(kissa, NULL, "Memory allocation error");
	while (i < kissa->map->height)
	{
		line = ft_get_next_line(kissa->fd);
		if (!line)
			quit_perror(kissa, NULL, "Error reading file");
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = 0;
		if (!is_map_line(line))
		{
			free(line);
			continue ;
		}
		kissa->map->array[i] = line;
		
		i++;
	}
	close_fd(kissa);
}

/*
Initializes the visited array for flood fill using calloc.
*/
void	init_visited(t_cub3d *kissa)
{
	int	i;

	i = 0;
	kissa->map->visited = calloc(kissa->map->height + 1, sizeof(char *));
	if (!kissa->map->visited)
		quit_perror(kissa, NULL, "Memory allocation error");
	while (i < kissa->map->height)
	{
		kissa->map->visited[i] = ft_strdup(kissa->map->array[i]);
		if (!kissa->map->visited[i])
			quit_perror(kissa, NULL, "Memory allocation error");
		i++;
	}
}

void	check_start(t_cub3d *kissa)
{
	int	i;
	int	j;
	int	start_flag;

	i = 0;
	start_flag = 0;
	while (i < kissa->map->height)
	{
		j = 0;
		while (j < kissa->map->width)
		{
			printf("The char being hecked is: %c\n", kissa->map->array[i][j]);
			if (ft_strchr("NSEW", kissa->map->array[i][j]))
			{
				if (start_flag)
					quit_error(kissa, NULL, "map has more than one start");
				start_flag = 1;
			}
			j++;
		}
		i++;
	}
}

/*
Checks the mapfile, fills the map and performs checks to ensure map is valid.
*/
void	init_map(char *file, t_cub3d *kissa)
{
	// check all variables are found from map file (variables in kissa->view)
	fill_map(file, kissa);
	printf("%s\n", *kissa->map->array);
	printf("height %d\n", kissa->map->height);
	check_start(kissa);
	print_map(kissa);
	init_visited(kissa);
	//check_walls(kissa);
	// check walls in map
	// check players starting location and no duplicates
	// 
	// check_access(kissa);
}

/*
Initializes MLX and stores the required images.
*/
void	init_mlx(t_cub3d *kissa)
{
	kissa->map->tile_size = 21;
	mlx_set_setting(MLX_STRETCH_IMAGE, 1);
	kissa->mlx = mlx_init(1200, 800, "NOT QUITE VOID", true);
	if (!kissa->mlx)
		quit_perror(kissa, NULL, "MLX42 failed");
	kissa->map->mlx_no = convert_png(kissa, kissa->view->no);
	kissa->map->mlx_we = convert_png(kissa, kissa->view->we);
	kissa->map->mlx_so = convert_png(kissa, kissa->view->so);
	kissa->map->mlx_ea = convert_png(kissa, kissa->view->ea);
}
