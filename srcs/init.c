/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:25:48 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/28 09:46:05 by vkettune         ###   ########.fr       */
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
	ray->side = -1;
	ray->ray_len = NULL;
	ray->step_dir = NULL;
	ray->step_len = NULL;
	ray->dir = NULL;
	ray->screen_start = NULL;
	ray->img_start = NULL;
	ray->ray_len = new_vec(kissa);
	ray->step_dir = new_vec(kissa);
	ray->step_len = new_vec(kissa);
	ray->dir = new_vec(kissa);
	ray->screen_start = new_vec(kissa);
	ray->img_start = new_vec(kissa);
	ray->wall_tex = NULL;
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
	view->floor_inst = NULL;
	view->cat_inst = NULL;
	view->wall_inst = NULL;
	view->mlx_scene = NULL;
	view->original_cat = NULL;
	view->mlx_start = NULL;
	view->mlx_victory = NULL;
	view->mlx_dead = NULL;
	view->ray = NULL;
	view->mlx_no = NULL;
	view->mlx_we = NULL;
	view->mlx_so = NULL;
	view->mlx_ea = NULL;
	view->mlx_wall = NULL;
	view->mlx_floor = NULL;
	view->mlx_player = NULL;
	view->mlx_cat = NULL;
	view->mlx_scene = NULL;
	return (view);
}

void	init_rays(t_cub3d *kissa)
{
	int	i;

	i = 0;
	kissa->ray_array = malloc(sizeof(t_ray*) * RAYC);
	if (!kissa->ray_array)
		quit_error(kissa, NULL, "memory allocation failure");
	while (i < RAYC)
	{
		kissa->ray_array[i] = new_ray(kissa);
		kissa->ray_array[i]->index = i;
		kissa->ray_array[i]->screen_start->x = i * MLX_WIDTH / RAYC;
		kissa->ray_array[i]->rot_diff = (RAYC / 2  - i) * RAYDIFF;
		kissa->ray_array[i]->fishey_adjust = cos(fabs(kissa->ray_array[i]->rot_diff));
		i++;
	}
}

/*
	Initializes a new obj struct.
*/
t_obj	*init_obj(t_cub3d *kissa, float speed)
{
	t_obj	*obj;

	obj = malloc(sizeof(t_obj));
	if (!obj)
		quit_error(kissa, NULL, "memory allocation failure");
	obj->x = 0;
	obj->y = 0;
	obj->rot = 0;
	obj->start_dir = 0;
	obj->cat_i = 0;
	obj->cat_j = 0;
	obj->type = 0;
	obj->caught = 0;
	obj->size = 0;
	obj->dir = NULL;
	obj->seen_by = NULL;
	obj->speed = speed;
	obj->dir = new_vec(kissa);
	return (obj);
}

/*
	Initializes the cub3d struct.
*/
void	init_kissa(t_cub3d *kissa)
{
	kissa->fd = -1;
	kissa->paused = true;
	kissa->tile_count = 0;
	kissa->total_cats = 0;
	kissa->cats_caught = 0;
	kissa->cats = NULL;
	kissa->column_width = MLX_WIDTH / RAYC;
	kissa->wall_height = WALL_HEIGHT;
	kissa->map = NULL;
	kissa->view = NULL;
	kissa->no = NULL;
	kissa->so = NULL;
	kissa->ea = NULL;
	kissa->we = NULL;
	kissa->ray_array = NULL;
	kissa->c[0] = -1;
	kissa->f[0] = -1;
	kissa->map = new_map(kissa);
	kissa->view = new_view(kissa);
	kissa->player = init_obj(kissa, PLAYER_SPEED);
	init_rays(kissa);
}

void	convert_textures(t_cub3d *kissa)
{
	kissa->view->mlx_start = convert_png(kissa, START_SCREEN);
	kissa->view->mlx_victory = convert_png(kissa, VICTORY_SCREEN);
	kissa->view->mlx_dead = convert_png(kissa, DEAD_SCREEN);
	kissa->view->mlx_no = convert_png(kissa, kissa->no);
	kissa->view->mlx_we = convert_png(kissa, kissa->we);
	kissa->view->mlx_so = convert_png(kissa, kissa->so);
	kissa->view->mlx_ea = convert_png(kissa, kissa->ea);
	kissa->view->mlx_wall = convert_png(kissa, WALL_TEXTURE);
	kissa->view->mlx_floor = convert_png(kissa, FLOOR_TEXTURE);
	kissa->view->mlx_player = convert_png(kissa, PLAYER_TEXTURE);
	kissa->view->mlx_cat = convert_png(kissa, MINI_CAT_TEXTURE);
	kissa->view->original_cat = convert_png(kissa, SPRITE_CAT_TEXTURE);
}

/*
Initializes MLX and stores the required images.
*/
void	init_mlx(t_cub3d *kissa)
{
	kissa->map->tile_size = 21;
	mlx_set_setting(MLX_STRETCH_IMAGE, 1);
	kissa->mlx = mlx_init(MLX_WIDTH, MLX_HEIGHT, "KISSA^3", true);
	if (!kissa->mlx)
		quit_perror(kissa, NULL, "mlx_init failed");
	convert_textures(kissa);
	init_cat_ani(kissa);
	printf("mlx_width = %d, mlx_height = %d\n", kissa->mlx->width, kissa->mlx->height);
	kissa->view->mlx_scene = mlx_new_image(kissa->mlx, kissa->mlx->width, kissa->mlx->height);
	if (!kissa->view->mlx_scene)
		quit_perror(kissa, NULL, "MLX42 failed");
	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_scene, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_set_instance_depth(kissa->view->mlx_scene->instances, Z_SCENE);
}
