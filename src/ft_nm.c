/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 19:38:38 by vlobunet          #+#    #+#             */
/*   Updated: 2020/01/27 19:38:39 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/nm_otool.h"

void	print_lst(t_sym *sort)
{
	char	*ret;

	while (sort)
	{
		if (g_f.attributes->aa)
		{
			g_f.attributes->f ? ft_putstr_fd(*g_f.attributes->f, STDOUT) :
			ft_putstr_fd("a.out", STDOUT);
			ft_putstr_fd(": ", STDOUT);
		}
		if (!sort->off)
			ft_putstr_fd("                ", STDOUT);
		else
		{
			ret = ft_itoabase(sort->off, 16);
			ft_putstr_fd("0000000", STDOUT);
			ft_putstr_fd(ret, STDOUT);
			ft_strdel(&ret);
		}
		ft_putchar_fd(' ', STDOUT);
		ft_putchar_fd(sort->type, STDOUT);
		ft_putchar_fd(' ', STDOUT);
		ft_putendl_fd(sort->str, STDOUT);
		sort = sort->n;
	}
}

int		main(int argc, char **argv)
{
	static t_cmanager	func_ptr[3];

	func_ptr[0] = &segment_manager;
	func_ptr[1] = &symtab_manager_86;
	func_ptr[2] = &symtab_manager_64;
	if (mmap_file(argc, argv, NM))
		return (1);
	if ((g_f.is_64 = check_architecture()) == -1)
		return (1);
	g_f.is_64 ? main_parser_64(func_ptr[0], LC_SEGMENT_64) :
	main_parser_86(func_ptr[0], LC_SEGMENT);
	g_f.is_64 ? main_parser_64(func_ptr[2], LC_SYMTAB) :
	main_parser_86(func_ptr[1], LC_SYMTAB);
	lst_sort();
	if (munmap_file(g_f))
		return (1);
	system("leaks ft_nm");
	return (0);
}
