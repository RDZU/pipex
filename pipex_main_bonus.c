/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:09:55 by razamora          #+#    #+#             */
/*   Updated: 2024/07/21 15:17:16 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

char	**ft_find_path(char **envp)
{
	char **var_env;
	char *value_path;
	while (*envp != NULL)
	{
		value_path = *envp++;
		if (ft_strncmp(value_path,"PATH=",5) == 0)
		{
			value_path = value_path + 5;
			break;
		}	
	}
	var_env = ft_split(value_path,':');	
	return(var_env);
	
}

void exec_command(char *cmd, char *cmd_route, char *envp, char **full_cmd)
{
	//printf("exec command  %s %s \n", cmd_route, cmd);
	//printf("exec command  %s %s \n", cmd, path[i]);
	int	flag;

	flag = 0;
	if (execve(cmd_route, full_cmd,NULL) < 0)
	{
		printf("command error");
		exit(1);
	}
	
//	printf("%d", flag);
//	exit(0);
}

void ft_check_command(char **path, char *cmd, char **envp)
{
	int		i;
	char	**full_cmd;

	i = 0;
	full_cmd = ft_split(cmd, ' ');
	cmd = ft_strjoin("/", full_cmd[0]);
	//exec_command(cmd, path[i], *envp,full_cmd);
	//printf("exec command  %s %s \n", cmd, path[i]);
	while (path[i] != NULL)
	{
		path[i] = ft_strjoin(path[i], cmd);
		if (access(path[i], F_OK | X_OK) == 0)
			exec_command(cmd, path[i], *envp,full_cmd);
		i++;
	}
	printf("command not found");
	exit(1);
}
// child
// fd = open(O_CREAT | O_RDWR | O_TRUNC, 0644)
void	process_one(char **argv, char **envp)
{
	int	fd;
	char **path;

	fd = open(argv[1], O_RDONLY | O_CREAT, 777);
	if (fd < 0)
		exit(1);
	dup2(fd, STDIN_FILENO);
	//dup2(file_pipe[1],1);
	//close(file_pipe[0]);
	//path = ft_find_path(envp);
	//ft_check_command(path, argv[2], envp);
	close(fd);
}

pid_t	middle_process(char *cmd ,char **envp, int i)
{
	pid_t id;
	int file_pipe[2];
	int status;
	char **path;
	if (pipe(file_pipe) == -1)
		perror ("Error:");
	id = fork();

	if (id == -1)
		perror ("Error:");
	if (id == 0)
	{
		close(file_pipe[0]);
		dup2(file_pipe[1], STDOUT_FILENO);
		close(file_pipe[1]);
		path = ft_find_path(envp);
		ft_check_command(path, cmd, envp);
		
	}
	else
	{
		close(file_pipe[1]);
		dup2(file_pipe[0], STDIN_FILENO);
		close(file_pipe[0]);
	}
	return (id);
}

void	process_two(char **argv, char **envp, int argc, pid_t *pids)
{
	char	**path;
	pid_t	pid;
	int		fd;
	int		i;
	int		status;

	fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pid = fork();
	if (pid == -1)
		(perror("Error al crear el hilo"), exit(1));
	else if (pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		path = ft_find_path(envp);
	//	close(fd);
		ft_check_command(path, argv[argc - 2], envp);
	}
	
	pids[argc - 2] = pid;
	i = 1;
	status = 0;
	while (i < argc - 1)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}

}

pid_t	ft_here_doc(char *str)
{
	int		fd;
	char	*line;
	pid_t	id;
	int		file_pipe[2];
	
	if (pipe(file_pipe) == -1)
		perror ("Error:");
	id = fork();
	if (id == -1)
		perror ("Error:");
	else if (id == 0)
	{
		close(file_pipe[0]);
		while (1)
		{
			write(1,"here_doc>>",10);
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
	int	status;
	pid_t	pids[argc - 1];

	status = 0;
	if (argc < 5)
		return (-1);
	i = 2;
	if (ft_strncmp("here_doc",argv[1],8) == 0)
	{
		if (argc < 6)
			perror("falta argc");
		id[i++ - 2] = ft_here_doc(argv[2]);
	}
	else
		process_one(argv, envp);
	while (i < argc - 2)
	{
		pids[i - 2] = middle_process(argv[i] ,envp, i);
		i++;
	}
	process_two(argv, envp ,argc, pids);
	return (0);
}



/*

void    finaly_process(char **av, int argc, char **env, pid_t *pids)
{
	pid_t    pid;
	int        fd_out;
	int        i;
	int        status;

	fd_out = open_file(av[argc - 1], STDOUT_FILENO);
	pid = fork();
	if (pid == -1)
		(perror("Error al crear el hilo"), exit(1));
	else if (pid == 0)
	{
		dup2(fd_out, 1);
		exec_cmd(av[argc - 2], env);
		close(fd_out);
	}
	pids[argc - 2] = pid;
	i = 1;
	status = 0;
	while (i < argc - 1)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
}
*/
/*
int	main(int argc, char **av, char **env)
{
	int		i;
	int		fd_in;
	int		fd_out;

	if (argc < 5)
		return (-1);
	i = 2;
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		if (argc < 6)
			(perror("d"), exit(0));
		i = 3;
		fd_out = open_file(av[argc - 1], 2);
		here_doc(av[2]);
	}
	else
	{
		fd_in = open_file(av[1], STDIN_FILENO);
		fd_out = open_file(av[argc - 1], STDOUT_FILENO);
		dup2(fd_in, STDIN_FILENO);
	}
	while (i < (argc - 2))
		redirect_and_exec(av[i++], env);
	dup2(fd_out, STDOUT_FILENO);
	(exec_cmd(av[argc - 2], env), close(fd_out));
}
*/


/*
static int here_doc(char *limiter)
{
	int		p_fd[2];
	pid_t	pid;
	int	status;
	char *p_heredoc;
	
	if (pipe(p_fd) == -1)
		return (perror("Error al crear el pipe"), 1);
	pid = fork();
	if (pid == -1)
		(perror("Error al crear el hilo"), exit(1));
	else if (pid == 0)
	{
		close(p_fd[0]);
		while(1)
		{
			p_heredoc = get_next_line(0);
			if (ft_strncmp(p_heredoc, limiter, ft_strlen(limiter)) == 0)
				(free(p_heredoc), exit(0));
			(ft_putstr_fd(p_heredoc, p_fd[1]), free(p_heredoc));
		}
	}
	else
		(waitpid(pid, &status, 0), close(p_fd[1]), dup2(p_fd[0], 0));
}
*/