/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cat_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:14:26 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/29 10:14:37 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// ------------------- ANIMATIONS -------------------

void	animate_cat(t_cub3d *kissa)
{
	int	i;

	i = 0;
	while (i < kissa->total_cats)
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
		*grid_x = (cat_type - 4);
	}
	else
	{
		*grid_y = 0;
		*grid_x = cat_type;
	}
}

/*
	changes the x and y so that theyactually find the correct cat 
	texture from view->original_cat image

	- cat_i is for what cat animation are we talking
	- cat_j is for what texture of that animation are we on (changed in ani hook)
	- grid_x and grid_y are for navigating cat_type from the object,
	aka WHICH CAT TEXTURE TYPE do we need to grab
*/
void	get_cat_texture(t_cub3d *kissa, t_obj *cat, int *x, int *y)
{
	int	grid_x;
	int	grid_y;

	grid_x = cat->cat_j;
	grid_y = cat->cat_i;
	choose_cat_type(kissa, cat->type, &grid_x, &grid_y);
	*x = (grid_x * CAT_TEX_SIZE * 3) + cat->cat_j * CAT_TEX_SIZE + *x;
	*y = (grid_y * CAT_TEX_SIZE * 4) + cat->cat_i * CAT_TEX_SIZE + *y;
}

uint32_t	get_cats_pixel(t_cub3d *kissa, t_obj *cat, int x, int y)
{
	int			pixel_index;
	uint8_t		*pixel;
	uint32_t	color;
	mlx_image_t	*img;

	img = kissa->view->original_cat;
	x = floor(x / cat->scaled_size * CAT_TEX_SIZE);
	y = floor(y / cat->scaled_size * CAT_TEX_SIZE);
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
	int			x;
	int			y;
	uint32_t	pixel;
	uint32_t	pix_x;
	uint32_t	pix_y;

	cat->screen_start_y = (MLX_HEIGHT - ray->screen_start->y)
		+ ((MLX_HEIGHT - ray->screen_start->y) / cat->distance);
	x = 0;
	y = 0;
	while (y < cat->scaled_size && cat->screen_start_y + y < MLX_HEIGHT)
	{
		x = 0;
		while (x < cat->scaled_size && cat->screen_start_x + x < MLX_WIDTH)
		{
			pix_x = cat->screen_start_x + x;
			pix_y = cat->screen_start_y + y;
			pixel = get_cats_pixel(kissa, cat, x, y);
			if (pixel != 0)
				mlx_put_pixel(kissa->view->mlx_scene, pix_x, pix_y, pixel);
			x++;
		}
		y++;
	}
}
