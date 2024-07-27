/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:09:55 by razamora          #+#    #+#             */
/*   Updated: 2024/07/27 15:15:25 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_command_error(char *cmd)
{
	ft_putstr_fd("command not found: ", 2);
	ft_putendl_fd(cmd, 2);
	exit(127);
}

void	exec_command(char *cmd_route, char **full_cmd)
{
	char const	*c;

	c = " ";
	cmd_route = ft_strtrim(cmd_route, c);
	if (execve(cmd_route, full_cmd, NULL) < 0)
		ft_command_error(cmd_route);
}
void	ft_check_command(char *cmd, char **envp)
{
	int		i;
	char	**full_cmd;
	char	**path;

	i = 0;
	full_cmd = ft_split(cmd, ' ');
	if (access(full_cmd[0], F_OK | X_OK) == 0)
		exec_command(cmd, full_cmd);
	else
	{
		cmd = ft_strjoin("/", full_cmd[0]);
		path = ft_find_path(envp);
		//ft_putstr_fd(path,2);
		if (path == NULL)
			ft_error("Error: PATH not found", 1);
		while (path[i] != NULL)
		{
			path[i] = ft_strjoin(path[i], cmd);
			if (access(path[i], F_OK | X_OK) == 0)
				exec_command(path[i], full_cmd);
			i++;
		}
		ft_command_error(full_cmd[0]);
	}
}

static int	ft_error_file(int *file_pipe, char *file)
{
	close(file_pipe[0]);
	close(file_pipe[1]);
	perror(file);
}

void	process_one(char **argv, char **envp, int *file_pipe)
{
	int	fd;

	fd = open(argv[1], 0);
	if (fd < 0)
		ft_error_file(file_pipe, argv[1]);
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

	if (argc != 5)
		(perror("format file1 cmd1 cmd2 file2"), exit(1));
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
