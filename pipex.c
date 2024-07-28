/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:09:55 by razamora          #+#    #+#             */
/*   Updated: 2024/07/28 19:38:28 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_command_error(char *cmd)
{
	cmd = ft_strjoin(cmd,"\n");
	write(2, "command not found: ", 20);
	write(2,cmd, ft_strlen(cmd));
	free(cmd);
	exit(127);
}

int	exec_command(char **full_cmd, char **env)
{
	char const	*c;

	c = " ";
	if (execve(full_cmd[0], full_cmd, env) == -1)
		return (-1);
}

char *ft_get_path(char **full_cmd, char **envp)
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
			(ft_command_error(full_cmd[0]),ft_free_memory(full_cmd));
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

static pid_t	process_one(char **argv, char **envp, int *file_pipe)
{
	int		fd;
	pid_t	id;

	id = fork();
	if (id == 0)
	{
		fd = open(argv[1], O_RDONLY, 0664);
		if (fd < 0)
			ft_error_file(file_pipe, argv[1]);
		dup2(fd, STDIN_FILENO);
		dup2(file_pipe[1], STDOUT_FILENO);
		close(file_pipe[0]);
		close(file_pipe[1]);
		close(fd);
		ft_check_command(argv[2], envp);
	}
	return (id);
}

// void	ft_perror(char *str)
// {
// 	if (str == NULL)
// 		write(2, "\"\": empty command\n", 19);
// 	else if ((str != NULL) && (errno != 0))
// 		perror(str);
// 	else
// 	{
// 		write(2, str, ft_strlen(str));
// 		write(2, ": command not found\n", 21);
// 	}
// }

static pid_t	process_two(char **argv, char **envp, int *file_pipe, int argc)
{
	char	**path;
	int		fd;
	pid_t	id;

	id = fork();
	if (id == 0)
	{
		fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0777);
		if (fd < 0)
			ft_error_file(file_pipe, argv[argc - 1]);
		dup2(fd, 1);
		dup2(file_pipe[0], 0);
		close(file_pipe[1]);
		close(file_pipe[0]);
		close(fd);
		ft_check_command(argv[argc - 2], envp);
	}
	return (id);
}

int	main(int argc, char **argv, char **envp)
{
	int		file_pipe[2];
	pid_t	id[2];
	int		status;

	if (argc != 5)
		(perror("format file1 cmd1 cmd2 file2"), exit(1));
	if (pipe(file_pipe) == -1)
		exit(1);
	id[0] = process_one(argv, envp, file_pipe);
	id[1] = process_two(argv, envp, file_pipe, argc);
	close(file_pipe[0]);
	close(file_pipe[1]);
	waitpid(id[0], &status, 0);
	waitpid(id[1], &status, 0);
	if (WEXITSTATUS(status) == 127)
		exit(127);
	return (0);
}
