/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayilmaz <mayilmaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 19:29:46 by mayilmaz          #+#    #+#             */
/*   Updated: 2024/10/22 08:36:17 by mayilmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		write (fd, "-2147483648", 11);
	else if (n < 0)
	{
		n *= -1;
		write (fd, "-", 1);
		ft_putnbr_fd (n, fd);
	}
	else if (n > 9)
	{
		ft_putnbr_fd ((n / 10), fd);
		ft_putnbr_fd ((n % 10), fd);
	}
	else
		ft_putchar_fd ((n + 48), fd);
}
