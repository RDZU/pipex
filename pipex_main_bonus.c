/*


 int	redirect_and_exec(int m_fd[2], char *line, char **env)
{
	int		p_fd[2];
	pid_t	pid;

	if (pipe(p_fd) == -1)
		return (perror("Error al crear el pipe"), 1);
	pid = fork();
	if (pid == -1)
		(perror("Error al crear el hilo"), exit(1));
	else if (pid == 0)
	{
		dup2(m_fd[READ], STDIN_FILENO);
		dup2(p_fd[WRITE], STDOUT_FILENO);

		close(p_fd[WRITE]);
		close(p_fd[READ]);
		close(m_fd[WRITE]);
		close(m_fd[READ]);
		ft_check_command(line, env);
	}
	close(m_fd[READ]);
	close(p_fd[WRITE]);
	m_fd[READ] = p_fd[READ];
	return (pid);
}

void	finaly_process(int *m_fd, char **av, int argc, char **env)
{
	int		id;
	int		fd_out;
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		(perror("Error al crear el hilo"), exit(1));
	else if (pid == 0)
	{
		fd_out = open(av[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		dup2(m_fd[READ], STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
		close(m_fd[READ]);
		close(m_fd[WRITE]);
		ft_check_command(av[argc - 2], env);
	}
	close(m_fd[READ]);
	close(m_fd[WRITE]);
	
	id = 2;
	while (id < argc - 1)
	{
		waitpid(-1, NULL, 0);
		id++;
	}
}
*/


#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#define READ    0    /* index pipe extremo escritura */
#define WRITE   1 

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

int open_file(char *file, int type)
{
	int ret;
	if (type == 0)
		ret = open(file, O_RDONLY, 0644);
	if (type == 1)
		ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (type == 2)
		ret = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (ret == -1)
		(perror(file));
	return (ret);
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
void ft_display(char *cmd, char *route)
{
	char *file = "comandos.log";
	 FILE *fp = fopen(file, "w"); // Open file in write mode
	if (fp == NULL) {
		printf("Error: Cannot open file for writing!\n");
		return;
	}
	fprintf(fp, " | comando: %s | ", cmd);
	fprintf(fp, " | route: %s | ", route);
	fprintf(fp, "\n");
	fclose(fp);
}
void	ft_check_command(char *cmd, char **envp)
{
	int		i;
	char	**full_cmd;
	char	**path;

	i = 0;
	
	full_cmd = ft_split(cmd, ' ');
	cmd = ft_strjoin("/", full_cmd[0]);
	//exec_command(cmd, path[i], *envp,full_cmd);
//	ft_display(cmd, path[i]);
	//printf("exec command  %s %s \n", cmd, path[i]);
	path = ft_find_path(envp);
	// if (path == NULL)
	// 	ft_error("Error: PATH not found", 2);
	while (path[i] != NULL)
	{
		path[i] = ft_strjoin(path[i], cmd);
		if (access(path[i], F_OK | X_OK) == 0)
			exec_command(cmd, path[i], *envp,full_cmd);
		i++;
	}
	ft_putstr_fd("command not found---->", 2);
	ft_putstr_fd(cmd, 2);
	exit(0);
}
// child
// fd = open(O_CREAT | O_RDWR | O_TRUNC, 0644)
// void	process_one(char **argv, char **envp, int *file_pipe)
// {
// 	int		pid;
// 	char	**path;
// 	int		fd;

// 	pid = fork();
// 	if (pid == -1)
// 		(perror("Error en fork"), exit(1));
// 	if (pid == 0)
// 	{
// 		close(file_pipe[0]);
// 		fd = open(argv[1], 0);
// 		dup2(fd, 0);
// 		dup2(file_pipe[1], 1);
// 		close(file_pipe[0]);
// 		close(fd);
// 		close(file_pipe[1]);
// 		ft_check_command(argv[2], envp);
// 	}
// 	close(file_pipe[1]);
// }

// void	process_one(char **argv, char **envp, int *file_pipe)
// {
// 	int		pid;
// 	char	**path;
// 	int		fd;

// 	pid = fork();
// 	if (pid == -1)
// 		(perror("Error en fork"), exit(1));
// 	if (pid == 0)
// 	{
// 		close(file_pipe[0]);
// 		fd = open(argv[1], 0);
// 		dup2(fd, 0);
// 		dup2(file_pipe[1], 1);
// 		close(file_pipe[0]);
// 		close(fd);
// 		close(file_pipe[1]);
// 		ft_check_command(argv[2], envp);
// 	}
// 	close(file_pipe[1]);
// }





void	process_one(char **argv, char **envp, int *file_pipe)
{
	int		pid;
	int		fd;

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

// int	middle_process(char *cmd ,char **envp, int m_fd[2])
// {
// 	pid_t	pid;
// 	int		p_fd[2];

	
// 	if (pipe(p_fd) == -1)
// 		return (perror("Error al crear el pipe"), 1);
// 	pid = fork();
// 	if (pid == -1)
// 		(perror("Error al crear el hilo"), exit(1));
// 	else if (pid == 0)
// 	{
// 		dup2(m_fd[READ], STDIN_FILENO);
// 		dup2(p_fd[WRITE], STDOUT_FILENO);

// 		close(p_fd[WRITE]);
// 		close(p_fd[READ]);
// 		close(m_fd[WRITE]);
// 		close(m_fd[READ]);
// 		ft_check_command(cmd, envp);
// 	}
// 	close(m_fd[READ]);
// 	close(p_fd[WRITE]);
// 	m_fd[READ] = p_fd[READ];
// 	return (pid);
// }

int	middle_process(char *cmd ,char **envp, int file_pipe[2])
{
	pid_t	id;
	int		middle_file[2];

	if (pipe(middle_file) == -1)
		perror ("Error:");
	id = fork();
	if (id == -1)
		perror ("Error:");
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

int	redirect_and_exec(int m_fd[2], char *line, char **env)
{
	int		p_fd[2];
	pid_t	pid;

	if (pipe(p_fd) == -1)
		return (perror("Error al crear el pipe"), 1);
	pid = fork();
	if (pid == -1)
		(perror("Error al crear el hilo"), exit(1));
	else if (pid == 0)
	{
		dup2(m_fd[READ], STDIN_FILENO);
		dup2(p_fd[WRITE], STDOUT_FILENO);

		close(p_fd[WRITE]);
		close(p_fd[READ]);
		close(m_fd[WRITE]);
		close(m_fd[READ]);
		ft_check_command(line, env);
	}
	close(m_fd[READ]);
	close(p_fd[WRITE]);
	m_fd[READ] = p_fd[READ];
	return (pid);
}

void	process_two(char **argv, char **envp, int argc, int *file_pipe)
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
		dup2(fd,1);
		close(fd);
		close(file_pipe[0]);
		close(file_pipe[1]);
		ft_check_command(argv[argc - 2], envp);
	}
	close(file_pipe[0]);
	close(file_pipe[1]);
	i = 2;
	// esperamos todos los subprocesos que no jan terminado
	while (i < argc - 1)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
}
void	finaly_process(int *m_fd, char **av, int argc, char **env)
{
	int		id;
	int		fd_out;
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		(perror("Error al crear el hilo"), exit(1));
	else if (pid == 0)
	{
		fd_out = open(av[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		dup2(m_fd[READ], STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
		close(m_fd[READ]);
		close(m_fd[WRITE]);
		ft_check_command(av[argc - 2], env);
	}
	close(m_fd[READ]);
	close(m_fd[WRITE]);
	
	id = 2;
	while (id < argc - 1)
	{
		waitpid(-1, NULL, 0);
		id++;
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
	int file_pipe[2];

	if (argc < 5)
		(perror("minimum 5 argc"), exit(1));
	if (pipe(file_pipe) == -1)
		exit(0);
	// process_one(argv, envp, file_pipe);
	// i = 3;
	// if (ft_strncmp("here_doc",argv[1],8) == 0)
	// {
	// 	if (argc < 6)
	// 		perror("falta argc");
	// 	ft_here_doc(argv[2]);
	// }
	// else
	
	i = 2;
	//first_process(argv, file_pipe, envp);
	process_one(argv, envp, file_pipe);
	i++;
	while (i < (argc - 2))
		//i += ((redirect_and_exec(file_pipe, argv[i], envp)), 1);
		//(redirect_and_exec(file_pipe, argv[i++], envp));
		middle_process(argv[i++] ,envp,file_pipe);
	//finaly_process(file_pipe, argv, argc, envp);
	process_two(argv, envp, argc,file_pipe);
	// while (i < argc - 2)
	// {
	// 	i += middle_process(argv[i] ,envp, file_pipe);
	// }
	// process_two(argv, envp ,argc, file_pipe);
	//return (0);
}