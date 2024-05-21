# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mminet <mminet@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/22 23:37:52 by mminet            #+#    #+#              #
#    Updated: 2024/05/21 01:53:37 by mminet           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_L =	libft/ft_atoi.c\
			libft/ft_bzero.c\
			libft/ft_calloc.c\
			libft/ft_isalnum.c\
			libft/ft_isalpha.c\
			libft/ft_isascii.c\
			libft/ft_isdigit.c\
			libft/ft_isprint.c\
			libft/ft_itoa.c\
			libft/ft_lstadd_back.c\
			libft/ft_lstadd_front.c\
			libft/ft_lstclear.c\
			libft/ft_lstdelone.c\
			libft/ft_lstiter.c\
			libft/ft_lstlast.c\
			libft/ft_lstmap.c\
			libft/ft_lstnew.c\
			libft/ft_lstsize.c\
			libft/ft_memccpy.c\
			libft/ft_memchr.c\
			libft/ft_memcmp.c\
			libft/ft_memcpy.c\
			libft/ft_memmove.c\
			libft/ft_memset.c\
			libft/ft_putchar_fd.c\
			libft/ft_putendl_fd.c\
			libft/ft_putnbr_fd.c\
			libft/ft_putstr_fd.c\
			libft/ft_split.c\
			libft/ft_strchr.c\
			libft/ft_strdup.c\
			libft/ft_striteri.c\
			libft/ft_strjoin.c\
			libft/ft_strlcat.c\
			libft/ft_strlcpy.c\
			libft/ft_strlen.c\
			libft/ft_strmapi.c\
			libft/ft_strncmp.c\
			libft/ft_strnstr.c\
			libft/ft_strrchr.c\
			libft/ft_strtrim.c\
			libft/ft_substr.c\
			libft/ft_tolower.c\
			libft/ft_toupper.c\

SRCS =	srcs/minishell.c\
		srcs/my_env.c\
		srcs/lexing.c\
		srcs/lexing_utils.c\
		srcs/parser.c\
		srcs/parser_utils.c\
		srcs/read_line.c\
		srcs/exec_line.c\
		srcs/exec_line_utils.c\
		srcs/exec_utils.c\
		srcs/build_in.c\
		srcs/redirect.c\
		srcs/build_in_utils.c\
		srcs/export.c\
		srcs/here_doc.c\
		srcs/here_doc_utils.c\

NAME = minishell

LIBFT = ./libft/libft.a

CC = gcc 

FLAGS = -Wall -Wextra -Werror -lreadline -ltermcap 

OBJ = $(patsubst srcs/%.c, obj/%.o, $(SRCS))

HEADER=	minishell.h

HEADER_LIB = libft/libft.h

all: $(NAME)

$(OBJ): ${HEADER} $(HEADER_LIB)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) $(FLAGS) -o $(NAME)

$(LIBFT): $(HEADER_LIB) $(SRCS_L)
	@echo "\n==> Making LIBFT"
	make -C ./libft

obj/%.o: srcs/%.c
	@mkdir -p obj
	$(CC) $(FLAGS) -c $< -o $@

norme:
	norminette *.c *.h

clean:
	rm -rf obj
	make -C ./libft clean

fclean: clean
	rm -rf $(NAME)
	make -C ./libft fclean

re: fclean all
