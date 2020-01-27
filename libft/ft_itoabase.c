/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoabase.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 19:57:00 by vlobunet          #+#    #+#             */
/*   Updated: 2020/01/27 19:57:02 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*rg(char *p, unsigned long long n, unsigned int s, unsigned int b)
{
	unsigned int		index;
	unsigned long long	lol;

	index = 0;
	lol = n;
	while (s > index && s)
	{
		if ((p[s - 1] = lol % b + '0') > 57)
			p[s - 1] = lol % b + '0' + 39;
		lol = lol / b;
		s--;
	}
	return (p);
}

char	*ft_itoabase(unsigned long long n, unsigned int base)
{
	unsigned long long	test;
	unsigned int		taille;
	char				*ret;

	ret = NULL;
	taille = 1;
	test = n;
	while (test / base != 0)
	{
		taille++;
		test = test / base;
	}
	ret = ft_strnew(taille);
	if (ret)
		ret = rg(ret, n, taille, base);
	else
		return (NULL);
	return (ret);
}
