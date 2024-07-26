/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:18:50 by razamora          #+#    #+#             */
/*   Updated: 2024/07/26 18:08:35 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_error(char *str, int code)
{
	ft_putendl_fd(str, 2);
	exit(code);
}

void	exec_command(char *cmd, char *cmd_route, char *envp, char **full_cmd)
{
	int	flag;

	flag = 0;
	if (execve(cmd_route, full_cmd, NULL) < 0)
	{
		printf("command error");
		exit(1);
	}
}
void	ft_check_command(char *cmd, char **envp)
{
	int		i;
	char	**full_cmd;
	char	**path;

	i = 0;
	full_cmd = ft_split(cmd, ' ');
	cmd = ft_strjoin("/", full_cmd[0]);
	path = ft_find_path(envp);
	while (path[i] != NULL)
	{
		path[i] = ft_strjoin(path[i], cmd);
		if (access(path[i], F_OK | X_OK) == 0)
			exec_command(cmd, path[i], *envp, full_cmd);
		i++;
	}
	ft_putstr_fd("command not found---->", 2);
	ft_putstr_fd(cmd, 2);
	exit(0);
}

