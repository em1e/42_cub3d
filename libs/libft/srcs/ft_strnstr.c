/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:42:12 by vkettune          #+#    #+#             */
/*   Updated: 2024/09/25 21:08:33 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	if (!haystack && len == 0)
		return (0);
	if (*needle == 0)
		return ((char *)haystack);
	if (ft_strlen(needle) > ft_strlen(haystack) || len == 0)
		return (0);
	while (ft_strlen(needle) <= len)
	{
		if (ft_strncmp(needle, haystack, ft_strlen(needle)) == 0)
			return ((char *)haystack);
		haystack++;
		len--;
	}
	return (NULL);
}
