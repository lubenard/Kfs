# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/06 17:01:46 by lubenard          #+#    #+#              #
#    Updated: 2021/02/09 19:39:51 by lubenard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test_libft.a
EXEC_NAME = testing_lib

LIB_SRC_FILE = lib/strlib.c \
               lib/strarraylib.c \
               lib/printk/printk.c \
               lib/printk/parse.c \
               lib/printk/minwidth.c \
               lib/printk/fill_buffer.c \
               lib/printk/flags/flag_c.c \
               lib/printk/flags/flag_di.c \
               lib/printk/flags/flag_s.c \
               lib/printk/flags/flag_u.c \
               lib/printk/flags/flag_o.c \
               lib/printk/flags/flag_x.c \
               lib/printk/flags/flag_p.c

SRC_FILES = bzero.c \
	strlen.c \
	tabchr.c \
	printf.c \

SRCSDIR = srcs
LIBSRCSDIR = ../../srcs

SRCS = $(addprefix $(SRCSDIR)/, $(SRC_FILES))
LIBSRCS = $(addprefix $(LIBSRCSDIR)/, $(LIB_SRC_FILE))

OBJ = $(SRCS:.c=.o)
LIBOBJS = $(LIBSRCS:.c=.o)

CC = clang

CFLAGS = -g3 -DLAUNCH_TESTS=1

all:  $(NAME)
	$(CC) $(CFLAGS) main.c $(NAME) -o $(EXEC_NAME)

$(NAME): $(LIBOBJS) $(OBJ)
	@printf "\033[33mCompilation de $(NAME)...\033[0m"
	@ar rcs $(NAME) $(OBJ)
	@printf "\033[32m[✓]\033[0m\n"

%.o : %.c
	@printf "\033[36mCompilation de $<...\033[0m\033[32m[✓]\033[0m\n"
	@$(CC) -c $(CFLAGS) -I includes $< -o $@

clean:
	@printf "\033[31mSuppression des objets de $(NAME)..\033[0m"
	@rm -f $(OBJ)
	@printf "\033[32m[✓]\033[0m\n"

fclean: clean
	@printf "\033[31mSuppression de $(NAME)...\033[0m"
	@rm -f $(NAME)
	@printf "\033[32m[✓]\033[0m\n"

re: fclean all

flcean: fclean
