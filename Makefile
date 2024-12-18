# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/25 19:49:37 by vkettune          #+#    #+#              #
#    Updated: 2024/10/30 10:11:17 by jajuntti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# remove the colors we aren't using
# change colors and emojis to be cat themed

NAME = cub3D
CC = cc
FLAGS = -Wall -Wextra -Werror $(HEADERS)
# -g -fsanitize=address
HEADERS = -I ./incs/ -I ./libs/MLX42/include/MLX42
BONUS = -DBONUS=1

LIBFT = ./libs/libft/libft.a
MLX42 = ./libs/MLX42/build/libmlx42.a
MLX42_DIR = ./libs/MLX42/

LIBS = -lm $(LIBFT) $(MLX42) -I include -lglfw \
		-L"/Users/$(USER)/.brew/opt/glfw/lib/"
#-L"/home/linuxbrew/.linuxbrew/Cellar/glfw/3.4/lib/" -lm

FILES = main.c \
		quit.c \
		utils.c \
		utils2.c \
		mlx_utils.c \
		map.c \
		map_utils.c \
		parser.c \
		parser_utils.c \
		init.c \
		new.c \
		hooks.c \
		hooks_input.c \
		game.c \
		dda.c \
		draw.c \
		clean.c \
		clean_utils.c \
		minimap.c \
		cats.c \
		cat_draw.c

SOURCES = $(addprefix srcs/, $(FILES))

OBJ = $(addprefix objs/, $(FILES:.c=.o))
BOBJ = $(addprefix bobjs/, $(FILES:.c=.o))

OBJECTS = $(OBJ)
BOBJECTS = $(BOBJ)
	
all: $(NAME)

objs/%.o: srcs/%.c
	@$(CC) $(FLAGS) -c $< -o $@ && echo "$(DARK_GRAY)Compiled: $@ $(X)"
bobjs/%.o: srcs/%.c
	@$(CC) $(FLAGS) $(BONUS) -c $< -o $@ && echo "$(DARK_GRAY)SUPER COMPILED: $@ $(X)"

$(NAME): folders libft mlx42 $(OBJECTS)
	@$(CC) $(OBJECTS) $(LIBS) $(FLAGS) -o $(NAME)
	@echo "$(DARK_MAGENTA)- - ✨✨✨✨ $(NAME) compiled! ✨✨✨✨ - -$(X)"
	@echo "$(GREEN)- - - - - - - - - - - - - - - - - - - - - - -$(X)"
	@echo "$(GREEN)Run the program with ./$(NAME)$(X)"
bonus: $(NAME) bfolders $(BOBJECTS)
	@$(CC) $(BOBJECTS) $(LIBS) $(FLAGS) $(BONUS) -o $(NAME)
	@echo "$(DARK_MAGENTA)- - ✨✨✨✨ $(NAME) SUPER COMPILED! ✨✨✨✨ - -$(X)"
	@echo "$(GREEN)- - - - - - - - - - - - - - - - - - - - - - -$(X)"
	@echo "$(GREEN)Run the program with ./$(NAME)$(X)"

folders:
	@mkdir -p objs/
	@mkdir -p libs/libft/objs/
	@echo "$(DARK_MAGENTA)- - - - - 📁 Created all folders! 📁 - - - - -$(X)"
bfolders:
	@mkdir -p bobjs/
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
	@rm -rf objs/ bobjs/
	@rm -rf $(MLX42_DIR)build/
	@make -C ./libs/libft/ clean
	@echo "$(DARK_MAGENTA)- - - - -❗All object files cleaned❗- - - - -$(X)"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT)
	@echo "$(DARK_MAGENTA)- - - -❗All executable files cleaned❗- - - -$(X)"

re: fclean all

.PHONY: all clean fclean re bonus

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
