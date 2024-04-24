/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:19:01 by mminet            #+#    #+#             */
/*   Updated: 2024/04/24 16:45:46 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_line(char *left)
{
	char	*line;
	char	c;
	int		i;

	i = 0;
	if (!left[0])
		return (NULL);
	while (left[i] && left[i] != '\n')
		i++;
	if (left[i])
	{
		c = left[i];
		left[i] = '\0';
		line = ft_strdup(left);
		left[i] = c;
	}
	else
		line = ft_strdup(left);
	return (line);
}

static char	*get_left(int fd, char *left)
{
	char	buf[BUFFER_SIZE + 1];
	char	*tmp;
	int		ret;

	ret = 1;
	while (!ft_strchr(left, '\n') && ret != 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == -1)
		{
			free(left);
			return (NULL);
		}
		tmp = left;
		buf[ret] = '\0';
		left = ft_strjoin(left, buf);
		free(tmp);
	}
	if (ft_strlen(left) == 0)
	{
		free(left);
		return (NULL);
	}
	return (left);
}

char	*new_left(char *left)
{
	int	i;

	i = 0;
	while (left[i] && left[i] != '\n')
		i++;
	if (!left[i] || !left[i + 1])
		return (NULL);
	left = ft_strdup(left + i + 1);
	return (left);
}

char	*get_next_line(int fd)
{
	static char	*left = NULL;
	char		*line;
	char		*tmp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (left == NULL)
		left = ft_strdup("");
	left = get_left(fd, left);
	if (left == NULL)
		return (NULL);
	line = get_line(left);
	tmp = left;
	left = new_left(left);
	free(tmp);
	return (line);
}
