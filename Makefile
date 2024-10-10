# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/25 19:49:37 by vkettune          #+#    #+#              #
#    Updated: 2024/10/10 14:34:21 by vkettune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# remove the colors we aren't using
# change colors and emojis to be cat themed

NAME = cub3D
FLAGS = -Wall -Wextra -Werror $(HEADERS) -g -fsanitize=address 
HEADERS = -I ./incs/ -I ./libs/MLX42/include/MLX42

LIBFT = ./libs/libft/libft.a
MLX42 = ./libs/MLX42/build/libmlx42.a
MLX42_DIR = ./libs/MLX42/

LIBS = $(LIBFT) $(MLX42) -I include -lglfw \
		-L"/Users/$(USER)/.brew/opt/glfw/lib/"
#-L"/home/linuxbrew/.linuxbrew/Cellar/glfw/3.4/lib/" -lm

FILES = main.c \
		quit.c \
		utils.c \
		img_convert.c \
		map.c \
		parser.c \
		parser_utils.c \
		init.c \
		hooks.c \
		game.c \
		draw.c \
		draw_scene.c \
		clean.c \
		minimap.c

#_FILES = put_images.c

# RAY_FILES = ray_tracing.c

# IMG = $(addprefix images/, $(IMAGES_FILES))
# RAY = $(addprefix ray/, $(RAY_FILES))

SOURCES = $(addprefix srcs/, $(FILES))
# $(addprefix srcs/, $(IMG))
# $(addprefix srcs/, $(RAY))

OBJ = $(addprefix objs/, $(FILES:.c=.o))
# IMG_OBJ = $(addprefix objs/, $(IMG:.c=.o))
# RAY_OBJ = $(addprefix objs/, $(RAY:.c=.o))

OBJECTS = $(OBJ) # $(IMG_OBJ) $(RAY_OBJ)

all: folders libft mlx42 $(NAME)
	@echo "$(GREEN)- - - - - - - - - - - - - - - - - - - - - - -$(X)"
	@echo "$(GREEN)Run the program with ./$(NAME)$(X)"

objs/%.o: srcs/%.c
	@cc $(FLAGS) -c $< -o $@ && echo "$(DARK_GRAY)Compiled: $@ $(X)"

$(NAME): $(OBJECTS)
	@cc $(OBJECTS) $(LIBS) $(FLAGS) -o $(NAME)
	@echo "$(DARK_MAGENTA)- - ✨✨✨✨ $(NAME) compiled! ✨✨✨✨ - -$(X)"

folders: # objs/images objs/ray
	@mkdir -p objs/
	@mkdir -p libs/libft/objs/
	@echo "$(DARK_MAGENTA)- - - - - 📁 Created all folders! 📁 - - - - -$(X)"

clone_mlx42:
	if [ ! -d "$(MLX42_DIR)" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git $(MLX42_DIR); \
	fi

mlx42: clone_mlx42
	@cmake $(MLX42_DIR) -B $(MLX42_DIR)build/ && make -C $(MLX42_DIR)build/
	@echo "$(DARK_MAGENTA)- - - - -✨📦 MLX42 compiled 📦✨- - - - - -$(X)"

libft:
	@echo "$(DARK_MAGENTA)- - - - - - 📦 Compiling libft 📦 - - - - - -$(X)"
	make -C ./libs/libft

clean:
	@clear
	@rm -rf objs/
	@rm -rf $(MLX42_DIR)build/
	@make -C ./libs/libft/ clean
	@echo "$(DARK_MAGENTA)- - - - -❗All object files cleaned❗- - - - -$(X)"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT)
	@echo "$(DARK_MAGENTA)- - - -❗All executable files cleaned❗- - - -$(X)"

re: fclean all

X = \033[0;39m
BLACK = \033[0;30m
DARK_RED = \033[0;31m
DARK_GREEN = \033[0;32m
DARK_YELLOW = \033[0;33m
DARK_BLUE = \033[0;34m
DARK_MAGENTA = \033[0;35m
DARK_CYAN = \033[0;36m
DARK_GRAY = \033[0;90m
LIGHT_GRAY = \033[0;37m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m
