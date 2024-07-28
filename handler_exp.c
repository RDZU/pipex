/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 16:15:31 by razamora          #+#    #+#             */
/*   Updated: 2024/07/28 16:17:37 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handler_exp.h"

int	ft_error_file(int *file_pipe, char *file)
{
	close(file_pipe[0]);
	close(file_pipe[1]);
	perror(file);
}

void	ft_print_error(char *title, int code, char *arg)
{
	char	*str;

	str = ft_strjoin(title, arg);
	if (!str)
		exit(1);
	ft_putendl_fd(str, 2);
	free(str);
	exit(code);
}
