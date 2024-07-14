/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:09:55 by razamora          #+#    #+#             */
/*   Updated: 2024/07/14 19:05:45 by razamora         ###   ########.fr       */
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
void	process_one(char **argv, char **envp, int *file_pipe)
{
	int	fd;

	char **path;

	fd = open(argv[1], 0);
	if (fd < 0)
		exit(1);
	dup2(fd,0);
	dup2(file_pipe[1],1);
	close(file_pipe[0]);
	path = ft_find_path(envp);
	ft_check_command(path, argv[2], envp);
	close(fd);
}

void	process_two(char **argv, char **envp, int *file_pipe)
{
	char **path;
	int	fd;
	fd = open(argv[4],O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
		exit(1);
	dup2(fd,1);
	dup2(file_pipe[0],0);
	close(file_pipe[1]);
	path = ft_find_path(envp);
	ft_check_command(path, argv[3], envp);
	close(fd);
}

int	main(int argc, char **argv, char **envp)
{
	// char *cmd_args[] = {"ls","-l", NULL};
	// execve("/usr/bin/ls", cmd_args, NULL);
	// exit(1);
	int	file_pipe[2];
	int	id;
	pipe(file_pipe);
	id = fork();
	if(id < 0)
		return(1);
	if(id == 0)
		process_one(argv, envp,file_pipe);
	else
		process_two(argv, envp, file_pipe);
	waitpid(-1, NULL,0);
	return (0);
}
