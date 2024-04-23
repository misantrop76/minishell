/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 23:30:44 by mminet            #+#    #+#             */
/*   Updated: 2024/04/23 02:08:44 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_h

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft/libft.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif

typedef enum
{
					TOKEN_WORD,
					TOKEN_PIPE,
					TOKEN_STDOUT,
					TOKEN_STDOUT_A,
					TOKEN_READ,
					TOKEN_STDIN,
					TOKEN_ERROR
}TokenType;

char		*get_next_line(int fd);

typedef struct s_token
{
					TokenType		type;
					char		*value;
}														t_token;

#endif