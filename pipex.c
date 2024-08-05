/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:09:55 by razamora          #+#    #+#             */
/*   Updated: 2024/08/04 14:51:13 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_command_error(char *cmd)
{
	cmd = ft_strjoin(cmd, "\n");
	write(2, "command not found: ", 20);
	write(2, cmd, ft_strlen(cmd));
	free(cmd);
	exit(127);
}

static pid_t	process_one(char **argv, char **envp, int *file_pipe)
{
	int		fd;
	pid_t	id;

	id = fork();
	if (id == -1)
		(perror("Error:"), exit(1));
	if (id == 0)
	{
		fd = open(argv[1], O_RDONLY, 0664);
		if (fd < 0)
			(ft_error_file(file_pipe, argv[1]), exit(1));
		dup2(fd, 0);
		dup2(file_pipe[1], 1);
		close(file_pipe[0]);
		close(file_pipe[1]);
		close(fd);
		ft_check_command(argv[2], envp);
	}
	return (id);
}

static pid_t	process_two(char **argv, char **envp, int *file_pipe, int argc)
{
	int		fd;
	pid_t	id;

	id = fork();
	if (id == 0)
	{
		fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0777);
		if (fd < 0)
			(ft_error_file(file_pipe, argv[argc - 1]), exit(1));
		dup2(file_pipe[0], 0);
		dup2(fd, 1);
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
		(write(2, "format: file1 cmd1 cmd2 file2", 29), exit(1));
	if (pipe(file_pipe) == -1)
		(perror("Error:"), exit(1));
	id[0] = process_one(argv, envp, file_pipe);
	id[1] = process_two(argv, envp, file_pipe, argc);
	close(file_pipe[0]);
	close(file_pipe[1]);
	waitpid(id[0], &status, 0);
	waitpid(id[1], &status, 0);
	exit(WEXITSTATUS(status));
	return (0);
}
