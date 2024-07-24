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

void	ft_error(char *str, int code)
{
	ft_putendl_fd(str, 2);
	exit(code);
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

void exec_command(char *cmd, char *cmd_route, char *envp, char **full_cmd)
{
	//printf("exec command  %s %s \n", cmd_route, cmd);
	//printf("exec command  %s %s \n", cmd, path[i]);
	int	flag;

	flag = 0;
	if (execve(cmd_route, full_cmd, NULL) < 0)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putendl_fd(cmd_route, 2);
		ft_free_memory(full_cmd);
		exit(0);
	}
//	printf("%d", flag);
//	exit(0);
}

void ft_check_command(char *cmd, char **envp)
{
	int		i;
	char	**full_cmd;
	char	**path;

	if (access(cmd, F_OK || X_OK) == 0)
	{
		printf("se puede ejecutar  %s \n", cmd);
	}
	else
	{
		printf("no se puede ejecutar  %s \n", cmd);
	}

	//exit(1);
	path = ft_find_path(envp);
	if (path == NULL)
		ft_error("Error: PATH not found", 2);
	i = 0;
	full_cmd = ft_split(cmd, ' ');
	cmd = ft_strjoin("/", full_cmd[0]);
	// exec_command(cmd, path[i], *envp,full_cmd);
	
	while (path[i] != NULL)
	{
		path[i] = ft_strjoin(path[i], cmd);
		if (access(path[i], F_OK | X_OK) == 0)
			//exec_command(cmd, path[i], *envp,full_cmd);
			printf("se puede ejecutar  %s \n", path[i]);
		i++;
	}
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd, 2);
}
// child
// fd = open(O_CREAT | O_RDWR | O_TRUNC, 0644)
void	process_one(char **argv, char **envp, int *file_pipe)
{
	int	fd;

	fd = open(argv[1], 0);
	dup2(fd, 0);
	dup2(file_pipe[1],1);
	close(file_pipe[0]);
	ft_check_command(argv[2], envp);
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
	ft_check_command(argv[3], envp);
	close(fd);
}



int	main(int argc, char **argv, char **envp)
{
	int	file_pipe[2];
	int	pid;

	if(argc != 5)
		ft_error("Error: invalid number of arguments", 2);
	if(pipe(file_pipe) == -1)
		exit(-1);
	pid = fork();
	if(pid < 0)
		exit(-1);
	if(pid == 0)
		process_one(argv, envp,file_pipe);
	else
		process_two(argv, envp, file_pipe);
	waitpid(-1, NULL,0);
	return (0);
}
