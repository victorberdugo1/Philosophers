/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:21:35 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/12 17:22:27 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

static void	ft_putstr(const char *s)
{
	while (s && *s)
		ft_putchar(*s++);
}

static void	ft_putnbr(int n)
{
	if (n == -2147483648)
	{
		ft_putstr("-2147483648");
		return;
	}
	if (n < 0)
	{
		ft_putchar('-');
		n = -n;
	}
	if (n >= 10)
		ft_putnbr(n / 10);
	ft_putchar('0' + (n % 10));
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i = 0;

	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			if (format[i] == 's')
				ft_putstr(va_arg(args, char *));
			else if (format[i] == 'd')
				ft_putnbr(va_arg(args, int));
			else if (format[i] == 'c')
				ft_putchar((char)va_arg(args, int));
			else
				ft_putchar(format[i]);
		}
		else
			ft_putchar(format[i]);
		i++;
	}
	va_end(args);
	return (0);
}
