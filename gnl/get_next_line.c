/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 04:10:30 by slombard          #+#    #+#             */
/*   Updated: 2023/02/03 22:06:47 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char			*n_idx;
	char			*line;
	static char		*stash;

	stash = ft_read(fd, stash);
	if (!stash || fd < 0)
		return (NULL);
	n_idx = ft_n_idx(stash);
	if (!n_idx)
	{
		line = ft_substr(stash, 0, ft_strlen(stash));
		free(stash);
		stash = NULL;
		return (line);
	}
	line = ft_substr(stash, 0, n_idx - stash + 1);
	if (!line)
	{
		free (stash);
		stash = NULL;
		return (NULL);
	}
	stash = ft_memmove(stash, n_idx + 1, (ft_strlen(n_idx + 1) + 1));
	return (line);
}

char	*ft_n_idx(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			return (&s[i]);
		i++;
	}
	return (NULL);
}

char	*ft_write_new(char **ptr_stash, char *ptr_buff)
{
	char	*str;

	if (*ptr_stash == NULL)
	{
		*ptr_stash = malloc(1);
		if (!*ptr_stash)
			return (NULL);
		**ptr_stash = '\0';
	}
	str = ft_strjoin(*ptr_stash, ptr_buff);
	if (!str)
	{
		free(*ptr_stash);
		*ptr_stash = NULL;
		return (NULL);
	}
	free(*ptr_stash);
	*ptr_stash = NULL;
	return (str);
}

int	ft_read_check(char **stash_ptr, int buff_len)
{
	if (buff_len == -1)
	{
		free(*stash_ptr);
		*stash_ptr = NULL;
		return (-1);
	}
	if (buff_len == 0)
	{
		if (*stash_ptr && (*stash_ptr)[0] == '\0')
		{
			free(*stash_ptr);
			*stash_ptr = NULL;
			return (0);
		}
		return (0);
	}
	return (1);
}

char	*ft_read(int fd, char *stash)
{
	int		buff_len;
	int		read_check;
	char	*buffer;	

	buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
	while (1)
	{
		buff_len = read(fd, buffer, BUFFER_SIZE);
		read_check = ft_read_check(&stash, buff_len);
		if (read_check == -1)
			break ;
		if (read_check == 0)
			break ;
		buffer[buff_len] = '\0';
		stash = ft_write_new(&stash, buffer);
		if (!stash)
			return (NULL);
		if (ft_n_idx(stash) || buff_len < BUFFER_SIZE)
			break ;
	}
	free (buffer);
	buffer = NULL;
	return (stash);
}

/*
int	main(void)
{
	char	*next_line;
	int		fd;

	fd = open("file.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("ERROR: opening file\n");
		return (0);
	}
	printf("File open with file descriptor %d\n", fd);

	while ((next_line = get_next_line(fd)))
	{
		printf("%s", next_line);
		printf("MAIN STOP\n%s", next_line);
		free(next_line);
		next_line = NULL;
	}
	printf("\n");
	if (next_line)
	{
		free(next_line);
		next_line = NULL;
	}
	close(fd);
	printf("File %d closed\n\n", fd);
	return (0);
}
*/
