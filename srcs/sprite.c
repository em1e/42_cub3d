/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:04:17 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/22 13:46:46 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	animate_heart(t_cub3d *kissa, int direction)
{
	static int	i;
	mlx_image_t	*heart_img;
	mlx_image_t	*new_img;

	if (direction == 1)
		i++;
	else
		i--;
	heart_img = kissa->view->mlx_heart;
	new_img = kissa->view->mlx_heart_ani[i];
	ft_memcpy(heart_img->pixels, new_img->pixels,
		heart_img->width * heart_img->height * sizeof(int32_t));
	return (i);
}

// ------------------- CAT TEXTURE STUFFF -------------------

void	choose_cat_from_file(t_cub3d *kissa, int *grid_x, int *grid_y)
{
	if (CAT < 0 || CAT > 7)
		quit_error(kissa, NULL, "Wrong cat selected");
	if (CAT > 3)
	{
		*grid_y = 1;
		*grid_x = CAT - 4;
	}
	else
	{
		*grid_y = 0;
		*grid_x = CAT;
	}
}

uint32_t	get_cats_pixel(t_cub3d *kissa, mlx_image_t *img, int x, int y)
{
	(void)kissa;
	int			pixel_index;
	uint8_t		*pixel;
	uint32_t	color;

	pixel_index = (y * img->width + x) * (32 / 8);
	pixel = img->pixels + pixel_index;
	if (pixel[3] == 0)
		return (0);
	color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | 255;
	return (color);
}

/*
	grabs the cat texture pixel by pixel from given start coordinates from a larger image
	
	NOTE: type variable is what cat texture type are we talking about
*/
void	grab_cat(t_cub3d *kissa, int start_x, int start_y, int cat_x)
{
	t_ani			*cats;
	uint32_t	pixel;
	int				x;
	int				y;
	(void)cat_x;
	
	y = start_y;
	cats = kissa->view->cats;
	while (y <= start_y + 48)
	{
		x = start_x;
		while (x <= start_x + 48)
		{
			pixel = get_cats_pixel(kissa, cats->original, x, y);
			if (pixel == 0)
			{
				x++;
				continue;
			}
			mlx_put_pixel(kissa->view->mlx_scene, x - start_x + (MLX_WIDTH / 2), y - start_y + (MLX_HEIGHT / 2), pixel);
			x++;
		}
		y++;
	}
}

/*
	fills the t_ani struct images in view->cats according to the CAT defininition

	grid_x and grid_y are for finding the correct cat kind from the grid.
		- brown cat?, green cat?
	cat_x and cat_y are for finding the correct type of cat:
		- first row, second row, third... etc
*/
void	init_cat_ani(t_cub3d *kissa)
{
	int	cat_x;
	int	cat_y;
	int	grid_x;
	int	grid_y;

	cat_y = 0;
	grid_x = 0;
	grid_y = 0;
	choose_cat_from_file(kissa, &grid_x, &grid_y);
	cat_x = 0;
	grab_cat(kissa, (grid_x * 48 * 3) + (cat_x * 48), (grid_y * 48 * 4) + (cat_y * 48), cat_x);
	// loop through all iages of the cats ----------------------
	// while (cat_y < 4)
	// {
	// 	cat_x = 0;
	// 	while (cat_x < 3)
	// 	{
	// 		grab_cat(kissa, (grid_x * 48 * 3) + (cat_x * 48), (grid_y * 48 * 4) + (cat_y * 48), cat_x);
			
	// 		cat_x++;
	// 	}
	// 	kissa->view->cats->type++;
	// 	cat_y++;
	// }
	// ---------------------------------------------------------
}