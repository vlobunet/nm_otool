/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <vlobunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 19:38:38 by vlobunet          #+#    #+#             */
/*   Updated: 2020/01/29 11:39:11 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/nm_otool.h"

void	print_str(char *ret)
{
	int i;
	int len;

	i = 0;
	len = ft_strlen(ret);
	while (i != 15 - len)
	{
		ft_putchar_fd('0', STDOUT);
		i++;
	}
	ft_putstr_fd(ret, STDOUT);
	ft_putchar_fd(32, STDOUT);
}

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
			print_str(ret);
			ft_strdel(&ret);
		}
		ft_putchar_fd(' ', STDOUT);
		ft_putchar_fd(sort->type, STDOUT);
		ft_putchar_fd(' ', STDOUT);
		ft_putendl_fd(sort->str, STDOUT);
		sort = sort->n;
	}
}

int		manager_fat(t_fmanager func_ptr)
{
	static t_cmanager	func_cmanager[3];
	struct fat_header	*hdr;
	size_t				off;
	uint32_t			magic;

	off = 0;
	func_cmanager[0] = &segment_manager;
	func_cmanager[1] = &symtab_manager_86;
	func_cmanager[2] = &symtab_manager_64;
	if (!(hdr = get_struct(0, sizeof(*hdr))))
		return (err(ERR_FILE, "missing fat header"));
	if (!func_ptr(get_4b(hdr->nfat_arch), sizeof(*hdr), &off, &magic))
		return (err(ERR_SYS, __func__));
	magic == MH_CIGAM || magic == MH_MAGIC ? g_f.is_64 = 0 : 0;
	magic == MH_CIGAM_64 || magic == MH_MAGIC_64 ? g_f.is_64 = 1 : 0;
	if (!off)
		return (err(ERR_FILE, "no known architectures found"));
	g_f.ofset = off;
	!g_f.is_64 ? main_parser_86(func_cmanager[0], LC_SEGMENT) :
	main_parser_64(func_cmanager[0], LC_SEGMENT_64);
	!g_f.is_64 ? main_parser_86(func_cmanager[1], LC_SYMTAB) :
	main_parser_64(func_cmanager[2], LC_SYMTAB);
	return (1);
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
	g_f.type == 3 ? manager_fat(func_fmanager[0]) : 0;
	g_f.type == 4 ? manager_fat(func_fmanager[1]) : 0;
	g_f.type == 5 ? manager_arch() : 0;
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
	return (0);
}
