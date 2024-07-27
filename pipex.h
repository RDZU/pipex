/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:22:18 by razamora          #+#    #+#             */
/*   Updated: 2024/07/27 11:36:28 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
#include "libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void	exec_command(char *cmd_route, char **full_cmd);
void	ft_check_command(char *cmd, char **envp);
void	process_one(char **argv, char **envp, int *file_pipe);
void	process_two(char **argv, char **envp, int *file_pipe, int argc);
char	**ft_find_path(char **envp);
void	ft_free_memory(char **tab);
void	ft_error(char *str, int code);
pid_t	ft_here_doc(char *str);

#endif