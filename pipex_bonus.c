/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:13:01 by razamora          #+#    #+#             */
/*   Updated: 2024/07/26 18:24:09 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	process_one(char **argv, char **envp, int *file_pipe)
{
	int	pid;
	int	fd;

	pid = fork();
	if (pid == -1)
		(perror("Error en fork"), exit(0));
	if (pid == 0)
	{
		close(file_pipe[0]);
		fd = open(argv[1], O_RDONLY, 0644);
		dup2(fd, 0);
		dup2(file_pipe[1], 1);
		close(file_pipe[0]);
		close(fd);
		close(file_pipe[1]);
		ft_check_command(argv[2], envp);
	}
	close(file_pipe[1]);
}

int	middle_process(char *cmd, char **envp, int file_pipe[2])
{
	pid_t	id;
	int		middle_file[2];

	if (pipe(middle_file) == -1)
		perror("Error:");
	id = fork();
	if (id == -1)
		perror("Error:");
	else if (id == 0)
	{
		dup2(file_pipe[0], STDIN_FILENO);
		dup2(middle_file[1], STDOUT_FILENO);
		close(file_pipe[1]);
		close(file_pipe[0]);
		close(middle_file[1]);
		close(middle_file[0]);
		ft_check_command(cmd, envp);
	}
	close(file_pipe[0]);
	close(middle_file[1]);
	file_pipe[0] = middle_file[0];
	return (id);
}

void	process_fin(char **argv, char **envp, int argc, int *file_pipe)
{
	pid_t	pid;
	int		fd;
	int		i;

	pid = fork();
	if (pid == -1)
		(perror("Error al crear el fork"), exit(1));
	if (pid == 0)
	{
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(file_pipe[0], 0);
		dup2(fd, 1);
		close(fd);
		close(file_pipe[0]);
		close(file_pipe[1]);
		ft_check_command(argv[argc - 2], envp);
	}
	close(file_pipe[0]);
	close(file_pipe[1]);
	i = 2;
	while (i++ < argc - 1)
		waitpid(-1, NULL, 0);
}

pid_t	ft_here_doc(char *str)
{
	int		fd;
	char	*line;
	pid_t	id;
	int		file_pipe[2];

	if (pipe(file_pipe) == -1)
		perror("Error:");
	id = fork();
	if (id == -1)
		perror("Error:");
	else if (id == 0)
	{
		close(file_pipe[0]);
		while (1)
		{
			write(1, "here_doc>>", 10);
			line = get_next_line(0);
			if (ft_strncmp(line, str, ft_strlen(line) - 1) == 0)
				break ;
			free(line);
		}
	}
	else
		(close(file_pipe[1]), dup2(file_pipe[0], STDIN_FILENO));
	return (id);
}

int	main(int argc, char **argv, char **envp)
{
	int	*id;
	int	i;
	int	file_pipe[2];

	if (argc < 5)
		(perror("minimum 5 argc"), exit(1));
	if (pipe(file_pipe) == -1)
		exit(0);
	process_one(argv, envp, file_pipe);
	i = 3;
	if (ft_strncmp("here_doc", argv[1], 8) == 0)
	{
		if (argc < 6)
			perror("falta argc");
		ft_here_doc(argv[2]);
	}
	else
		i = 2;
	process_one(argv, envp, file_pipe);
	i++;
	while (i < (argc - 2))
		middle_process(argv[i++], envp, file_pipe);
	process_fin(argv, envp, argc, file_pipe);
}
