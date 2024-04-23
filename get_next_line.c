/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:19:01 by mminet            #+#    #+#             */
/*   Updated: 2024/04/23 01:38:41 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdel(char *str)
{
	if (str)
	{
		free(str);
		str = NULL;
	}
	return (NULL);
}


char	*get_line(char *left)
{
	char	*line;
	int	i = 0;
	char c;

	if (!left[0])
		return(NULL);
	while (left[i] && left[i] != '\n')
		i++;
	
	if (left[i])
	{
		c = left[i + 1];
		left[i + 1] = '\0';
		line = ft_strdup(left);
		left[i + 1] = c;
	}
	else
		line = ft_strdup(left);
	return (line);
}

static char	*get_left(int fd, char *left)
{
	char *buf;
	int ret;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	ret = 1;
	if (buf == NULL)
		return (NULL);
	while (!ft_strchr(left, '\n') && ret != 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == -1)
		{
			free(buf);
			return(NULL);
		}
		buf[ret] = '\0';
		left = ft_strjoin(left, buf);
	}
	free(buf);
	return(left);
}

char	*new_left(char *left)
{
	int i = 0;

	while(left[i] && left[i] != '\n')
		i++;
	if (!left[i])
		return (NULL);
	return (ft_strdup(left + i + 1));
}

char	*get_next_line(int fd)
{
	static char	*left = NULL;
	char		*line = NULL;

	if (left == NULL)
		left = ft_strdup("");
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	left = get_left(fd, left);
	line = get_line(left);
	left = new_left(left);
	if (!*line && left)
		free(left);
	return (line);
}
