/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:28:20 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/11 12:52:35 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include "libft.h"
# include "../libs/MLX42/include/MLX42/MLX42.h"
# include <math.h>

# define ROT_SPEED 0.1
# define MOVE_SPEED 0.2

# define RAYC 240

# define MMRAD 5

# define NORTH M_PI * 1.5
# define EAST 0
# define SOUTH M_PI * 0.5
# define WEST M_PI

typedef struct	s_vec
{
	float	x;
	float	y;
} t_vec;

typedef struct	s_view
{
	mlx_image_t	*ray;
	mlx_image_t	*mlx_no;
	mlx_image_t	*mlx_we;
	mlx_image_t	*mlx_so;
	mlx_image_t	*mlx_ea;
	mlx_image_t	*mlx_wall;
	mlx_image_t	*mlx_floor;
	mlx_image_t	*mlx_player;
	int			player_inst;
	int			**wall_inst;
	int			**floor_inst;
	float		*ray_array;
	t_vec		*scene;
} t_view;

typedef struct s_obj
{
	char	start_dir;
	float	x;
	float	y;
	t_vec	*dir;
	float	rot;
} t_obj;

typedef struct	s_map
{
	int		width;
	int		height;
	int		tile_size;
	char	**array;
	char	**visited;
	char	*file;
	char	*line;
	int		first_map_line;
} t_map;

// remove if dda func is not needed
typedef struct	s_ray
{
	float	x;
	float	y;
	float	rot;
	float	line_len;
	t_vec	*dir;
	t_vec	*ray_len;
	t_vec	*step;
	t_vec	*step_size;
}	t_ray;

typedef struct	s_cub3d
{
	mlx_t		*mlx;
	t_ray		*ray;
	t_map		*map;
	t_obj	*player;
	t_view		*view;
	int			fd;
	char		*no;
	char		*so;
	char		*we;
	char		*ea;
	char		*wall_tex;
	char		*floor_tex;
	char		*player_tex;
	int			f[3];
	int			c[3];
} t_cub3d;

// main.c
void	clean_kissa(t_cub3d *kissa);

// init.c
t_map	*new_map(t_cub3d *kissa);
t_view	*new_view(t_cub3d *kissa);
t_vec	*new_vec(t_cub3d *kissa);
t_ray	*new_ray(t_cub3d *kissa);
void	init_kissa(t_cub3d *kissa);
void	init_mlx(t_cub3d *kissa);

// parser.c
void	parse_kissa(t_cub3d *kissa);

// parser_utils.c
void	skip_space(char **str);
void	get_texture(t_cub3d *kissa, char **texture, char *line);
void	set_rgb(t_cub3d *kissa, int *rgb, char **rgb_arr, int rgb_i);
void	get_rgb(t_cub3d *kissa, int *rgb, char *line);

// hooks.c
void	escape_hook(void *param);
void	quit_hook(void *param);
void	move_keyhook(mlx_key_data_t keykissa, void *param);
void	update_hook(void *param);

// game.c
void	set_rot(t_obj *obj, char rot_char);
int	is_wall(t_cub3d *kissa, float x, float y);
void	move(t_cub3d *kissa, t_obj *obj, int dir_x, int dir_y);
void	rotate(t_cub3d *kissa, t_obj *obj, int dir);
void	play_game(t_cub3d *kissa);

// minimap.c
void	shoot_ray(t_cub3d *kissa, t_obj *obj);
void	refresh_minimap(t_cub3d *kissa);
void	setup_minimap(t_cub3d *kissa);

// dda.c
void	cast_ray(t_cub3d *kissa, float rot, t_ray *ray);

// draw_scene.c
void	draw_scene(t_cub3d *kissa);

// utils.c
void	clean_array(char **array);
void	close_fd(t_cub3d *kissa);
int		is_directory(char *filepath);
void	check_file(t_cub3d *kissa, char *file, char *ext);
void	print_map(t_cub3d *kissa);
void	print_floodfill(t_cub3d *kissa);
mlx_instance_t	*get_tile(t_view *view, int x, int y, char tile);

// img_convert.c
mlx_image_t	*convert_png(t_cub3d *kissa, char *file);

// map.c
void	init_map(t_cub3d *kissa);
void	init_mlx(t_cub3d *kissa);

// clean.c
void	clean_map(t_map *map);
void	clean_view(t_map *map, t_view *view);
void	clean_obj(t_obj *obj);
void	clean_kissa(t_cub3d *kissa);

// quit.c
void	quit_perror(t_cub3d *kissa, char *file, char *error_message);
void	quit_error(t_cub3d *kissa, char *file, char *error_message);
void	quit_success(t_cub3d *kissa, char *message);

#endif