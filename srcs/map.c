/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 04:12:10 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/03 21:54:06 by jajuntti         ###   ########.fr       */
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
static void	fill_map(t_cub3d *kissa)
{
	int		i;
	char	*line;

	i = 0;
	kissa->fd = open(kissa->map->file, O_RDONLY);
	if (kissa->fd == -1)
		quit_perror(kissa, kissa->map->file, "error opening scene file");
	line = ft_get_next_line(kissa->fd);
	while (line)
	{
		if (!line)
			quit_perror(kissa, NULL, "error reading scene file");
		if (i >= kissa->map->first_map_line)
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = 0;
			kissa->map->array[i - kissa->map->first_map_line] = line;
		}
		else
			free(line);
		i++;
		line = ft_get_next_line(kissa->fd);
	}
	close_fd(kissa);
}

/*
Initializes the visited array for flood fill using calloc.
Might not be needed...
void	init_visited(t_cub3d *kissa)
{
	int	i;

	i = 0;
	kissa->map->visited = calloc(kissa->map->height + 1, sizeof(char *));
	if (!kissa->map->visited)
		quit_perror(kissa, NULL, "memory allocation failure");
	while (i < kissa->map->height)
	{
		kissa->map->visited[i] = ft_strdup(kissa->map->array[i]);
		if (!kissa->map->visited[i])
			quit_perror(kissa, NULL, "memory allocation failure");
		i++;
	}
}
*/

/*
Gives an error if:
- the map tile is a player start and one has already been found
- the map tile is '0' or a player start, and is located on the edge of the map
- the tile up, right, down, or left of the given tile is not defined ' '.
*/
static void	check_tile(t_cub3d *kissa, int i, int j, int *start_flag)
{
	char	this;
	char	up;
	char	right;
	char	down;
	char	left;
	
	this = kissa->map->array[i][j];
	if (this != '0' && *start_flag)
		quit_error(kissa, NULL, "map element has more than one start");
	if (this != '0') 
		*start_flag = 1;
	if (i == 0 || i == kissa->map->height -1 \
		|| j == 0 || j == kissa->map->width -1)
		quit_error(kissa, NULL, "map is not surrounded by walls");
	up = kissa->map->array[i - 1][j];
	right = kissa->map->array[i][j + 1];
	down = kissa->map->array[i + 1][j];
	left = kissa->map->array[i][j - 1];
	if (up == ' ' || right == ' ' || down == ' '|| left == ' ')
		quit_error(kissa, NULL, "map is not surrounded by walls");
}

/*
Checks that the map only has one starting point for the player and that the 
map is surrounded by walls. The latter is done my making sure that empty (0) 
or player start (NESW) tiles are not on the edges of the map and that they are 
not next to undefined tiles (' ').
*/
static void	check_map(t_cub3d *kissa)
{
	int		i;
	int		j;
	int		start_flag;

	i = 0;
	start_flag = 0;
	while (i < kissa->map->height)
	{
		j = 0;
		while (j < kissa->map->width)
		{
			if (ft_strchr("0NESW", kissa->map->array[i][j]))
				check_tile(kissa, i, j, &start_flag);
			j++;
		}
		i++;
	}
}

/*
Checks the mapfile, fills the map and performs checks to ensure map is valid.
*/
void	init_map(t_cub3d *kissa)
{
	kissa->map->array = ft_calloc(kissa->map->height + 1, sizeof(char *));
	if (!kissa->map->array)
		quit_perror(kissa, NULL, "memory allocation error");
	fill_map(kissa);
	check_map(kissa); // TODO: should also set player starting position
	//init_visited(kissa);
	print_map(kissa);
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
		quit_perror(kissa, NULL, "mlx_init failed");
	kissa->map->mlx_no = convert_png(kissa, kissa->view->no);
	kissa->map->mlx_we = convert_png(kissa, kissa->view->we);
	kissa->map->mlx_so = convert_png(kissa, kissa->view->so);
	kissa->map->mlx_ea = convert_png(kissa, kissa->view->ea);
}
