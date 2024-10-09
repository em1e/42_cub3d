/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/09 16:48:07 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
Draws the image for the provided character at the given coordinate, which is 
calculated using provided indexes and the tile size used.
*/
void	draw_tile(t_cub3d *kissa, char c, int i, int j)
{
	int		x;
	int		y;

	x = j * kissa->map->tile_size;
	y = i * kissa->map->tile_size;
	// something like this should use --------------------------
	// if (c == '1')
	// {
	// 	mlx_put_pixel(kissa->view->mlx_wall, (int)x, (int)y, 0x000000);
	// 	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_wall, x, y) < 0)
	// 		quit_perror(kissa, NULL, "MLX42 failed");
	// }
	// else if (c == '0')
	// {
	// 	mlx_put_pixel(kissa->view->mlx_floor, (int)x, (int)y, 0xFFFFFF);
	// 	if (mlx_image_to_window(kissa->mlx, kissa->view->mlx_floor, x, y) < 0)
	// 		quit_perror(kissa, NULL, "MLX42 failed");
	// }
	//----------------------------------------------------------------

	// needed if array is upside down ----------------------
	// y = abs((i - kissa->map->height + 1) * kissa->map->tile_size);
	
	if (c == '1' \
		&& mlx_image_to_window(kissa->mlx, kissa->view->mlx_wall, x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
	else if (c == '0' \
		&& mlx_image_to_window(kissa->mlx, kissa->view->mlx_floor, x, y) < 0)
		quit_perror(kissa, NULL, "MLX42 failed");
}

/*
	flag = 0 checks for y rotation
	flag = 1 checks for x rotation
	rot is kissa->player->rot

	this function checks if the current rotation going in a
	positive or negative direction, in either the x or y axis
*/
int check_dir(float rot, int flag)
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

// THIS IS OLD CODE FROM YESTERDAY ---------------------------------
// void	shoot_ray(t_cub3d *kissa, t_obj *obj)
// {
// 	double	ray_x = obj->x;
// 	double	ray_y = obj->y;
// 	double	px_x = obj->x;
// 	double	px_y = obj->y;

// 	kissa->view->ray = mlx_new_image(kissa->mlx, 10 * kissa->map->tile_size, 10 * kissa->map->tile_size);
// 	while (kissa->map->array[(int)ray_y][(int)ray_x] != '1')
// 	{
// 		mlx_put_pixel(kissa->view->ray, (int)ray_x * kissa->map->tile_size, (int)ray_y * kissa->map->tile_size, 0xFF0000FF); // Draw the pixel
// 		printf("AAAAAAAAA ( %d, %d ) \n", (int)ray_x, (int)ray_y);
// 		ray_x += obj->dir->x * STEP_SIZE;
// 		ray_y += obj->dir->y * STEP_SIZE;
// 	}
// 	mlx_image_to_window(kissa->mlx, kissa->view->ray, 0, 0);
// }
// -------------------------------------------------------------------

void	shoot_ray(t_cub3d *kissa, t_obj *obj)
{
	double	px_x = obj->x;
	double	px_y = obj->y;

	kissa->view->ray = mlx_new_image(kissa->mlx, 100 * kissa->map->tile_size, 100 * kissa->map->tile_size);
	px_x *= kissa->map->tile_size;
	px_y *= kissa->map->tile_size;
	// printf("AAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	while (kissa->map->array[(int)px_y / kissa->map->tile_size][(int)px_x / kissa->map->tile_size] != '1'
		&& !(px_x / kissa->map->tile_size < 0 || px_x / kissa->map->tile_size >= kissa->map->width
		|| px_y / kissa->map->tile_size < 0 || px_y / kissa->map->tile_size >= kissa->map->height))
	{
		// printf("px_x = %d\n", (int)px_x / kissa->map->tile_size);
		// printf("px_y = %d\n", (int)px_y / kissa->map->tile_size);
		mlx_put_pixel(kissa->view->ray, (int)px_x, (int)px_y, 0xFF0000FF);
		px_x += 1 * obj->dir->x;
		px_y += 1 * obj->dir->y;
	}
	mlx_image_to_window(kissa->mlx, kissa->view->ray, 0, 0);
}


// this was working the same as code above, but too complex and long. Tried following the dda guide and their explanation
// void	shoot_ray(t_cub3d *kissa, t_obj *obj)
// {
// 	double	x = obj->x;
// 	double	y = obj->y;
	
// 	float step_x = check_dir(obj->rot, 1);
// 	float step_y = check_dir(obj->rot, 0);

// 	float	ray_len_x;
// 	float	ray_len_y;
	
// 	float	step_size_x = sqrt(1 + (obj->dir->y / obj->dir->x) * (obj->dir->y / obj->dir->x));
// 	float	step_size_y = sqrt(1 + (obj->dir->x / obj->dir->y) * (obj->dir->x / obj->dir->y));
	
// 	float	map_check_x = x;
// 	float	map_check_y = y;
// 	float	px_x = x;
// 	float	px_y = y;

// 	float	line_len;

// 	printf("\t---------------------\n");
// 	printf("\tNORTH = %f\n", NORTH);
// 	printf("\tEAST = %d\n", EAST);
// 	printf("\tSOUTH = %f\n", SOUTH);
// 	printf("\tWEST = %f\n", WEST);
// 	printf("\t---------------------\n");
	
// 	// error check for check_dir() ----------------------
// 	if (step_x == -10 || step_y == -10)
// 	{
// 		printf("something wrong\n");
// 		return ;
// 	}

// 	printf("rot = %f\n", obj->rot);
// 	kissa->view->ray = mlx_new_image(kissa->mlx, 10 * kissa->map->tile_size, 10 * kissa->map->tile_size);
	
// 	// get ray_len_x ------------------------------------
// 	printf("step_x = %f\n", step_x);
// 	if (step_x < 0)
// 		ray_len_x = (x - map_check_x) * step_size_x;
// 	else
// 		ray_len_x = ((map_check_x + 1) - x) * step_size_x;

// 	// get ray_len_y ------------------------------------
// 	printf("step_y = %f\n", step_y);
// 	if (step_y < 0)
// 		ray_len_y = (y - map_check_y) * step_size_y;
// 	else
// 		ray_len_y = ((map_check_y + 1) - y) * step_size_y;
	
// 	while (kissa->map->array[(int)map_check_y][(int)map_check_x] != '1')
// 	{
// 		// put this in a while loop that loops each pixel through tiles
		
// 		if (ray_len_x < ray_len_y)
// 		{
// 			printf("walking on x ( %f, %f)\n", map_check_x, map_check_y);
// 			// printf("map_check_x = %d\n", (int)map_check_x * kissa->map->tile_size);
// 			px_x = map_check_x;
// 			map_check_x += step_x;
// 			line_len = ray_len_x;
// 			ray_len_x += step_size_x;
// 		}
// 		else
// 		{
// 			printf("walking on y ( %f, %f)\n", map_check_x, map_check_y);
// 			px_y = map_check_y;
// 			map_check_y += step_y;
// 			line_len = ray_len_y;
// 			ray_len_y += step_size_y;
// 		}
		

// 		px_x *= kissa->map->tile_size;
// 		px_y *= kissa->map->tile_size;
// 		while ((px_x / kissa->map->tile_size != map_check_x || px_y / kissa->map->tile_size != map_check_y)
// 			&& !(px_x / kissa->map->tile_size < 0 || px_x / kissa->map->tile_size >= kissa->map->width
// 			|| px_y / kissa->map->tile_size < 0 || px_y / kissa->map->tile_size >= kissa->map->height))
// 		{
// 			printf("px_x = %d\n", (int)px_x);
// 			printf("px_y = %d\n", (int)px_y);
// 			mlx_put_pixel(kissa->view->ray, (int)px_x, (int)px_y, 0xFF0000FF);
// 			px_x += 1 * obj->dir->x;
// 			px_y += 1 * obj->dir->y;
// 		}
// 	}
// 	printf("line len = %f\n", line_len);
// 	mlx_image_to_window(kissa->mlx, kissa->view->ray, 0, 0);
// }
