/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 19:42:13 by vlobunet          #+#    #+#             */
/*   Updated: 2020/01/27 19:42:14 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/nm_otool.h"

t_attr	*check_argv(char **argv, t_attr *atr, uint8_t is_nm)
{
	if (argv && *argv && *argv[0] == '-' && !check_lines(argv, is_nm))
		return (NULL);
	atr = init_array_attributes();
	while (argv && *argv && *argv[0] == '-')
	{
		search_letter(*argv, 'U') ? atr->au = 1 : 0;
		search_letter(*argv, 'T') ? atr->at = 1 : 0;
		search_letter(*argv, 'B') ? atr->ab = 1 : 0;
		search_letter(*argv, 'S') ? atr->as = 1 : 0;
		search_letter(*argv, 'D') ? atr->ad = 1 : 0;
		search_letter(*argv, 'A') ? atr->aa = 1 : 0;
		search_letter(*argv, 'a') ? atr->a = 1 : 0;
		search_letter(*argv, 't') ? atr->t = 1 : 0;
		search_letter(*argv, 'n') ? atr->n = 1 : 0;
		search_letter(*argv, 'p') ? atr->n = 1 : 0;
		search_letter(*argv, 'g') ? atr->g = 1 : 0;
		argv = argv + 1;
	}
	if (!atr->t && !is_nm)
		return (atr);
	atr->f = (argv && *argv ? argv : NULL);
	return (atr);
}

int		mmap_file(int argc, char **argv, uint8_t is_nm)
{
	int			fd;
	struct stat	buf;
	void		*p;
	t_attr		*attr;

	if (argc && !(attr = check_argv(argv + 1, NULL, is_nm)))
		return (1);
	if (!is_nm && !attr->t)
	{
		free(attr);
		return (err_otool(ERR_USAGE, "invalid number attributes"));
	}
	if ((fd = open((!attr->f ? "a.out" : *(attr->f)), O_RDONLY)) == -1)
		return (err(ERR_FILE, "No such file or directory"));
	if (fstat(fd, &buf) < 0 || buf.st_mode & S_IFDIR)
		return (err(ERR_FILE, "you can only use MACH-O files."));
	if ((p = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (err(ERR_SYS, "mmap: unable to map file to memory"));
	close(fd);
	g_f.lstsym = NULL;
	g_f.attributes = attr;
	g_f.p = p;
	g_f.size = buf.st_size;
	g_f.ofset = 0;
	return (0);
}

int		munmap_file(t_save_file f)
{
	if (munmap(f.p, f.size))
		return (err(ERR_SYS, "Unable to free file displayed in memory"));
	return (0);
}

int		check_architecture(void)
{
	uint32_t	*magic;

	if (!(magic = get_struct(0, sizeof(*magic))))
	{
		err(ERR_SYS, "unable magic");
		return (-1);
	}
	if (*magic == MH_CIGAM || *magic == MH_MAGIC)
		return (0);
	if (*magic == MH_CIGAM_64 || *magic == MH_MAGIC_64)
		return (1);
	err(ERR_FILE, "unknown format");
	return (-1);
}
