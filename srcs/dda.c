/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/16 17:13:51 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	Sets the length of the ray to the next wall in the x or y direction

	flag = 0 checks for y rotation
	flag = 1 checks for x rotation
*/
// static void	set_ray_len(float *ray_len_q, float step_q, t_ray *ray, int flag)
// {
// 	if (step_q < 0)
// 		*ray_len_q = 0;
// 	else if (flag == 1)
// 		*ray_len_q = ray->step_size->x;
// 	else
// 		*ray_len_q = ray->step_size->y;
// }

/*
	Initializes the DDA algorithm by setting the starting coordinates and step sizes
*/
static void	init_dda(t_cub3d *kissa, t_ray *ray, float rot)
{
	ray->x = kissa->player->x;
	ray->y = kissa->player->y;
	ray->dir->x = cos(rot);
	ray->dir->y = sin(rot);
	ray->line_len = 0;
	ray->step->x = check_dir(rot, 1);
	ray->step->y = check_dir(rot, 0);
	ray->side = -1;
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

void calculate_initial_ray_lens(t_cub3d *kissa, t_ray *ray)
{
	ray->ray_len->y = kissa->player->y - floor(kissa->player->y);
	if (check_dir(ray->rot, 0) == - 1)
		ray->ray_len->y = 1 - ray->ray_len->y;
	ray->ray_len->x = kissa->player->x - floor(kissa->player->x);
	if (check_dir(ray->rot, 1) == - 1)
		ray->ray_len->x = 1 - ray->ray_len->x;
	if (ray->ray_len->y == 0 || ray->ray_len->y == 1)
		ray->ray_len->y = ray->step_size->y;
	if (ray->ray_len->x == 0 || ray->ray_len->x == 1)
		ray->ray_len->x = ray->step_size->x;
}

/*
	Shoots a ray from the player object in the direction of the player object
	using the DDA algo. While the ray is not hitting a wall, the ray is drawn on the screen.

	This code works, is more optimized, bur is not as readable as function above
*/
void	cast_ray(t_cub3d *kissa, float rot, t_ray *ray)
{
	init_dda(kissa, ray, rot);
	if (ray->step->x == -10 || ray->step->y == -10)
		quit_error(kissa, NULL, "math failed in init_ray");
	calculate_initial_ray_lens(kissa, ray);
	// set_ray_len(&ray->ray_len->x, ray->step->x, ray, 1);
	// set_ray_len(&ray->ray_len->y, ray->step->y, ray, 0);
	while (is_wall(kissa, ray->x, ray->y) == 0)
	{
		if (ray->ray_len->x < ray->ray_len->y)
		{
			ray->x += ray->step->x;
			ray->line_len = ray->ray_len->x;
			ray->ray_len->x += ray->step_size->x;
			ray->side = 0;
		}
		else
		{
			ray->y += ray->step->y;
			ray->line_len = ray->ray_len->y;
			ray->ray_len->y += ray->step_size->y;
			ray->side = 1;
		}
	}
	set_wall_texture(kissa, ray);
}
