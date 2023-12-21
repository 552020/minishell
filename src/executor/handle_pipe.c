/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsengeze <bsengeze@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 00:08:30 by bsengeze          #+#    #+#             */
/*   Updated: 2023/11/12 22:27:04 by bsengeze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_handle_pipe {
  int pipe_fd[2];
  int stdout_backup;
  int old_read_fd;
  int *old_read_fd_arr;
  pid_t l_pid;
  pid_t r_pid;
  int status_l;
  int status_r;
} t_handle_pipe;

int default_stdout = -1;

int handle_pipe(t_ast_node *node, t_data *data, int old_read_fd) {
  t_handle_pipe vars;
  static size_t i = 1;

  if (old_read_fd == -1) {
    vars.old_read_fd_arr = malloc(sizeof(int) * 1);
    if (!vars.old_read_fd_arr)
      free_exit(data, "Error: malloc failed\n");
  }
  if (default_stdout == -1)
    default_stdout = dup(STDOUT_FILENO);
  // ft_putstr_fd("handle_pipe\n", default_stdout);
  // ft_putstr_fd("old_read_fd: ", default_stdout);
  // ft_putnbr_fd(old_read_fd, default_stdout);
  // write(default_stdout, "\n", 1);
  vars.status_r = 0;
  if (pipe(vars.pipe_fd) == -1)
    free_exit(data, "Error: pipe failed\n");
  if (node->children[0]->type == N_PIPE) {
    if (old_read_fd != -1) {
      ft_realloc(vars.old_read_fd_arr, i, i + 1);
      i += 1;
    }
    vars.old_read_fd_arr[i - 1] = old_read_fd;
    node->children[0]->old_read_fd = vars.pipe_fd[0];
    node->children[0]->arr_size = i;
    node->children[0]->old_read_fd_arr = vars.old_read_fd_arr;
    // printf("node->children[0]->old_read_fd: %d\n",
    // 	node->children[0]->old_read_fd);
    // ft_putstr_fd("node->children[0]->old_read_fd: ", default_stdout);
    // ft_putnbr_fd(node->children[0]->old_read_fd, default_stdout);
    // write(default_stdout, "\n", 1);
  } else {
    node->children[0]->old_read_fd = old_read_fd;
    // printf("node->children[0]->old_read_fd: %d\n",
    // 	node->children[0]->old_read_fd);
    // ft_putstr_fd("node->children[0]->old_read_fd: ", default_stdout);
    // ft_putnbr_fd(node->children[0]->old_read_fd, default_stdout);
    // write(default_stdout, "\n", 1);
  }
  if (node->children[1]) {
    node->children[1]->old_read_fd = old_read_fd;
    // printf("node->children[1]->old_read_fd: %d\n",
    // 	node->children[1]->old_read_fd);
    // ft_putstr_fd("\nnode->children[1]->old_read_fd: ", default_stdout);
    // ft_putnbr_fd(node->children[1]->old_read_fd, default_stdout);
    // write(default_stdout, "\n", 1);
  }
  vars.stdout_backup = dup(STDOUT_FILENO);
  dup2(vars.pipe_fd[1], STDOUT_FILENO);
  close(vars.pipe_fd[1]);
  handle_l_child(node->children[0], data, &vars.l_pid, vars.pipe_fd[0]);
  dup2(vars.stdout_backup, STDOUT_FILENO);
  close(vars.stdout_backup);
  vars.status_r =
      handle_r_child(node->children[1], data, &vars.r_pid, vars.pipe_fd[0]);
  close(vars.pipe_fd[0]);
  return (vars.status_r);
}

void execute_child_left(t_ast_node *node, t_data *data, int pipe_fd) {
  // ft_putstr_fd("execute_child_left\n", default_stdout);
  close(pipe_fd);
  if (node->old_read_fd != -1) {
    while (node->old_read_fd_arr && node->arr_size > 0) {
      close(node->old_read_fd_arr[node->arr_size - 1]);
      node->arr_size -= 1;
    }
  }
  // ft_putstr_fd("pipe_fd is closed ", default_stdout);
  // ft_putnbr_fd(pipe_fd, default_stdout);
  // ft_putstr_fd("\nnode->old_read_fd: ", default_stdout);
  // ft_putnbr_fd(node->old_read_fd, default_stdout);
  // write(default_stdout, "\n", 1);
  if (node->old_read_fd != -1) {
    // ft_putstr_fd("node->old_read_fd is closed in left child\n",
    // 	default_stdout);
    close(node->old_read_fd);
  }
  if (command_is_builtin(node))
    node->exit_status = execute_builtin(node, data);
  else {
    if (handle_redirections(node, data))
      execute_cmd(node, data);
    else
      exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}

int handle_l_child(t_ast_node *node, t_data *data, pid_t *l_pid, int pipe_fd) {
  // ft_putstr_fd("handle_l_child\n\n", default_stdout);
  if (node->type == N_PIPE) {
    // ft_putstr_fd("node->type == N_PIPE\n", default_stdout);
    // ft_putstr_fd("node->old_read_fd: ", default_stdout);
    // ft_putnbr_fd(node->old_read_fd, default_stdout);
    // write(default_stdout, "\n", 1);
    execute(data, node, node->old_read_fd);
  } else if ((node->cmd != NULL) && (node->type == N_COMMAND)) {
    *l_pid = fork();
    if (*l_pid == -1)
      free_exit(data, "Error: fork failed\n");
    if (*l_pid != 0)
      node->pid = *l_pid;
    handle_signals_child(*l_pid);
    if (*l_pid == 0)
      execute_child_left(node, data, pipe_fd);
  }
  return (EXIT_SUCCESS);
}

void execute_child_right(t_ast_node *node, t_data *data, int pipe_fd) {
  // ft_putstr_fd("execute_child_right\n\n", default_stdout);
  dup2(pipe_fd, STDIN_FILENO);
  close(pipe_fd);
  if (node->old_read_fd != -1) {
    while (node->old_read_fd_arr && node->arr_size > 0) {
      close(node->old_read_fd_arr[node->arr_size - 1]);
      node->arr_size -= 1;
    }
  }
  // ft_putstr_fd("pipe_fd is closed ", default_stdout);
  // ft_putnbr_fd(pipe_fd, default_stdout);
  // ft_putstr_fd("\nnode->old_read_fd: ", default_stdout);
  // ft_putnbr_fd(node->old_read_fd, default_stdout);
  // write(default_stdout, "\n", 1);
  if (node->old_read_fd != -1) {
    // ft_putstr_fd("node->old_read_fd is closed*********\n",
    // default_stdout);
    close(node->old_read_fd);
  }
  if (command_is_builtin(node))
    node->exit_status = execute_builtin(node, data);
  else {
    if (handle_redirections(node, data))
      execute_cmd(node, data);
    else
      exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}

int handle_r_child(t_ast_node *node, t_data *data, pid_t *r_pid, int pipe_fd) {
  // ft_putstr_fd("handle_r_child\n\n", default_stdout);
  if (node->type == N_PIPE)
    execute(data, node, node->old_read_fd);
  else if ((node->cmd != NULL) && (node->type == N_COMMAND)) {
    *r_pid = fork();
    if (*r_pid == -1)
      free_exit(data, "Error: fork failed\n");
    if (*r_pid != 0)
      node->pid = *r_pid;
    handle_signals_child(*r_pid);
    if (*r_pid == 0)
      execute_child_right(node, data, pipe_fd);
  }
  return (EXIT_SUCCESS);
}
