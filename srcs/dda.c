/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/22 15:20:12 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	flag = 0 checks for y rotation
	flag = 1 checks for x rotation
	rot is kissa->player->rot
	this function checks if the current rotation going in a
	positive or negative direction, in either the x or y axis
*/
static int	check_dir(float rot, int flag)
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
	Initializes the DDA algorithm by setting the starting coordinates and step sizes
*/
static void	init_dda(t_cub3d *kissa, t_ray *ray)
{
	ray->x = kissa->player->x;
	ray->y = kissa->player->y;
	ray->dir->x = cos(ray->rot);
	ray->dir->y = sin(ray->rot);
	ray->line_len = 0;
	ray->step_len->x = fabs(1/ray->dir->x); // far faster way to calculate lens
	ray->step_len->y = fabs(1/ray->dir->y); // far faster way to calculate lens
	// ray->step_len->x = sqrt(1 + (ray->dir->y / ray->dir->x) * (ray->dir->y / ray->dir->x));
	// ray->step_len->y = sqrt(1 + (ray->dir->x / ray->dir->y) * (ray->dir->x / ray->dir->y));
	ray->side = -1;

	ray->step_dir->x = check_dir(ray->rot, 1);
	ray->step_dir->y = check_dir(ray->rot, 0);
	if (ray->step_dir->x == -10 || ray->step_dir->y == -10)
		quit_error(kissa, NULL, "math failed in init_ray");

	if (ray->rot > (float)WEST)
		ray->ray_len->y = (ray->y - floor(ray->y)) * ray->step_len->y;
	else
		ray->ray_len->y = (floor(ray->y) + 1 - ray->y) * ray->step_len->y;
	if (ray->rot > (float)NORTH && ray->rot < (float)SOUTH)
		ray->ray_len->x = (ray->x - floor(ray->x)) * ray->step_len->x;
	else
		ray->ray_len->x = (floor(ray->x) + 1 - ray->x) * ray->step_len->x;
}

void	set_wall_texture(t_cub3d *kissa, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->dir->x > 0)
			ray->wall_tex = kissa->view->mlx_ea;
		else
			ray->wall_tex = kissa->view->mlx_we;
	}
	else
	{
		if (ray->dir->y > 0)
			ray->wall_tex = kissa->view->mlx_no;
		else
			ray->wall_tex = kissa->view->mlx_so;
	}
}

void	calculate_values(t_cub3d *kissa, t_ray *ray)
{
	float	fishey_adjust;

	fishey_adjust = cos(fabs(kissa->player->rot - ray->rot));
	ray->scaled_height = floor(kissa->wall_height / (ray->line_len * fishey_adjust));
	ray->screen_start->x = ray->index * MLX_WIDTH / RAYC;
	if (ray->scaled_height < MLX_HEIGHT)
	{
		ray->offset = 0;
		ray->screen_start->y = MLX_HEIGHT / 2 - ray->scaled_height / 2;
		ray->img_start->y = 0;
	}
	else
	{
		ray->offset = ray->scaled_height - MLX_HEIGHT;
		ray->screen_start->y = 0;
		ray->img_start->y = ray->offset / 2; //(int)floor((ray->scaled_height - MLX_HEIGHT) / 2);
	}
	if (ray->side)
		ray->img_start->x = floor(ray->scaled_height * (ray->x - floor(ray->x)));
	else
		ray->img_start->x = floor(ray->scaled_height * (ray->y - floor(ray->y)));
}

/*
	Shoots a ray from the player object in the direction of the player object
	using the DDA algo. While the ray is not hitting a wall, the ray is drawn on the screen.

	This code works, is more optimized, bur is not as readable as function above
*/
void	cast_ray(t_cub3d *kissa, t_ray *ray)
{
	init_dda(kissa, ray);
	
	// these do the same thing ---------------------------------------
	// set_ray_len(&ray->ray_len->x, ray->step_dir->x, ray, 1);
	// set_ray_len(&ray->ray_len->y, ray->step_dir->y, ray, 0);
	
	// if (ray->step_dir->x < 0)
	// 	ray->ray_len->x = 0;
	// else
	// 	ray->ray_len->x = ray->step_len->x;
	// if (ray->step_dir->y < 0)
	// 	ray->ray_len->y = 0;
	// else
	// 	ray->ray_len->y = ray->step_len->y;
	// ---------------------------------------------------------------
	
	while (is_wall(kissa, ray->x, ray->y) == 0)
	{
		if (ray->ray_len->x < ray->ray_len->y)
		{
			ray->x += ray->step_dir->x;
			ray->line_len = ray->ray_len->x;
			// if (ray->rot == kissa->player->rot)
			// 	printf("BB ray->ray_len->x = %f\n", ray->ray_len->x);
			ray->ray_len->x += ray->step_len->x;
			ray->side = 0;
			// if (ray->rot == kissa->player->rot)
			// 	printf("X: Line len %f, ray_x %f, ray_y %f, ray_len x %f, ray_len y %f\n", ray->line_len, ray->x, ray->y, ray->ray_len->x, ray->ray_len->y);
		}
		else
		{
			ray->y += ray->step_dir->y;
			ray->line_len = ray->ray_len->y;
			// if (ray->rot == kissa->player->rot)
			// 	printf("CC ray->ray_len->y = %f\n", ray->ray_len->y);
			ray->ray_len->y += ray->step_len->y;
			ray->side = 1;
			// if (ray->rot == kissa->player->rot)
			// 	printf("Y: Line len %f, ray_x %f, ray_y %f, ray_len x %f, ray_len y %f\n", ray->line_len, ray->x, ray->y, ray->ray_len->x, ray->ray_len->y);
		}
		// if (ray->rot == kissa->player->rot)
		// 	printf("LINE_LEN = %f, ray->x = %f, ray->y = %f\n--\n", ray->line_len, ray->x, ray->y);
	}
	
	// this should not be needed -------------------------------------
	// if (ray->rot == kissa->player->rot)
	// 	printf("OLD POS (x %f, y %f)\n", ray->x, ray->y);
	ray->x = kissa->player->x + ray->dir->x * ray->line_len;
	ray->y = kissa->player->y + ray->dir->y * ray->line_len;
	// if (ray->rot == kissa->player->rot)
	// {
	// 	printf("NEW POS (x %f, y %f)\n", ray->x, ray->y);
	// 	printf("PLAYER POS (x %f, y %f)\n", kissa->player->x, kissa->player->y);
	// }
	// ---------------------------------------------------------------
	
	set_wall_texture(kissa, ray);
	calculate_values(kissa, ray);
}
