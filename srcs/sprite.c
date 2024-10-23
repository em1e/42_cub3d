/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:04:17 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/23 16:56:57 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// ------------------- ANIMATIONS -------------------

void	animate_cat(t_cub3d *kissa)
{
	int	i;

	i = 0;
	while (i < kissa->cat_count)
	{
		kissa->cats[i]->cat_j++;
		if (kissa->cats[i]->cat_j > 2)
			kissa->cats[i]->cat_j = 0;
		i++;
	}
}

// ------------------- CAT TEXTURE -------------------

void	choose_cat_type(t_cub3d *kissa, int cat_type, int *grid_x, int *grid_y)
{
	if (cat_type < 0 || cat_type > 7)
		quit_error(kissa, NULL, "Wrong cat selected");
	if (cat_type > 3)
	{
		*grid_y = 1;
		*grid_x = cat_type - 4;
	}
	else
	{
		*grid_y = 0;
		*grid_x = cat_type;
	}
}

/*
	changes the x and y so that theyactually find the correct cat texture from view->original_cat image

	- cat_i is for what cat animation are we talking
	- cat_j is for what texture of that animation are we on (changed in ani hook)
	- grid_x and grid_y are for navigating cat_type from the object,
	aka WHICH CAT TEXTURE TYPE do we need to grab
*/
void	get_cat_texture(t_cub3d *kissa, t_obj *cat, int *x, int *y)
{
	int	grid_x;
	int	grid_y;

	grid_x = 0;
	grid_y = 0;
	choose_cat_type(kissa, cat->cat_type, &grid_x, &grid_y);
	*x = grid_x * CAT_TEX_SIZE + cat->cat_j * CAT_TEX_SIZE + *x;
	*y = grid_y * CAT_TEX_SIZE + cat->cat_i * CAT_TEX_SIZE + *y;
}

uint32_t	get_cats_pixel(t_cub3d *kissa, t_obj *cat, int x, int y)
{
	(void)kissa;
	int			pixel_index;
	uint8_t		*pixel;
	uint32_t	color;
	mlx_image_t	*img;

	img = kissa->view->original_cat;
	get_cat_texture(kissa, cat, &x, &y);
	pixel_index = (y * img->width + x) * (32 / 8);
	pixel = img->pixels + pixel_index;
	if (pixel[3] == 0)
		return (0);
	color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | 255;
	return (color);
}

/*
	draws the cat object's texture pixel by pixel from a larger image
*/
void	draw_cat(t_cub3d *kissa, t_obj *cat, t_ray *ray)
{
	uint32_t	pixel;
	int			x;
	int			y;
	int			start_x;
	int			start_y;

	start_x = ray->index * MLX_WIDTH / RAYC;
	start_y = MLX_HEIGHT - ray->screen_start->y;
	y = 0;
	while (y <= CAT_TEX_SIZE)
	{
		x = 0;
		while (x <= CAT_TEX_SIZE)
		{
			pixel = get_cats_pixel(kissa, cat, x, y);
			if (pixel != 0 && start_x + x < MLX_WIDTH)
				mlx_put_pixel(kissa->view->mlx_scene, start_x + x, start_y + y, pixel);
			x++;
		}
		y++;
	}
}

// ------------------- CAT MOVEMENT -------------------

void	move_cats(t_cub3d *kissa)
{
	int	i;

	i = 0;
	while (i < kissa->cat_count)
	{
		kissa->map->array[(int)kissa->cats[i]->y][(int)kissa->cats[i]->x] = '0';
		if (move(kissa, kissa->cats[i], 0, 1) == 1)
		{
			//printf("ROTATE CAT %d\n", i + 1);
			rotate(kissa, kissa->cats[i], 1, NORTH);
		}
		kissa->map->array[(int)kissa->cats[i]->y][(int)kissa->cats[i]->x] = 'C';
		i++;
	}
	// printf("cat 1 : pos (%d, %d), rot = %f, dir->x %f, dir->y %f\n", (int)kissa->cats[0]->x, (int)kissa->cats[0]->y, kissa->cats[0]->rot, kissa->cats[0]->dir->x, kissa->cats[0]->dir->y);
	// printf("cat 2 : pos (%d, %d), rot = %f, dir->x %f, dir->y %f\n", (int)kissa->cats[1]->x, (int)kissa->cats[1]->y, kissa->cats[1]->rot, kissa->cats[1]->dir->x, kissa->cats[1]->dir->y);
	//print_map(kissa);
	// check player dir and cat dir

}

// ------------------- INIT CAT -------------------

void	init_cat_pos(t_cub3d *kissa, int cat, int x, int y)
{
	kissa->cats[cat]->x = (float)x + 0.5;
	kissa->cats[cat]->y = (float)y + 0.5;
	set_rot(kissa->cats[cat], 'E'); // default cat dir
	kissa->cats[cat]->dir->x = cos(kissa->cats[cat]->rot);
	kissa->cats[cat]->dir->y = sin(kissa->cats[cat]->rot);
	// kissa->cats[cat]->cat_type++;
	// if (kissa->cats[cat]->cat_type > 7)
	// kissa->cats[cat]->cat_type = 0;
}

/*
	creates cat objects if conditions are met
*/
void	create_cat_objs(t_cub3d *kissa)
{
	int		y;
	int		x;
	int		cat;

	y = 0;
	cat = 0;
	kissa->cats = malloc(sizeof(t_obj*) * kissa->cat_count);
	if (!kissa->cats)
		quit_error(kissa, NULL, "memory allocation failure");
	while (y < kissa->map->height)
	{
		x = 0;
		while (x < kissa->map->width)
		{
			if (kissa->map->array[y][x] == 'C')
			{
				kissa->cats[cat] = init_obj(kissa, CAT_SPEED);
				kissa->cats[cat]->size = CAT_SIZE;
				init_cat_pos(kissa, cat, x, y);
				cat++;
			}
			x++;
		}
		y++;
	}
	if (cat != kissa->cat_count)
		quit_error(kissa, NULL, "cat count issue");
}

/*
	places down cats or "C" on the map, and creates as many cat objects
	into kissa->cats as needed
*/
void	place_cats_down(t_cub3d *kissa)
{
	int	distance;
	int	i;
	int	y;
	int	x;

	y = 0;
	i = 0;
	distance = kissa->tile_count / 3;
	while (y < kissa->map->height)
	{
		x = 0;
		distance += 2;
		while (x < kissa->map->width)
		{
			if (!is_wall(kissa, x, y) || (x != kissa->player->x && y != kissa->player->y))
				i++;
			if (i >= distance && x != kissa->player->x && y != kissa->player->y)
			{
				i = 0;
				kissa->map->array[y][x] = 'C';
				kissa->cat_count++;
			}
			x++;
		}
		y++;
	}
	print_map(kissa);
	create_cat_objs(kissa);
}

/*
	calculates how many tiles are in the map
*/
void	calcuate_tile_count(t_cub3d *kissa)
{
	int	y;
	int	x;

	y = 0;
	while (y < kissa->map->height)
	{
		x = 0;
		while (x < kissa->map->width)
		{
			if (!is_wall(kissa, x, y))
				kissa->tile_count++;
			x++;
		}
		y++;
	}
}

void	init_cat_ani(t_cub3d *kissa)
{
	calcuate_tile_count(kissa);
	place_cats_down(kissa);
}

