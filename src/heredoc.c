#include "minishell.h"

void	collect_heredoc_content(t_token *token_arr, size_t token_count)
{
	size_t i = 0;
	char *delimiter;
	char *line;
	char *heredoc_content_str;
	char *rl_prompt;
	char *tmp;

	// printf("Collecting heredoc content...\n");
	while (i < token_count)
	{
		if (token_arr[i].type == T_HEREDOC)
		{
			delimiter = token_arr[i + 1].str;
			heredoc_content_str = ft_strdup("");
			line = NULL;
			rl_prompt = "heredoc> ";
			while ((line = readline(rl_prompt)) && ft_strncmp(line, delimiter,
					ft_strlen(delimiter)))
			{
				if (heredoc_content_str)
				{
					tmp = heredoc_content_str;
					heredoc_content_str = ft_strjoin(heredoc_content_str, "\n");
					free(tmp);
					heredoc_content_str = ft_strjoin(heredoc_content_str, line);
					free(tmp);
				}
				else
					heredoc_content_str = ft_strdup(line);
			}
			free(line);
			// We transform the delimiter token into a heredoc content token
			free(token_arr[i + 1].str);
			token_arr[i + 1].type = T_HEREDOC_CONTENT;
			token_arr[i + 1].str = heredoc_content_str;
		}
		i++;
	}
	// printf("Done collecting heredoc content.\n");
}