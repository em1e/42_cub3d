/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:25:48 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/10 05:04:15 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Initializes a new map struct.
*/
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

/*
	Initializes a new vec struct.
*/
t_vec	*new_vec(t_cub3d *kissa)
{
	t_vec	*vec;

	vec = malloc(sizeof(t_vec));
	if (!vec)
		quit_error(kissa, NULL, "memory allocation failure");
	vec->x = 0;
	vec->y = 0;
	return (vec);
}


/*
	Initializes a new ray struct.
*/
t_ray	*new_ray(t_cub3d *kissa)
{
	t_ray	*ray;

	ray = malloc(sizeof(t_ray));
	if (!ray)
		quit_error(kissa, NULL, "memory allocation failure");
	ray->x = 0;
	ray->y = 0;
	ray->line_len = 0;
	ray->ray_len = new_vec(kissa);
	ray->step = new_vec(kissa);
	ray->step_size = new_vec(kissa);
	return (ray);
}

/*
	Initializes a new view struct.
*/
t_view	*new_view(t_cub3d *kissa)
{
	t_view	*view;

	view = malloc(sizeof(t_view));
	if (!view)
		quit_error(kissa, NULL, "memory allocation failure");
	// view->scene = new_vec(kissa);
	view->player_inst = 0;
	return (view);
}

/*
	Initializes a new obj struct.
*/
t_obj	*init_player(t_cub3d *kissa)
{
	t_obj	*player;

	player = malloc(sizeof(t_obj));
	if (!player)
		quit_error(kissa, NULL, "memory allocation failure");
	player->x = 0;
	player->y = 0;
	player->rot = 0;
	player->start_dir = 0;
	player->dir = new_vec(kissa);
	return (player);
}

/*
	Initializes the cub3d struct.
*/
void	init_kissa(t_cub3d *kissa)
{
	kissa->fd = -1;
	kissa->map = NULL;
	kissa->view = NULL;
	kissa->no = NULL;
	kissa->so = NULL;
	kissa->ea = NULL;
	kissa->we = NULL;
	kissa->wall_tex = NULL;
	kissa->floor_tex = NULL;
	kissa->player_tex = NULL;
	kissa->wall_tex = ft_strdup("./textures/png/21black.png");
	kissa->floor_tex = ft_strdup("./textures/png/21white.png");
	kissa->player_tex = ft_strdup("./textures/png/21player.png");
	if (!kissa->wall_tex || !kissa->floor_tex || !kissa->player_tex)
		quit_error(kissa, NULL, "memory allocation failure");
	kissa->c[0] = -1;
	kissa->f[0] = -1;
	kissa->map = new_map(kissa);
	kissa->view = new_view(kissa);
	kissa->player = init_player(kissa);
	kissa->ray = new_ray(kissa);
}

/*
Initializes MLX and stores the required images.

NOTE:
for later if we want to use pixels instead of images in minimap:
	// kissa->view->mlx_wall = mlx_new_image(kissa->mlx, 100 * kissa->map->tile_size,
	// 	100 * kissa->map->tile_size);
	// kissa->view->mlx_floor = mlx_new_image(kissa->mlx, 100 * kissa->map->tile_size,
	// 	100 * kissa->map->tile_size);
	// kissa->view->mlx_player = mlx_new_image(kissa->mlx, 100 * kissa->map->tile_size,
	// 	100 * kissa->map->tile_size);
*/
void	init_mlx(t_cub3d *kissa)
{
	kissa->map->tile_size = 21;
	mlx_set_setting(MLX_STRETCH_IMAGE, 1);
	kissa->mlx = mlx_init(1200, 800, "NOT QUITE VOID", true);
	if (!kissa->mlx)
		quit_perror(kissa, NULL, "mlx_init failed");
	kissa->view->mlx_no = convert_png(kissa, kissa->no);
	kissa->view->mlx_we = convert_png(kissa, kissa->we);
	kissa->view->mlx_so = convert_png(kissa, kissa->so);
	kissa->view->mlx_ea = convert_png(kissa, kissa->ea);
	kissa->view->mlx_wall = convert_png(kissa, kissa->wall_tex);
	kissa->view->mlx_floor = convert_png(kissa, kissa->floor_tex);
	kissa->view->mlx_player = convert_png(kissa, kissa->player_tex);
}
