/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaylik@student.42istanbul.com.tr          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 01:52:21 by mbaylik           #+#    #+#             */
/*   Updated: 2026/09/26 02:08:50 by mbaylik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_file(int fd, char *stash)
{
	char	*buffer;
	char	*temp;
	ssize_t	b;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free(stash), NULL);
	b = 1;
	while (!gnl_strchr(stash, '\n') && b > 0)
	{
		b = read(fd, buffer, BUFFER_SIZE);
		if (b < 0)
			return (free(buffer), free(stash), NULL);
		buffer[b] = '\0';
		if (b > 0)
		{
			temp = gnl_strjoin(stash, buffer);
			free(stash);
			stash = temp;
			if (!stash)
				return (free(buffer), NULL);
		}
	}
	free(buffer);
	return (stash);
}

static char	*get_line(char *stash)
{
	size_t	i;

	if (!stash || !stash[0])
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	return (gnl_substr(stash, 0, i));
}

static char	*get_rest(char *stash)
{
	char	*rest;
	size_t	i;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	i++;
	rest = gnl_substr(stash, i, gnl_strlen(stash) - i);
	free(stash);
	if (rest && !rest[0])
	{
		free(rest);
		return (NULL);
	}
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_file(fd, stash);
	if (!stash)
		return (NULL);
	line = get_line(stash);
	if (!line)
	{
		free(stash);
		return (NULL);
	}
	stash = get_rest(stash);
	return (line);
}
