/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_exp.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razamora <razamora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 16:16:00 by razamora          #+#    #+#             */
/*   Updated: 2024/07/28 16:18:47 by razamora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_EXP_H
# define HANDLER_EXP_H

int		ft_error_file(int *file_pipe, char *file);
void	ft_print_error(char *title, int code, char *arg);

#endif