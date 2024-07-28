/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:18:50 by razamora          #+#    #+#             */
/*   Updated: 2024/07/28 17:08:47 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_error(char *str, int code)
{
	ft_putendl_fd(str, 2);
	exit(code);
}
static int	ft_command_error (char *cmd)
{
	char *msg;
	
	msg = ft_strjoin("command not found: ", cmd);
	ft_putendl_fd(msg, 2);
	exit(127);
}

void	exec_command(char *cmd_route, char **full_cmd)
{
	char *msg;
	
	if (execve(cmd_route, full_cmd, NULL) < 0)
	{
		msg = ft_strjoin("command not found: ", cmd_route);
		exit(127);
	}
}
void exec_command_route(char *cmd_route, char **full_cmd)
{
	char *msg;
	char const *c = " ";
	cmd_route = ft_strtrim(cmd_route, c); 
	if (execve(cmd_route, full_cmd, NULL) < 0)
	{
		msg = ft_strjoin("command not found: ", cmd_route);
		ft_putendl_fd(msg, 2);
		exit(127);
	}
}
void	ft_check_command(char *cmd, char **envp)
{
	int		i;
	char	**full_cmd;
	char	*path;
	char	**path_split;

	i = 0;
	full_cmd = ft_split(cmd, ' ');
	if (access(full_cmd[0], F_OK | X_OK) == 0)
		exec_command(cmd, full_cmd);
	else
	{
		cmd = ft_strjoin("/", full_cmd[0]);
		path = ft_find_path(envp);
		if (path == NULL)
			exit(1);
		path_split = ft_split(path, ':');
		while (path_split[i] != NULL)
		{
			path_split[i] = ft_strjoin(path_split[i], cmd);
			if (access(path_split[i], F_OK | X_OK) == 0)
				exec_command(path_split[i], full_cmd);
			i++;
		}
		ft_command_error(full_cmd[0]);
	}
}

