#include "minishell.h"

char	*find_unpaired_quote(const char *str)
{
	const char	*current;
	char		*next_quote;

	if (!str)
		return (NULL);
	current = str;
	while ((current = ft_strchr(current, '\'')))
	{
		next_quote = ft_strchr(current + 1, '\'');
		if (!next_quote)
			return ((char *)current);
		else
			current = next_quote + 1;
	}
	return (NULL);
}

int	count_single_quotes(const char *str)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == '\'')
			count++;
		str++;
	}
	return (count);
}

char	*remove_single_quotes(char *str)
{
	char	*dest;
	char	*ret;
	char	*unpaired_quote;
	int		str_len;
	int		quotes_count;

	if (!str)
		return (NULL);
	quotes_count = count_single_quotes(str);
	str_len = ft_strlen(str) - quotes_count;
	dest = ft_calloc(str_len + 1, sizeof(char));
	if (!dest)
		return (NULL);
	ret = dest;
	unpaired_quote = find_unpaired_quote(str);
	while (*str)
	{
		if (*str != '\'' || (str == unpaired_quote))
			*dest++ = *str;
		str++;
	}
	return (ret);
}

char	*add_single_quotes(char *str)
{
	char	*result;
	char	*start;
	int		str_len;

	str_len = ft_strlen(str);
	result = ft_calloc(str_len + 3, sizeof(char));
	start = result;
	if (!result)
		return (NULL);
	*result++ = '\'';
	while (*str)
		*result++ = *str++;
	*result++ = '\'';
	return (start);
}

typedef struct reshuffle_quotes
{
	const char	*start_input;
	const char	*start_sub;
	char		*end_sub;
	char		*sub;
	char		*tmp;
	char		*before;
	char		*after;
	char		*result;
	char		*cur;
	int			before_and_sub_len;
}				t_reshuffle_quotes;

void	build_substring(t_reshuffle_quotes *rsq)
{
	rsq->start_sub = rsq->cur;
	rsq->end_sub = ft_strchr(rsq->cur + 1, *rsq->cur);
	while (rsq->start_sub > rsq->start_input && isregularchar(*(rsq->cur - 1),
			rsq->cur))
		rsq->start_sub--;
	while (*rsq->end_sub && (isregularchar(*rsq->end_sub, rsq->end_sub)
			|| *rsq->end_sub == '\''))
		rsq->end_sub++;
	rsq->tmp = ft_substr(rsq->start_sub, 0, rsq->end_sub - rsq->start_sub);
}

void	remove_add_single_quotes(t_reshuffle_quotes *rsq)
{
	rsq->sub = remove_single_quotes(rsq->tmp);
	free(rsq->tmp);
	rsq->tmp = add_single_quotes(rsq->sub);
	free(rsq->sub);
}

void	build_result(t_reshuffle_quotes *rsq)
{
	rsq->sub = rsq->tmp;
	rsq->before = ft_substr(rsq->result, 0, rsq->start_sub - rsq->start_input);
	rsq->after = ft_strdup(rsq->end_sub);
	free(rsq->result);
	rsq->result = ft_strjoin(rsq->before, rsq->sub);
	rsq->before_and_sub_len = ft_strlen(rsq->result);
	free(rsq->before);
	free(rsq->sub);
	rsq->tmp = ft_strjoin(rsq->result, rsq->after);
	free(rsq->result);
	free(rsq->after);
	rsq->result = rsq->tmp;
	rsq->cur = rsq->result + rsq->before_and_sub_len;
}

char	*reshuffle_single_quotes(const char *input)
{
	t_reshuffle_quotes	rsq;

	if (!input)
		return (NULL);
	rsq.result = ft_strdup(input);
	rsq.cur = rsq.result;
	rsq.start_input = rsq.result;
	while (*rsq.cur)
	{
		rsq.start_input = rsq.result;
		if (*rsq.cur == '\'')
		{
			if (isregularchar(*rsq.cur, rsq.cur))
				break ;
			build_substring(&rsq);
			remove_add_single_quotes(&rsq);
			build_result(&rsq);
		}
		rsq.cur++;
	}
	return (rsq.result);
}

char	*reshuffle_quotes(const char *input)
{
	const char *start_input;
	const char *start;
	char *end;
	char *result;

	result = ft_strdup(input);
	start_input = input;
	while (*input)
	{
		if (*input == '\'' || *input == '"')
		{
			// Problem: if there is a unpaired single quote but valid double quotes
			// we will exit the loop and return the input string
			if (isregularchar(*input, input))
			{
				break ;
			}

			start = input;
			end = ft_strchr(input + 1, *input);
			(void)end;
			while (start > start_input && isregularchar(*(input - 1), input))
				start--;

			if (ft_strchr(input + 1, *input))
			{
				input = ft_strchr(input + 1, *input);
				input++;
			}
			else
				return (ft_strdup(input));
		}
		else
			input++;
	}
	return (result);
}