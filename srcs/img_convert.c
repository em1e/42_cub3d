/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 04:40:55 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/03 06:40:54 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// THESE WORK THE SAME WAY, we just need to choose which one to use

/*
Converts a .png file into a texture and then converts that texture into an 
MLX image.
*/
mlx_image_t	*convert_png(t_cub3d *kissa, char *file)
{
	mlx_texture_t	*texture;
	mlx_image_t		*img;

	printf("\tfile = |%s|\n", file); // testing
	texture = mlx_load_png(file);
	if (!texture)
		quit_perror(kissa, file, "MLX42 failed A");
	img = mlx_texture_to_image(kissa->mlx, texture);
	if (!img)
		quit_perror(kissa, NULL, "MLX42 failed B");
	mlx_delete_texture(texture);
	return (img);
}
