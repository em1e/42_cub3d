/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cats.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:04:17 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/29 12:37:37 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	cat_is_near(t_cub3d *kissa, t_obj *cat)
{
	if (kissa->map->array[(int)cat->y][(int)cat->x] == 'P'
	|| kissa->map->array[(int)cat->y + 1][(int)cat->x] == 'P'
	|| kissa->map->array[(int)cat->y - 1][(int)cat->x] == 'P'
	|| kissa->map->array[(int)cat->y][(int)cat->x + 1] == 'P'
	|| kissa->map->array[(int)cat->y][(int)cat->x - 1] == 'P'
	|| kissa->map->array[(int)cat->y - 1][(int)cat->x - 1] == 'P'
	|| kissa->map->array[(int)cat->y + 1][(int)cat->x - 1] == 'P'
	|| kissa->map->array[(int)cat->y - 1][(int)cat->x + 1] == 'P'
	|| kissa->map->array[(int)cat->y + 1][(int)cat->x + 1] == 'P')
		return (1);
	return (0);
}

void	catch_cats(t_cub3d *kissa)
{
	int		i;
	t_obj	*cat;

	i = 0;
	while (i < kissa->total_cats)
	{
		cat = kissa->cats[i];
		if (cat->caught == 0 && cat_is_near(kissa, cat))
		{
			if (cat->view_dir <= M_PI * 0.25 || cat->view_dir > M_PI * 0.25 * 7)
			{
				printf("Oh no! You grabbed a cat from behind. GAME OVER!\n");
				kissa->paused = true;
				kissa->cats_caught++;
				draw_game_state(kissa, kissa->view->mlx_dead);
				return ;
			}
			cat->caught++;
			kissa->cats_caught++;
			kissa->map->array[(int)cat->y][(int)cat->x] = '0';
			printf("Caught cat %d. %d cats left.\n", i,
				kissa->total_cats - kissa->cats_caught);
		}
		i++;
	}
}

void	move_cats(t_cub3d *kissa)
{
	int		i;
	t_obj	*cat;

	i = 0;
	while (i < kissa->total_cats)
	{
		cat = kissa->cats[i];
		if (cat->caught <= 0)
		{
			kissa->map->array[(int)cat->y][(int)cat->x] = '0';
			if (move(kissa, cat, 0, 1) == 1)
				rotate(kissa, cat, 1, NORTH);
			kissa->map->array[(int)cat->y][(int)cat->x] = 'C';
		}
		i++;
	}
}

/*
	creates cat objects if conditions are met
*/
void	create_cat_objs(t_cub3d *kissa)
{
	int	y;
	int	x;
	int	i;

	y = 0;
	i = 0;
	kissa->cats = malloc(sizeof(t_obj *) * kissa->total_cats);
	if (!kissa->cats)
		quit_error(kissa, NULL, "memory allocation failure");
	while (y < kissa->map->height)
	{
		x = 0;
		while (x < kissa->map->width)
		{
			if (kissa->map->array[y][x] == 'C')
			{
				init_cat_pos(kissa, i, x, y);
				i++;
			}
			x++;
		}
		y++;
	}
	if (i != kissa->total_cats)
		quit_error(kissa, NULL, "cat count issue");
}

/*
	places down cats or "C" on the map, and creates as many cat objects
	into kissa->cats as needed
*/
void	place_cats(t_cub3d *kissa)
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
			if (!is_wall(kissa, x, y))
				i++;
			if (i >= distance && x != kissa->player->x && y != kissa->player->y)
			{
				i = 0;
				kissa->map->array[y][x] = 'C';
				kissa->total_cats++;
			}
			x++;
		}
		y++;
	}
	create_cat_objs(kissa);
}
