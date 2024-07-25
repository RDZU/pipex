/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:09:55 by razamora          #+#    #+#             */
/*   Updated: 2024/07/25 16:03:22 by razamora         ###   ########.fr       */
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

void ft_check_command(char *cmd, char **envp)
{
	int		i;
	char	**full_cmd;
	char	**path;

	i = 0;
	full_cmd = ft_split(cmd, ' ');
	cmd = ft_strjoin("/", full_cmd[0]);
	//exec_command(cmd, path[i], *envp,full_cmd);
	//printf("exec command  %s %s \n", cmd, path[i]);
	path = ft_find_path(envp);
	if (path == NULL)
		ft_error("Error: PATH not found", 2);
	while (path[i] != NULL)
	{
		path[i] = ft_strjoin(path[i], cmd);
		if (access(path[i], F_OK | X_OK) == 0)
			exec_command(cmd, path[i], *envp,full_cmd);
		i++;
	}
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	exit(1);
}
// child
// fd = open(O_CREAT | O_RDWR | O_TRUNC, 0644)
void	process_one(char **argv, char **envp, int *file_pipe)
{
	int	fd;

	fd = open(argv[1], 0);
	dup2(fd,0);
	dup2(file_pipe[1],1);
	close(file_pipe[0]);
	ft_check_command(argv[2], envp);
	close(fd);
}

void	process_two(char **argv, char **envp, int *file_pipe, int argc)
{
	char **path;
	int	fd;
	fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0777);
	dup2(fd,1);
	dup2(file_pipe[0],0);
	close(file_pipe[1]);
	ft_check_command(argv[argc - 2], envp);
	close(fd);
}

int	main(int argc, char **argv, char **envp)
{
	// char *cmd_args[] = {"ls","-l", NULL};
	// execve("/usr/bin/ls", cmd_args, NULL);
	// exit(1);
	int	file_pipe[2];
	pid_t	id;

	
	if (argc < 5)
		(perror("minimum 5 argc"), exit(1));
	if (pipe(file_pipe) == -1)
		exit(0);
	id = fork();
	if (id < 0)
		return(1);
	if (id == 0)
		process_one(argv, envp, file_pipe);
	else
		process_two(argv, envp, file_pipe,argc);
	waitpid(-1, NULL,0);
	return (0);
}


/*
static int	do_exec(char **command, char **env)
{
	char	*cmd;

	if (access(command[0], F_OK | X_OK) == 0)
	{
		if (execve(command[0], command, env) == -1)
			return (ft_free_p2(command), perror("execve"), -1);
	}
	else
	{
		cmd = get_path(command[0], env);
		if (execve(cmd, command, env) == -1)
			return (free(cmd), ft_free_p2(command), perror("execve"), -1);
		free(cmd);
	}
	return (0);
}
*/