/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:25:06 by razamora          #+#    #+#             */
/*   Updated: 2024/07/29 01:27:06 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include "libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

void	ft_check_command(char *cmd, char **envp);
void	process_one(char **argv, char **envp, int *file_pipe);
int		middle_process(char *cmd, char **envp, int file_pipe[2]);
void	process_fin(char **argv, char **envp, int argc, int *file_pipe);
char	*ft_find_path(char **envp);
void	ft_free_memory(char **tab);
void	ft_error(char *str, int code);
int		open_file(char *file, int type);
void	ft_check_command(char *cmd, char **envp);
int		ft_command_error(char *cmd);
int		ft_error_file(int *file_pipe, char *file);
void	ft_print_error(char *title, int code, char *arg);
char	*ft_get_path(char **full_cmd, char **envp);
int		ft_strcmp(char *s1, char *s2);
int		finaly_openfile(char **av, int argc);

#endif