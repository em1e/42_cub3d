/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:49:02 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/25 16:43:44 by jajuntti         ###   ########.fr       */
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
	if (flag == 0)
	{
		if (rot > (float)WEST)
			return (-1);
		else 
			return (1);
	}
	else if (flag == 1)
	{
		if (rot > (float)NORTH && rot < (float)SOUTH)
			return (-1);
		else 
			return (1);
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
	ray->step_len->x = fabs(1/ray->dir->x);
	ray->step_len->y = fabs(1/ray->dir->y);
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
			ray->wall_tex = kissa->view->mlx_we;
		else
			ray->wall_tex = kissa->view->mlx_ea;
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
	ray->scaled_height = floor(kissa->wall_height / (ray->line_len * ray->fishey_adjust));
	ray->scale_factor = WALL_HEIGHT / ray->scaled_height;
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
		ray->img_start->y = ray->offset / 2;
	}
	if (ray->side)
		ray->img_start->x = floor(ray->scaled_height * (ray->x - floor(ray->x)));
	else
		ray->img_start->x = floor(ray->scaled_height * (ray->y - floor(ray->y)));
}

void	check_for_cats(t_cub3d *kissa, t_ray *ray)
{
	int		i;
	float	x;
	float	y;
	t_obj	*cat;

	i = 0;
	x = floor(ray->x);
	y = floor(ray->y);
	while (i < kissa->total_cats)
	{
		cat = kissa->cats[i];
		if (!cat->seen_by && cat->x >= x && cat->x < x + 1 && cat->y >= y 
			&& cat->y < y + 1)
		{
			cat->seen_by = ray;
			cat->distance = calc_distance(cat->x, cat->y, kissa->player->x, kissa->player->y);
			cat->scaled_size = cat->size / cat->distance;
			cat->screen_start_x = ray->screen_start->x + (cat->x - floor(cat->x) / cat->distance); // needs to be fixed to use x/y depending on view direction
			cat->view_dir = fix_rot(kissa->player->rot - cat->rot);
		}
		i++;
	}
}

/*
	Shoots a ray from the player object in the direction of the player object
	using the DDA algo. While the ray is not hitting a wall, the ray is drawn on the screen.

	This code works, is more optimized, bur is not as readable as function above
*/
void	cast_ray(t_cub3d *kissa, t_ray *ray)
{
	init_dda(kissa, ray);
	while (is_wall(kissa, ray->x, ray->y) == 0)
	{
		if (ray->ray_len->x < ray->ray_len->y)
		{
			ray->x += ray->step_dir->x;
			ray->line_len = ray->ray_len->x;
			ray->ray_len->x += ray->step_len->x;
			ray->side = 0;
		}
		else
		{
			ray->y += ray->step_dir->y;
			ray->line_len = ray->ray_len->y;
			ray->ray_len->y += ray->step_len->y;
			ray->side = 1;
		}
		check_for_cats(kissa, ray);
	}
	ray->x = kissa->player->x + ray->dir->x * ray->line_len;
	ray->y = kissa->player->y + ray->dir->y * ray->line_len;
	set_wall_texture(kissa, ray);
	calculate_values(kissa, ray);
}
