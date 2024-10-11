/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/11 06:10:05 by vkettune         ###   ########.fr       */
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
	THIS IS THE OLD SHOOT_RAY FUNCTION
*/
// void	shoot_ray(t_cub3d *kissa, t_obj *obj)
// {
// 	double	px_x;
// 	double	px_y;

// 	px_x = obj->x * kissa->map->tile_size;
// 	px_y = obj->y * kissa->map->tile_size;
// 	kissa->view->ray = mlx_new_image(kissa->mlx, 100 * kissa->map->tile_size, 100 * kissa->map->tile_size);
// 	while (is_wall(kissa, px_x / kissa->map->tile_size, px_y / kissa->map->tile_size) == 0
// 		&& !(px_x / kissa->map->tile_size < 0 || px_x / kissa->map->tile_size >= kissa->map->width
// 		|| px_y / kissa->map->tile_size < 0 || px_y / kissa->map->tile_size >= kissa->map->height))
// 	{
// 		mlx_put_pixel(kissa->view->ray, (int)px_x, (int)px_y, 0xFF0000FF);
// 		px_x += 1 * obj->dir->x;
// 		px_y += 1 * obj->dir->y;
// 	}
// 	mlx_image_to_window(kissa->mlx, kissa->view->ray, 0, 0);
// }

/*
	Returns 1 if coordinate ray_x and ray_y are within the minimap radius
	from player pos (6, 6), else returns 0.
*/
int	check_radius(t_cub3d *kissa, float ray_x, float ray_y)
{
	int center_x;
	int center_y;

	center_x = 6 * kissa->map->tile_size + kissa->map->tile_size / 2;
	center_y = 6 * kissa->map->tile_size + kissa->map->tile_size / 2;
	if (sqrt(pow((ray_x - center_x), 2) + pow((ray_y - center_y), 2)) <= MM_RADIUS * kissa->map->tile_size)
		return (1);
	return (0);
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

	NOTE: 
		"+ tile / 2" : this centers the ray to the player
		it seems to be working, the ray just goes over by a fiew pixels
		(dif between float and int), do we want to do smth about that?
		
*/
void	shoot_ray(t_cub3d *kissa, t_obj *obj)
{
	int tile = kissa->map->tile_size;
	int radius = MM_RADIUS * 2;
	int center_x = 6 * tile + tile / 2;
	int center_y = 6 * tile + tile / 2;
	float ray_x = center_x;
	float ray_y = center_y;
	float player_x = obj->x * tile;
	float player_y = obj->y * tile;

	kissa->view->ray = mlx_new_image(kissa->mlx, tile * 100, tile * 100);
	if (!kissa->view->ray)
		quit_perror(kissa, NULL, "MLX42 image creation failed");
	
	while (check_radius(kissa, ray_x, ray_y) && (ray_x - center_x) + radius / 2 * tile >= 0
		&& (ray_y - center_y) + radius / 2 * tile >= 0
		&& is_wall(kissa, player_x / tile, player_y / tile) == 0)
	{
		if (ray_x < 0 || ray_x >= kissa->map->width * tile || ray_y < 0 || ray_y >= kissa->map->height * tile)
				break;
		// printf("ray_x - center_x + radius = %f\n", ray_x - center_x + radius);
		// printf("ray_y - center_y + radius = %f\n", ray_y - center_y + radius);
		mlx_put_pixel(kissa->view->ray, (ray_x - center_x + radius * tile / 2) + tile / 2,
			(ray_y - center_y + radius * tile / 2) + tile / 2, 0xFF0000FF);
		ray_x += obj->dir->x;
		ray_y += obj->dir->y;
		player_x += obj->dir->x;
		player_y += obj->dir->y;

		// if (is_wall(kissa, player_x / kissa->map->tile_size, player_y / kissa->map->tile_size) == 1)
		// 	printf("\tIS A WALL ( %f, %f )\n", player_x, player_y);
		// else if ((ray_y - center_y) + radius / 2 * kissa->map->tile_size <= 0)
		// 	printf("\ty is 0\n");
		// else if ((ray_x - center_x) + radius / 2 * kissa->map->tile_size <= 0)
		// 	printf("\tx is 0\n");
		// else if (sqrt(pow((ray_x - center_x), 2) + pow((ray_y - center_y), 2))
		// 	>= (radius - 6) * kissa->map->tile_size)
		// 	printf("\tRadius is finished\n");
	}
	if (mlx_image_to_window(kissa->mlx, kissa->view->ray, 0, 0) < 0)
		quit_perror(kissa, NULL, "MLX42 image to window failed");
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
void	init_dda(t_cub3d *kissa, t_ray *ray, float rot)
{
	ray->x = kissa->player->x;
	ray->y = kissa->player->y;
	ray->dir->x = cos(rot);
	ray->dir->y = sin(rot);
	ray->line_len = 0;
	ray->step->x = check_dir(rot, 1);
	ray->step->y = check_dir(rot, 0);
	if (ray->dir->x == 0)
	{
		ray->step_size->x = 1;
		ray->step_size->y = HUGE_VAL;
	}
	else if (ray->dir->y == 0)
	{
		ray->step_size->y = 1;
		ray->step_size->x = HUGE_VAL;
	}
	else
	{
		ray->step_size->x = sqrt(1 + (ray->dir->y / ray->dir->x) * (ray->dir->y / ray->dir->x));
		ray->step_size->y = sqrt(1 + (ray->dir->x / ray->dir->y) * (ray->dir->x / ray->dir->y));
	}
}

/*
	Shoots a ray from the player object in the direction of the player object
	using the DDA algo. While the ray is not hitting a wall, the ray is drawn on the screen.

	This code works, is more optimized, bur is not as readable as function above
*/
void	dda_shoot_ray(t_cub3d *kissa, float rot, t_ray *ray)
{
	init_dda(kissa, ray, rot);
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
}
