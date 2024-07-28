/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:25:06 by razamora          #+#    #+#             */
/*   Updated: 2024/07/28 17:13:43 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PIPEX_BONUS_C
# define PIPEX_BONUS_C
#include "libft/libft.h"
#include "handler_exp.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void	exec_command(char *cmd_route, char **full_cmd);
void	ft_check_command(char *cmd, char **envp);
void	process_one(char **argv, char **envp, int *file_pipe);
int	    middle_process(char *cmd, char **envp, int file_pipe[2]);
void	 process_fin(char **argv, char **envp, int argc, int *file_pipe);
char	*ft_find_path(char **envp);
void	ft_free_memory(char **tab);
void	ft_error(char *str, int code);
int		open_file(char *file, int type);
//void	*ft_here_doc(char *str, int *file_pipe);
#endif