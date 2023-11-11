#include "minishell.h"

int	ft_isvalidvarname(char c)
{
	// Check if the character is alphanumeric or an underscore
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 17:46:41 by slombard          #+#    #+#             */
/*   Updated: 2023/01/15 20:12:16 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_free_ret(char **ret, size_t i);

static size_t	ft_count_word(const char *s, char c)
{
	size_t	i;
	size_t	words;

	i = 0;
	words = 0;
	if (!s[0])
		return (0);
	while (s[i] && s[i] == c)
		i++;
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			words++;
			while (s[i] && s[i] == c)
				i++;
		}
		else
			i++;
	}
	if (s[i - 1] != c)
		words++;
	return (words);
}

char **ft_split_envp(const char *s, char c) 
{
    char **ret;
    size_t len;
    size_t i;

    i = 0;
    ret = (char **)malloc(sizeof(char *) * (ft_count_word(s, c) + 2)); // Allocate one more for the possible empty string at the end.
    if (!ret)
        return (0);
    while (*s) {
        if (*s != c) {
            len = 0;
            while (*s && *s != c && ++len)
                s++;
            ret[i] = ft_substr(s - len, 0, len);
            if (!ret[i] && ft_free_ret(ret, i))
                return (0);
            i++;
        } 
		else 
		{
            if (*(s + 1) == '\0')// Check if next character is the end of the string, indicating an empty value.
                ret[i++] = ft_strdup(""); // Allocate an empty string for the value.
            s++;
        }
    }
    ret[i] = 0;
    return (ret);
}

int	ft_free_ret_envp(char **ret, size_t i)
{
	while (i--)
		free(ret[i]);
	free(ret);
	return (1);
}
