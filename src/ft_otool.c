#include "../includes/nm_otool.h"

int ft_ottol_mmap(char *fname)
{
	int			fd;
	struct stat	buf;
	void		*p;

	if ((fd = open(fname, O_RDONLY)) == -1)
		return (err_otool(ERR_FILE, "No such file or directory"));
	if (fstat(fd, &buf) < 0 || buf.st_mode & S_IFDIR)
		return (err_otool(ERR_FILE, "you can only use MACH-O files."));
	if ((p = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (err_otool(ERR_SYS, "mmap: unable to map file to memory"));
	close(fd);
	g_f.p = p;
	g_f.size = buf.st_size;
	g_f.ofset = 0;
	return (0);
}

static int ft_otool_t(char *fname)
{
	static t_lc func_ptr[2];

	func_ptr[0] = &otool_print_sector;
	func_ptr[1] = &otool_print_sector_64;
	if (ft_ottol_mmap(fname))
		return (1);
	if ((g_f.is_64 = check_architecture()) == -1)
		return (1);
	if (g_f.is_64)
		main_parser_64(func_ptr[0], LC_SEGMENT_64);
	else
		main_parser(func_ptr[0], LC_SEGMENT);
	return (0);
}

int main(int argc, char **argv)
{
	t_attr *attributes;

	attributes = NULL;
	if (argc && !(attributes = check_atributes(argv + 1)))
		return (1);
	if (attributes->files && !*(attributes->files) && attributes->t)
		ft_otool_t("a.out");
	while (attributes->files && *(attributes->files))
	{
		if (attributes->t)
			ft_otool_t(*(attributes->files));
		(attributes->files)++;
	}
	free(attributes);
	return (0);
}
