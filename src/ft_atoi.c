/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:40:03 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/18 12:30:17 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *nptr)
{
	int	res;
	int	i;
	int	negf;

	i = 0;
	res = 0;
	negf = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == 43 || nptr[i] == 45)
	{
		if (nptr[i] == 45)
			negf = -1;
		i++;
	}
	if (negf == -1)
		return (0);
	while (ft_isdigit(nptr[i]))
	{
		res = (res * 10) + (nptr[i] - '0');
		i++;
	}
	if (nptr[i] != '\0' && !ft_isdigit(nptr[i]))
		return (0);
	return (res * negf);
}
