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

int		main_run(void)
{
	static t_cmanager	func_cmanager[3];
	static t_fmanager	func_fmanager[2];

	func_cmanager[0] = &segment_manager;
	func_cmanager[1] = &symtab_manager_86;
	func_cmanager[2] = &symtab_manager_64;
	func_fmanager[0] = &fat_manager_86;
	func_fmanager[1] = &fat_manager_64;

	g_f.type == 1 ? main_parser_86(func_cmanager[0], LC_SEGMENT) : 0;
	g_f.type == 1 ? main_parser_86(func_cmanager[1], LC_SYMTAB) : 0;
	g_f.type == 2 ? main_parser_64(func_cmanager[0], LC_SEGMENT_64) : 0;
	g_f.type == 2 ? main_parser_64(func_cmanager[2], LC_SYMTAB) : 0;
	g_f.type == 3 ? manager_fat(func_fmanager[0]): 0;
	g_f.type == 4 ? manager_fat(func_fmanager[1]): 0;
	return (0);
}

int		main(int argc, char **argv)
{
	if (mmap_file(argc, argv, NM))
		return (1);
	if ((g_f.type = check_architecture()) == 0)
		return (1);
	main_run();
	lst_sort();
	if (munmap_file(g_f))
		return (1);
	system("leaks ft_nm");
	return (0);
}
