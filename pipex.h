/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:22:18 by razamora          #+#    #+#             */
/*   Updated: 2024/07/28 18:07:38 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# include "handler_exp.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stddef.h>
# include <sys/types.h>
# include <errno.h>

int		exec_command(char **full_cmd, char **env);
void	ft_check_command(char *cmd, char **envp);
char	*ft_find_path(char **envp);
void	ft_free_memory(char **tab);
void	ft_error(char *str, int code);
pid_t	ft_here_doc(char *str);

#endif