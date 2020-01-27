/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_attributes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 19:35:05 by vlobunet          #+#    #+#             */
/*   Updated: 2020/01/27 19:35:07 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/nm_otool.h"

t_attr	*init_array_attributes(void)
{
	t_attr	*attr;

	if (!(attr = (t_attr *)malloc(sizeof(t_attr))))
		return (NULL);
	attr->au = 0;
	attr->at = 0;
	attr->ab = 0;
	attr->as = 0;
	attr->ad = 0;
	attr->ac = 0;
	attr->ai = 0;
	attr->aa = 0;
	attr->a = 0;
	attr->n = 0;
	attr->t = 0;
	attr->p = 0;
	attr->f = NULL;
	return (attr);
}

int		search_letter(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (1);
		else
			i++;
	}
	return (0);
}

void	print_other(void)
{
	ft_putstr_fd("\t-U -- Display symbols only U/u type.\n", STDERR);
	ft_putstr_fd("\t-T -- Display symbols only T/t type.\n", STDERR);
	ft_putstr_fd("\t-B -- Display symbols only B/b type.\n", STDERR);
	ft_putstr_fd("\t-S -- Display symbols only S/s type.\n", STDERR);
	ft_putstr_fd("\t-D -- Display symbols only D/d type.\n", STDERR);
	ft_putstr_fd("\t-C -- Display symbols only C/c type.\n", STDERR);
	ft_putstr_fd("\t-A -- Display symbols only A/a type.\n", STDERR);
	ft_putstr_fd("\t-g -- Display only global symbol\n", STDERR);
	ft_putstr_fd("\t-a -- Sort by name (alphabet)(default)\n", STDERR);
	ft_putstr_fd("\t-n -- Sort by offset\n", STDERR);
	ft_putstr_fd("\t-t -- Sort by type\n", STDERR);
	ft_putstr_fd("\t-p -- Display as in the symbol table (no sort)\n", STDERR);
}

int		err(const int err, const char *str)
{
	static const char	*msg[ERR_NUMBER];

	msg[0] = "Fatal Error: ";
	msg[1] = "Bad usage: ";
	msg[2] = "Bad file: ";
	ft_putstr_fd(RED, STDERR);
	ft_putstr_fd(msg[err], STDERR);
	ft_putstr_fd(RES, STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putstr_fd("\n", STDERR);
	if (err == ERR_USAGE)
	{
		ft_putstr_fd("usage: ./ft_nm [-UTBSDCIAgantp] <files> \n", STDERR);
		print_other();
	}
	return (1);
}
