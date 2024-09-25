/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:26:56 by vkettune          #+#    #+#             */
/*   Updated: 2024/09/25 23:04:36 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* .cub maps have many rules and edge cases, here's the basic info on it:

These can be in any order, separated by x amount of spaces, etc.

NO = north facing wall texture path
SO = south facing wall texture path
WE = west facing wall texture path
EA = east facing wall texture path

F = floor color RBG code
C = ceiling color RBG code

map's has the be last in the file.

1 = walls
0 = space
N, S, E, W = player for that map, and in what direction
is the player looking at in the beginning

map checks for:
	- space or player on the map edge
	- multiple players (combination of the 4 letters)
	- no space or player in map
	- etc.
*/
int	main_parser(t_cub3d *kissa, char **argv)
{
	(void)kissa;
	(void)argv;
	printf("--- IN MAIN PARSER ---\n");

	return (0);
}
