/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:28:20 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/24 10:11:40 by jajuntti         ###   ########.fr       */
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

// mlx window size
# define MLX_WIDTH 1200
# define MLX_HEIGHT 800

// FPS
# define FPS 30
# define DELAY 1 / FPS

// FOV & distance to projection pane
# define FOV M_PI / 2
# define PRO_DIST MLX_WIDTH / 2 / tan(FOV)

// wall & object sizes
# define WALL_HEIGHT MLX_HEIGHT
# define CAT_SIZE WALL_HEIGHT / 3
# define CAT_TEX_SIZE 48

// number of rays to be cast
# define RAYC 240
# define RAYDIFF M_PI / 2 / RAYC

// radius of collision bumper
# define BUMPER_SIZE 0.3

// speeds for moving and rotating
# define ROT_SPEED M_PI / 20
# define PLAYER_SPEED 0.2
# define CAT_SPEED 0.05

// minimap radius
# define MMRAD 5

// radians for directions 
# define NORTH M_PI * 0.5
# define EAST 0
# define SOUTH M_PI * 1.5
# define WEST M_PI

// depth values for rendering images
# define Z_HIDDEN 0
# define Z_BACKGROUND 1
# define Z_SCENE 2
# define Z_MINIMAP 3
# define Z_START 4

# define START_SCREEN "./textures/start_screen.png"
# define VICTORY_SCREEN "./textures/victory_screen.png"
# define WALL_TEXTURE "./textures/21black.png"
# define FLOOR_TEXTURE "./textures/21white.png"
# define PLAYER_TEXTURE "./textures/21player.png"
# define MINI_CAT_TEXTURE "./textures/21cat.png"
# define SPRITE_CAT_TEXTURE "./textures/cat_sprite_1.png"

typedef struct	s_vec
{
	float	x;
	float	y;
} t_vec;

typedef struct	s_view
{
	mlx_image_t	*original_cat;
	mlx_image_t	*mlx_start;
	mlx_image_t	*mlx_victory;
	mlx_image_t	*ray;
	mlx_image_t	*mlx_no;
	mlx_image_t	*mlx_we;
	mlx_image_t	*mlx_so;
	mlx_image_t	*mlx_ea;
	mlx_image_t	*mlx_wall;
	mlx_image_t	*mlx_floor;
	mlx_image_t	*mlx_player;
	mlx_image_t	*mlx_cat;
	mlx_image_t	*mlx_scene;
	int			**wall_inst;
	int			**floor_inst;
	int			**cat_inst;
	t_vec		*scene;
} t_view;

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
	int		index;
	int		side;
	float	x;
	float	y;
	float	rot;
	float	line_len;
	float	initial_step;
	float	scaled_height;
	int		offset;
	t_vec	*dir;
	t_vec	*ray_len;
	t_vec	*step_dir;
	t_vec	*step_len;
	mlx_image_t	*wall_tex;
	t_vec	*screen_start;
	t_vec	*img_start;
}	t_ray;

typedef struct s_obj
{
	char	start_dir;
	float	x;
	float	y;
	t_vec	*dir;
	float	rot;
	int		caught;
	int		cat_type;
	int		cat_i;
	int		cat_j;
	float	speed;
	t_ray	*seen_by;
	float	view_dir;
	float	distance;
	float	size;
	float	scaled_size;
} t_obj;

typedef struct	s_cub3d
{
	mlx_t		*mlx;
	t_ray		**ray_array;
	t_map		*map;
	t_obj		*player;
	t_obj		**cats;
	t_view		*view;
	bool		paused;
	int			column_width;
	int			cats_caught;
	int			wall_height;
	int			fd;
	char		*no;
	char		*so;
	char		*we;
	char		*ea;
	int			f[3];
	int			c[3];
	int			tile_count;
	int			total_cats;
} t_cub3d;

// main.c
void	clean_kissa(t_cub3d *kissa);

// init.c
t_map	*new_map(t_cub3d *kissa);
t_view	*new_view(t_cub3d *kissa);
t_vec	*new_vec(t_cub3d *kissa);
t_ray	*new_ray(t_cub3d *kissa);
t_obj	*init_obj(t_cub3d *kissa, float speed);
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
void	mouse_hook(double xpos, double ypos, void *param);
void	anim_update_hook(void *param);

// game.c
void	set_rot(t_obj *obj, char rot_char);
int		is_wall(t_cub3d *kissa, float x, float y);
int		move(t_cub3d *kissa, t_obj *obj, int dir_x, int dir_y);
void	draw_game_state(t_cub3d *kissa, mlx_image_t *img);
void	rotate(t_cub3d *kissa, t_obj *obj, int rot, float amount);
void	play_game(t_cub3d *kissa);

// sprite.c
void	animate_cat(t_cub3d *kissa);
void	move_cats(t_cub3d *kissa);
void	catch_cats(t_cub3d *kissa);
void	draw_cat(t_cub3d *kissa, t_obj *cat, t_ray *ray);
uint32_t	get_cats_pixel(t_cub3d *kissa, t_obj *cat, int x, int y, t_ray *ray);
void	init_cat_ani(t_cub3d *kissa);

// minimap.c
void	refresh_minimap(t_cub3d *kissa);
void	setup_minimap(t_cub3d *kissa, int i, int j);

// dda.c
void	cast_ray(t_cub3d *kissa, t_ray *ray);

// draw.c
void	draw_background(t_cub3d *kissa);
void	draw_scene(t_cub3d *kissa);

// utils.c
float	fix_rot(float rot);
void	clean_array(char **array);
void	close_fd(t_cub3d *kissa);
int		is_directory(char *filepath);
void	check_file(t_cub3d *kissa, char *file, char *ext);
void	print_map(t_cub3d *kissa);
void	print_floodfill(t_cub3d *kissa);
mlx_instance_t	*get_tile(t_view *view, int x, int y, char tile);
float	calc_distance(float x1, float y1, float x2, float y2);

// img_convert.c
mlx_image_t	*convert_png(t_cub3d *kissa, char *file);

// map.c
void	init_map(t_cub3d *kissa);
void	init_mlx(t_cub3d *kissa);

// clean.c
void	clean_map(t_map *map);
void	clean_view(t_cub3d *kissa, t_map *map, t_view *view);
void	clean_obj(t_obj *obj);
void	clean_kissa(t_cub3d *kissa);

// quit.c
void	quit_perror(t_cub3d *kissa, char *file, char *error_message);
void	quit_error(t_cub3d *kissa, char *file, char *error_message);
void	quit_success(t_cub3d *kissa, char *message);

#endif