/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 20:28:20 by vkettune          #+#    #+#             */
/*   Updated: 2024/10/30 12:34:10 by jajuntti         ###   ########.fr       */
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

// bonus
# ifndef BONUS
#  define BONUS 0
# endif

// mlx window size
# define MLX_WIDTH 1200
# define MLX_HEIGHT 800

// FPS
# define FPS 90

// FOV & distance to projection pane
# define FOV 1.5

// wall & object sizes
# define WALL_HEIGHT MLX_HEIGHT
# define CAT_SIZE_FACTOR 0.25
# define CAT_TEX_SIZE 48

// number of rays to be cast
# define RAYC 240

// radius of collision bumper
# define BUMPER_SIZE 0.1

// speeds for moving and rotating
# define ROT_SPEED 2 
# define PLAYER_SPEED 5
# define CAT_SPEED 3

// minimap radius
# define MMRAD 5

// depth values for rendering images
# define Z_HIDDEN 0
# define Z_BACKGROUND 1
# define Z_SCENE 2
# define Z_MINIMAP 3
# define Z_START 5

# define START_SCREEN "./textures/start_screen.png"
# define VICTORY_SCREEN "./textures/victory_screen.png"
# define DEAD_SCREEN "./textures/dead_screen.png"
# define WALL_TEXTURE "./textures/21black.png"
# define FLOOR_TEXTURE "./textures/21white.png"
# define PLAYER_TEXTURE "./textures/21player.png"
# define MINI_CAT_TEXTURE "./textures/21cat.png"
# define SPRITE_CAT_TEXTURE "./textures/cat_sprite_1.png"

typedef struct s_vec
{
	float	x;
	float	y;
}	t_vec;

typedef struct s_view
{
	mlx_image_t	*original_cat;
	mlx_image_t	*mlx_start;
	mlx_image_t	*mlx_victory;
	mlx_image_t	*mlx_dead;
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
}	t_view;

typedef struct s_map
{
	int		width;
	int		height;
	int		tile_size;
	char	**array;
	char	**visited;
	char	*file;
	char	*line;
	int		first_map_line;
}	t_map;

// remove if dda func is not needed
typedef struct s_ray
{
	int			index;
	int			side;
	float		x;
	float		y;
	float		rot;
	float		line_len;
	float		initial_step;
	float		scaled_height;
	float		rot_diff;
	float		fisheye;
	float		perp_dist;
	int			offset;
	t_vec		*dir;
	t_vec		*ray_len;
	t_vec		*step_dir;
	t_vec		*step_len;
	mlx_image_t	*wall_tex;
	t_vec		*screen_start;
	t_vec		*img_start;
}	t_ray;

typedef struct s_obj
{
	char	*name;
	char	start_dir;
	float	x;
	float	y;
	t_vec	*dir;
	float	rot;
	int		caught;
	int		type;
	int		cat_i;
	int		cat_j;
	float	speed;
	t_ray	*seen_by;
	float	view_dir;
	float	distance;
	float	size;
	float	scaled_size;
	int		screen_start_x;
	int		screen_start_y;
}	t_obj;

typedef struct s_cub3d
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
	double		time_adjust;
	int			fd;
	char		*no;
	char		*so;
	char		*we;
	char		*ea;
	float		north;
	float		east;
	float		south;
	float		west;
	int			f[3];
	int			c[3];
	int			total_cats;
}	t_cub3d;

// init.c
void			init_kissa(t_cub3d *kissa);
void			init_mlx(t_cub3d *kissa);

// mlx_utils.c
void			convert_textures(t_cub3d *kissa);
mlx_image_t		*convert_png(t_cub3d *kissa, char *file);
void			delete_views_mlx(t_cub3d *kissa, t_view *view);
int				init_minimap_tile(t_cub3d *kissa, int i, int j, char c);
void			draw_direction(t_cub3d *kissa);

// new.c
t_map			*new_map(t_cub3d *kissa);
t_view			*new_view(t_cub3d *kissa);
t_vec			*new_vec(t_cub3d *kissa);
t_ray			*new_ray(t_cub3d *kissa);
t_obj			*new_obj(t_cub3d *kissa, float speed);

// parser.c
void			parse_kissa(t_cub3d *kissa);

// parser_utils.c
void			skip_space(char **str);
void			get_texture(t_cub3d *kissa, char **texture, char *line);
void			set_rgb(t_cub3d *kissa, int *rgb, char **rgb_arr, int rgb_i);
void			get_rgb(t_cub3d *kissa, int *rgb, char *line);

// hooks.c
void			quit_hook(void *param);
void			update_hook(void *param);
void			anim_update_hook(void *param);

// hooks_input.c
void			escape_hook(void *param);
void			move_keyhook(mlx_key_data_t keykissa, void *param);
void			mouse_hook(double xpos, double ypos, void *param);

// game.c
int				is_wall(t_cub3d *kissa, float x, float y);
int				move(t_cub3d *kissa, t_obj *obj, int dir_x, int dir_y);
void			rotate(t_cub3d *kissa, t_obj *obj, int rot, float amount);
void			play_game(t_cub3d *kissa);

// cats.c
void			move_cats(t_cub3d *kissa);
void			catch_cats(t_cub3d *kissa);
void			place_cats(t_cub3d *kissa, int i, int y, int x);

// draw_cat.c
void			animate_cat(t_cub3d *kissa);
void			draw_cat(t_cub3d *kissa, t_obj *cat, t_ray *ray);

// minimap.c
void			refresh_minimap(t_cub3d *kissa);
void			setup_minimap(t_cub3d *kissa, int i, int j);

// dda.c
void			cast_ray(t_cub3d *kissa, t_ray *ray);

// draw.c
void			draw_scene(t_cub3d *kissa);
void			draw_game_state(t_cub3d *kissa, mlx_image_t *img);

// map.c
void			init_map(t_cub3d *kissa);
void			init_mlx(t_cub3d *kissa);

// map_utils.c
void			init_cat_pos(t_cub3d *kissa, int cat, int x, int y);
void			init_player_pos(t_cub3d *kissa, char this, int i, int j);
void			replace_spaces(t_cub3d *kissa);

// clean.c
void			clean_map(t_map *map);
void			clean_view(t_cub3d *kissa, t_view *view);
void			clean_obj(t_obj *obj);
void			clean_kissa(t_cub3d *kissa);

// clean_utils.c
void			clean_file_content(t_cub3d *kissa);
void			clean_ray_array(t_cub3d *kissa, t_ray **ray_array);
void			clean_cats(t_cub3d *kissa, t_obj **cats);

// quit.c
void			quit_perror(t_cub3d *kissa, char *file, char *error_message);
void			quit_error(t_cub3d *kissa, char *file, char *error_message);
void			quit_success(t_cub3d *kissa, char *message);

// utils.c
void			close_fd(t_cub3d *kissa);
int				is_directory(char *filepath);
void			check_file(t_cub3d *kissa, char *file, char *ext);
void			print_map(t_cub3d *kissa);
mlx_instance_t	*get_tile(t_view *view, int x, int y, char tile);

// utils2.c
int32_t			rgb_to_pixel(int *rgb);
float			calc_distance(float x1, float y1, float x2, float y2);
float			fix_rot(float rot);
void			clean_array(char **array);
int				check_dir(t_cub3d *kissa, float rot, int flag);

#endif