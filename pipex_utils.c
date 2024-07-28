/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:22:03 by razamora          #+#    #+#             */
/*   Updated: 2024/07/29 01:34:31 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_find_path(char **envp)
{
	char	*var_env;
	char	*value_path;

	while (*envp != NULL)
	{
		value_path = *envp++;
		if (ft_strncmp(value_path, "PATH=", 5) == 0)
		{
			value_path = value_path + 5;
			return (value_path);
		}
	}
	return (NULL);
}

void	ft_free_memory(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	ft_error(char *str, int code)
{
	ft_putendl_fd(str, 2);
	exit(code);
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
