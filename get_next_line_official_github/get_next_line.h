/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 22:07:39 by slombard          #+#    #+#             */
/*   Updated: 2023/02/03 22:15:22 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42 
# endif

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stddef.h>
# include <ctype.h>
# include <fcntl.h>

char			*get_next_line(int fd);
char			*ft_n_idx(char *ptr_buff_idx);
char			*ft_read(int fd, char *stash);
char			*ft_write_new(char **ptr_stash, char *ptr_buff_idx);
char			*ft_strjoin(char const *s1, char const *s2);
int				ft_check(char **ptr_stash, int buff_len);
char			*ft_substr(char const *s, unsigned int start, size_t len);
void			*ft_memmove(void *dest, const void *src, size_t n);
size_t			ft_strlen(const char *s);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);

#endif
