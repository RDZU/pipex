/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_handle_error_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:22:03 by razamora          #+#    #+#             */
/*   Updated: 2024/07/30 21:17:36 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	ft_command_error(char *cmd)
{
	cmd = ft_strjoin(cmd, "\n");
	write(2, "command not found: ", 20);
	write(2, cmd, ft_strlen(cmd));
	free(cmd);
	exit(127);
}

void	ft_is_empty(char *cmd)
{
	while ((*cmd >= 9 && *cmd <= 13) || *cmd == ' ')
		cmd++;
	if (*cmd == '\0')
		(ft_command_error(cmd));
}
