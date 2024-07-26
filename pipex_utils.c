/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 07:22:03 by razamora          #+#    #+#             */
/*   Updated: 2024/07/26 07:32:12 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_find_path(char **envp)
{
	char	**var_env;
	char	*value_path;

	while (*envp != NULL)
	{
		value_path = *envp++;
		if (ft_strncmp(value_path, "PATH=", 5) == 0)
		{
			value_path = value_path + 5;
			break ;
		}
	}
	var_env = ft_split(value_path, ':');
	return (var_env);
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
