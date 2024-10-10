/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/10 11:20:31 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Draws the image for the provided character at the given coordinate, which is 
	calculated using provided indexes and the tile size used.
*/
void	draw_tile(t_cub3d *kissa, char c, int i, int j)
{
	int	x;
	int	y;

	x = j * kissa->map->tile_size;
	y = i * kissa->map->tile_size;

	if (c == '1' && mlx_image_to_window(kissa->mlx, kissa->view->mlx_wall,
		x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	else if (c == '0' && mlx_image_to_window(kissa->mlx, kissa->view->mlx_floor,
		x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
}

/*
	Shoots a ray from the player object in the direction of the player object.
	While the ray is not hitting a wall, the ray is drawn on the screen.

	0xFF0000FF = red
	0x00FF00FF = green
	0x0000FFFF = blue
	0x00FFFFFF = white
	0x000000FF = black
	0xFF00FFFF = purple
	0xFFFF00FF = yellow
*/
void	shoot_ray(t_cub3d *kissa, t_obj *obj)
{
	double	px_x;
	double	px_y;

	px_x = obj->x * kissa->map->tile_size;
	px_y = obj->y * kissa->map->tile_size;
	kissa->view->ray = mlx_new_image(kissa->mlx, 100 * kissa->map->tile_size, 100 * kissa->map->tile_size);
	while (is_wall(kissa, px_x / kissa->map->tile_size, px_y / kissa->map->tile_size) == 0
		&& !(px_x / kissa->map->tile_size < 0 || px_x / kissa->map->tile_size >= kissa->map->width
		|| px_y / kissa->map->tile_size < 0 || px_y / kissa->map->tile_size >= kissa->map->height))
	{
		mlx_put_pixel(kissa->view->ray, (int)px_x, (int)px_y, 0xFF0000FF);
		px_x += 1 * obj->dir->x;
		px_y += 1 * obj->dir->y;
	}
	mlx_image_to_window(kissa->mlx, kissa->view->ray, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
//														DDA ALGORITHM															//
//////////////////////////////////////////////////////////////////////////

/*
	NOTE:

	I coudn't get the DDA algorithm to work properly with putting the ray pixels on screen,
	however, the algorithm itself seems to work. I will leave the code here for future reference
	if we want to get back to it or implement it in a different way e.g. in the scene.
*/

/*
	flag = 0 checks for y rotation
	flag = 1 checks for x rotation
	rot is kissa->player->rot
	this function checks if the current rotation going in a
	positive or negative direction, in either the x or y axis
*/
int	check_dir(float rot, int flag)
{
	if (flag == 0) // y -------------------------------
	{
		if (rot > (float)WEST)
			return (-1); // y is negative
		else 
			return (1); // y is positive or 0
	}
	else if (flag == 1) // x ---------------------------
	{
		if (rot > (float)NORTH && rot < (float)SOUTH)
			return (-1); // x is negative
		else 
			return (1); // x is positive or 0
	}
	return (-10);
}

/*
	Sets the length of the ray to the next wall in the x or y direction

	flag = 0 checks for y rotation
	flag = 1 checks for x rotation
*/
void	set_ray_len(float *ray_len_q, float step_q, t_ray *ray, int flag)
{
	if (step_q < 0)
		*ray_len_q = 0;
	else if (flag == 1)
		*ray_len_q = ray->step_size->x;
	else
		*ray_len_q = ray->step_size->y;
}

/*
	Initializes the DDA algorithm by setting the starting coordinates and step sizes
*/
void	init_dda(t_ray *ray, t_obj *obj)
{
	ray->x = obj->x;
	ray->y = obj->y;
	ray->line_len = 0;
	ray->step->x = check_dir(obj->rot, 1);
	ray->step->y = check_dir(obj->rot, 0);
	if (obj->dir->x == 0)
	{
		ray->step_size->x = 1;
		ray->step_size->y = HUGE_VAL;
	}
	else if (obj->dir->y == 0)
	{
		ray->step_size->y = 1;
		ray->step_size->x = HUGE_VAL;
	}
	else
	{
		ray->step_size->x = sqrt(1 + (obj->dir->y / obj->dir->x) * (obj->dir->y / obj->dir->x));
		ray->step_size->y = sqrt(1 + (obj->dir->x / obj->dir->y) * (obj->dir->x / obj->dir->y));
	}
}

/*
	Shoots a ray from the player object in the direction of the player object
	using the DDA algo. While the ray is not hitting a wall, the ray is drawn on the screen.

	This code works, is more optimized, bur is not as readable as function above
*/
void	dda_shoot_ray(t_cub3d *kissa, t_obj *obj, t_ray *ray)
{
	init_dda(ray, obj);
	if (ray->step->x == -10 || ray->step->y == -10)
		return ;
	set_ray_len(&ray->ray_len->x, ray->step->x, ray, 1);
	set_ray_len(&ray->ray_len->y, ray->step->y, ray, 0);
	while (is_wall(kissa, ray->x, ray->y) == 0)
	{
		if (ray->ray_len->x < ray->ray_len->y)
		{
			ray->x += ray->step->x;
			ray->line_len = ray->ray_len->x;
			ray->ray_len->x += ray->step_size->x;
		}
		else
		{
			ray->y += ray->step->y;
			ray->line_len = ray->ray_len->y;
			ray->ray_len->y += ray->step_size->y;
		}
	}
	printf("\nline len = %f\n", ray->line_len);
}
