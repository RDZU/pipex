/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:09:55 by razamora          #+#    #+#             */
/*   Updated: 2024/07/26 07:31:45 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (path == NULL)
		ft_error("Error: PATH not found", 2);
	while (path[i] != NULL)
	{
		path[i] = ft_strjoin(path[i], cmd);
		if (access(path[i], F_OK | X_OK) == 0)
			exec_command(cmd, path[i], *envp, full_cmd);
		i++;
	}
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	exit(1);
}

void	process_one(char **argv, char **envp, int *file_pipe)
{
	int	fd;

	fd = open(argv[1], 0);
	dup2(fd, 0);
	dup2(file_pipe[1], 1);
	close(file_pipe[0]);
	ft_check_command(argv[2], envp);
	close(fd);
}

void	process_two(char **argv, char **envp, int *file_pipe, int argc)
{
	char	**path;
	int		fd;

	fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0777);
	dup2(fd, 1);
	dup2(file_pipe[0], 0);
	close(file_pipe[1]);
	ft_check_command(argv[argc - 2], envp);
	close(fd);
}

int	main(int argc, char **argv, char **envp)
{
	int		file_pipe[2];
	pid_t	id;

	if (argc < 5)
		(perror("minimum 5 argc"), exit(1));
	if (pipe(file_pipe) == -1)
		exit(0);
	id = fork();
	if (id < 0)
		return (1);
	if (id == 0)
		process_one(argv, envp, file_pipe);
	else
		process_two(argv, envp, file_pipe, argc);
	waitpid(-1, NULL, 0);
	return (0);
}