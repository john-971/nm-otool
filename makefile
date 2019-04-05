#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jandreu <jandreu@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/09/06 17:45:06 by jandreu           #+#    #+#              #
#    Updated: 2015/09/06 17:49:41 by jandreu          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC=gcc
NMDIRSRCS=nm/src/

NAME_NM = ft_nm
NAME_OTOOL = ft_otool

PATH_SRC = ./src/
PATH_INC = ./include/

PATH_INC_LIBFT = ./libft/includes/
INCLUDES = -I $(PATH_INC) -I $(PATH_INC_LIBFT)
CFLAGS =  -Wall -Werror -Wextra $(INCLUDES)


LIBS = -L libft/ -lft -lm


SRC_NM =	./nm/src/archive.c \
			./nm/src/fat_32.c \
			./nm/src/mach_o_32.c \
			./nm/src/mach_o_64.c \
			./nm/src/main.c \
			./nm/src/manage_endian.c \
			./nm/src/output_32.c \
			./nm/src/output_64.c \
			./nm/src/tools.c

SRC_OTOOL = ./otool/src/archive.c \
			./otool/src/fat_32.c \
			./otool/src/mach_o_32.c \
			./otool/src/mach_o_64.c \
			./otool/src/main.c \
			./otool/src/manage_endian.c \
			./otool/src/tools.c


OBJ_NM = $(SRC_NM:.c=.o)
OBJ_OTOOL = $(SRC_OTOOL:.c=.o)

.PHONY: clean fclean re

all: $(NAME_NM) $(NAME_OTOOL)

$(NAME_NM): $(OBJ_NM)
	make -C libft/
	$(CC) $(OBJ_NM) -o $(NAME_NM) $(LIBS)
	@echo "\033[36m◉ \033[33mmake nm is done\033[0m"

$(NAME_OTOOL): $(OBJ_OTOOL)
	make -C libft/
	$(CC) $(OBJ_OTOOL) -o $(NAME_OTOOL) $(LIBS)
	@echo "\033[36m◉ \033[33mmake otool is done\033[0m"

clean:
	rm -f $(OBJ_NM)
	rm -f $(OBJ_OTOOL)
	make clean -C libft/
	@echo "\033[36m◉ \033[33mclean done\033[0m"

fclean: clean
	rm -f $(NAME_NM)
	rm -f $(NAME_OTOOL)
	make fclean -C libft/
	@echo "\033[36m◉ \033[33mfclean done\033[0m"


re: fclean all