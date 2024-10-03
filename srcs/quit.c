/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:20:47 by jajuntti          #+#    #+#             */
/*   Updated: 2024/10/03 03:07:01 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

/*
Cleans all allocated memory and quits with provided error message using perror.
*/
void	quit_perror(t_cub3d *kissa, char *file, char *error_message)
{
	char	*temp_file;
	char	*extra;
	char	*perror_msg;

	temp_file = ft_strdup(file);
	perror_msg = NULL;
	extra = NULL;
	if (kissa)
		clean_kissa(kissa);
	if (temp_file)
	{
		extra = ft_strjoin("Error: ", temp_file);
		free(temp_file);
		perror_msg = ft_strjoin(extra, error_message);
		free(extra);
	}
	else
		perror_msg =  ft_strjoin("Error: ", error_message);
	perror(perror_msg);
	free(perror_msg);
	exit(1);
}

/*
Quits with a provided error message.
*/
void	quit_error(t_cub3d *kissa, char *temp, char *error_message)
{
	char	*file;

	file =  NULL;
	if (temp)
		file = ft_strdup(temp);
	if (kissa)
		clean_kissa(kissa);
	ft_putstr_fd("Error: ", 2);
	if (file)
	{
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
	}
	free(file);
	ft_putendl_fd(error_message, 2);
	exit(1);
}

