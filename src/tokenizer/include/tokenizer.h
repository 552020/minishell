#ifndef TOKENIZER_H
#define TOKENIZER_H

#ifndef MINISHELL_H

#include "minishell.h"
#endif

void assign_parentheses_open(const char **str, t_data *data, size_t *idx);
void assign_parentheses_close(const char **str, t_data *data, size_t *idx);
void assign_log_and(const char **str, t_data *data, size_t *idx);
void assign_log_or(const char **str, t_data *data, size_t *idx);
void assign_redirect_in(const char **str, t_data *data, size_t *idx);

#endif
