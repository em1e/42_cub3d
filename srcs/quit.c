/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:20:47 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/02 15:15:45 by jajuntti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

/*
Cleans all allocated memory and quits with provided error message using perror.
*/
void	quit_perror(t_cub3d *kissa, char *file, char *error_message)
{
	if (kissa)
		clean_kissa(kissa);
	ft_putstr_fd("Error: ", 2);
	if (file)
	{
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
	}
	perror(error_message);
	exit(1);
}

/*
Quits with a provided error message.
*/
void	quit_error(t_cub3d *kissa, char *file, char *error_message)
{
	if (kissa)
		clean_kissa(kissa);
	ft_putstr_fd("Error: ", 2);
	if (file)
	{
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(error_message, 2);
	exit(1);
}

