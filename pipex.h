/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:22:18 by razamora          #+#    #+#             */
/*   Updated: 2024/07/29 01:18:27 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stddef.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

int		exec_command(char **full_cmd, char **env);
void	ft_check_command(char *cmd, char **envp);
char	*ft_find_path(char **envp);
void	ft_free_memory(char **tab);
void	ft_error(char *str, int code);
void	ft_print_error(char *title, int code, char *arg);
char	*ft_get_path(char **full_cmd, char **envp);
int		ft_error_file(int *file_pipe, char *file);
void	ft_print_error(char *title, int code, char *arg);
int		ft_command_error(char *cmd);

#endif