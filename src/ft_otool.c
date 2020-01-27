/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 19:39:16 by vlobunet          #+#    #+#             */
/*   Updated: 2020/01/27 19:39:18 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/nm_otool.h"

void	print_normal_format(char *str)
{
	uint16_t	len;

	len = ft_strlen(str);
	if (len >= 2)
	{
		ft_putchar_fd(str[len - 2], STDOUT);
		ft_putchar_fd(str[len - 1], STDOUT);
	}
	else if (len == 1)
	{
		ft_putchar_fd('0', STDOUT);
		ft_putchar_fd(str[0], STDOUT);
	}
	else
		ft_putstr_fd("00", STDOUT);
	ft_putchar_fd(' ', 1);
	ft_strdel(&str);
}

void	print_text_section(uint64_t size, uint64_t offset, char *text)
{
	uint64_t	i;
	uint64_t	j;
	char		*adr;

	g_f.attributes->f ? ft_putendl_fd(*g_f.attributes->f, STDOUT) :
	ft_putendl_fd("a.out", STDOUT);
	ft_putstr_fd("Contents of (__TEXT,__text) section\n", STDOUT);
	i = 0;
	while (i < size)
	{
		adr = ft_itoabase(offset + i, 16);
		ft_putstr_fd(adr, STDOUT);
		ft_strdel(&adr);
		ft_putchar_fd('\t', STDOUT);
		j = 0;
		while (j < 0x10 && i + j < size)
			print_normal_format(ft_itoabase(text[i + j++], 16));
		ft_putchar_fd('\n', STDOUT);
		i += 0x10;
	}
}

int		main(int argc, char **argv)
{
	static t_cmanager	func_ptr[2];

	func_ptr[0] = &segment_manager_x86;
	func_ptr[1] = &segment_manager_x64;
	if (mmap_file(argc, argv, OTOOL))
		return (1);
	if ((g_f.is_64 = check_architecture()) == -1)
		return (1);
	g_f.is_64 ? main_parser_64(func_ptr[1], LC_SEGMENT_64) :
	main_parser_86(func_ptr[0], LC_SEGMENT);
	if (munmap_file(g_f))
		return (1);
	free(g_f.attributes);
	return (0);
}
