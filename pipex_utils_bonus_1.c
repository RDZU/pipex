/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:18:50 by razamora          #+#    #+#             */
/*   Updated: 2024/07/29 01:10:48 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_error(char *str, int code)
{
	ft_putendl_fd(str, 2);
	exit(code);
}

int	ft_command_error(char *cmd)
{
	cmd = ft_strjoin(cmd, "\n");
	write(2, "command not found: ", 20);
	write(2, cmd, ft_strlen(cmd));
	free(cmd);
	exit(127);
}

void	exec_command_route(char *cmd_route, char **full_cmd)
{
	char		*msg;
	char const	*c;

	c = " ";
	cmd_route = ft_strtrim(cmd_route, c);
	if (execve(cmd_route, full_cmd, NULL) < 0)
	{
		msg = ft_strjoin("command not found: ", cmd_route);
		ft_putendl_fd(msg, 2);
		exit(127);
	}
}

char	*ft_get_path(char **full_cmd, char **envp)
{
	char	*cmd;
	char	*path;
	char	*temp;
	char	**path_split;
	int		i;

	path = ((i = 0), ft_find_path(envp));
	if (path == NULL)
		ft_command_error(ft_strdup(full_cmd[0]));
	cmd = ft_strjoin("/", full_cmd[0]);
	path_split = ft_split(path, ':');
	while (path_split[i] != NULL)
	{
		path = ft_strjoin(path_split[i], cmd);
		if (access(path, F_OK | X_OK) == 0)
			return (free(cmd), ft_free_memory(path_split), path);
		free(path);
		i++;
	}
	(free(cmd), ft_free_memory(path_split));
	return (NULL);
}

void	ft_check_command(char *cmd, char **envp)
{
	char	**full_cmd;
	char	*path;
	char	**path_split;

	full_cmd = ft_split(cmd, ' ');
	if (access(full_cmd[0], F_OK | X_OK) == 0)
	{
		if (execve(full_cmd[0], full_cmd, envp) == -1)
		{
			(ft_command_error(full_cmd[0]), ft_free_memory(full_cmd));
		}
	}
	else
	{
		path = ft_get_path(full_cmd, envp);
		if (execve(path, full_cmd, envp) == -1)
			(ft_command_error(full_cmd[0]), ft_free_memory(full_cmd));
		ft_free_memory(full_cmd);
		free(path);
	}
}
