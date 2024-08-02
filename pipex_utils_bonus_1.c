/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:18:50 by razamora          #+#    #+#             */
/*   Updated: 2024/08/02 19:38:04 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*ft_get_path(char **full_cmd, char **envp)
{
	char	*cmd;
	char	*path;
	char	**path_split;
	int		i;

	i = 0;
	path = ft_find_path(envp);
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

	ft_is_empty(cmd);
	full_cmd = ft_split(cmd, ' ');
	if (access(full_cmd[0], F_OK | X_OK) == 0)
	{
		if (execve(full_cmd[0], full_cmd, envp) == -1)
			(ft_command_error(full_cmd[0]), ft_free_memory(full_cmd));
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

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	ft_error_file(int *file_pipe, char *file)
{
	close(file_pipe[0]);
	close(file_pipe[1]);
	perror(file);
}
