/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 04:40:55 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/03 05:08:43 by vkettune         ###   ########.fr       */
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

	texture = mlx_load_png(file);
	if (!texture)
		quit_perror(kissa, file, "MLX42 failed");
	img = mlx_texture_to_image(kissa->mlx, texture);
	if (!img)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_delete_texture(texture);
	return (img);
}

/*
Converts a .xpm file into a texture and then converts that texture into an 
MLX image.
*/
mlx_image_t	*convert_xpm(t_cub3d *kissa, char *file)
{
	xpm_t	*texture;
	mlx_image_t		*img;

	img = NULL;
	texture = mlx_load_xpm42(file);
	if (!texture)
		quit_perror(kissa, file, "MLX42 failed");
	// this doesn't compile ----------------	
	// img = mlx_texture_to_image(kissa->mlx, texture);
	if (!img)
		quit_perror(kissa, NULL, "MLX42 failed");
	mlx_delete_xpm42(texture);
	return (img);
}