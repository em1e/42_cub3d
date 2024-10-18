/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/18 10:11:23 by vkettune         ###   ########.fr       */
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
	ray->step_dir->x = check_dir(ray->rot, 1);
	ray->step_dir->y = check_dir(ray->rot, 0);
	ray->side = -1;
	if (ray->rot == kissa->player->rot)
		printf("dir_x = %f, dir_y = %f, ray rot = %f, player rot = %f\n", ray->dir->x, ray->dir->y, ray->rot, kissa->player->rot);
	ray->step_len->x = sqrt(1 + (ray->dir->y / ray->dir->x) * (ray->dir->y / ray->dir->x));
	ray->step_len->y = sqrt(1 + (ray->dir->x / ray->dir->y) * (ray->dir->x / ray->dir->y));
	ray->ray_len->x = ray->step_len->x;
	ray->ray_len->y = ray->step_len->y;
	// }
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

void calculate_initial_step(t_cub3d *kissa, t_ray *ray)
{
	float	initial_y;
	float	initial_x;

	initial_y = kissa->player->y - floor(kissa->player->y);
	if (check_dir(ray->rot, 0) == - 1)
		initial_y = 1 - initial_y;
	initial_x = kissa->player->x - floor(kissa->player->x);
	if (check_dir(ray->rot, 1) == - 1)
		initial_x = 1 - initial_x;
	if (initial_y == 0)
	{
		ray->line_len = initial_x;
		ray->x += ray->step_dir->x * initial_x;
	}
	else if (initial_x == 0)
	{
		ray->line_len = initial_y;
		ray->y += ray->step_dir->y * initial_y;
	}
	else
	{
		ray->line_len = sqrt(initial_x * initial_x + initial_y * initial_y);
		if (initial_x * ray->step_len->x < initial_y * ray->step_len->y)
			ray->x += ray->step_dir->x * initial_x;
		else
			ray->y += ray->step_dir->y * initial_y;
	}
	if (ray->rot == kissa->player->rot)
		printf("line len before stuff: %f\n", ray->line_len);
}

/*
	Shoots a ray from the player object in the direction of the player object
	using the DDA algo. While the ray is not hitting a wall, the ray is drawn on the screen.

	This code works, is more optimized, bur is not as readable as function above
*/
void	cast_ray(t_cub3d *kissa, t_ray *ray)
{
	init_dda(kissa, ray);
	if (ray->step_dir->x == -10 || ray->step_dir->y == -10)
		quit_error(kissa, NULL, "math failed in init_ray");
	while (is_wall(kissa, ray->x, ray->y) == 0)
	{
		if (ray->ray_len->x < ray->ray_len->y)
		{
			ray->x += ray->step_dir->x;
			ray->line_len = ray->ray_len->x;
			ray->ray_len->x += ray->step_len->x;
			ray->side = 0;
			if (ray->rot == kissa->player->rot)
				printf("X: Line len %f, ray_x %f, ray_y %f, ray_len x %f, ray_len y %f\n", ray->line_len, ray->x, ray->y, ray->ray_len->x, ray->ray_len->y);
		}
		else
		{
			ray->y += ray->step_dir->y;
			ray->line_len = ray->ray_len->y;
			ray->ray_len->y += ray->step_len->y;
			ray->side = 1;
			if (ray->rot == kissa->player->rot)
				printf("Y: Line len %f, ray_x %f, ray_y %f, ray_len x %f, ray_len y %f\n", ray->line_len, ray->x, ray->y, ray->ray_len->x, ray->ray_len->y);
		}
	}
	ray->x = kissa->player->x + ray->dir->x * ray->line_len;
	ray->y = kissa->player->y + ray->dir->y * ray->line_len;
	set_wall_texture(kissa, ray);
}
