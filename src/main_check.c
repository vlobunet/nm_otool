#include "../includes/nm_otool.h"

int	err(const int err, const char *str)
{
	//printf("%s\n", __func__);
	static const char	*msg[ERR_NUMBER] =
	{
		"Fatal Error: ",
		"Bad usage: ",
		"Bad file: ",
	};

	ft_putstr_fd(RED, STDERR);
	ft_putstr_fd(msg[err], STDERR);
	ft_putstr_fd(RES, STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putstr_fd("\n", STDERR);
	if (err == ERR_USAGE)
		ft_putstr_fd("usage: ./ft_nm <fname> \n", STDERR);
	return (1);
}

t_attr *check_argv(char **argv)
{
	//printf("%s\n", __func__);
	t_attr *atr;

	if (argv && *argv && *argv[0] == '-' && !check_lines(argv, 0))
		return (NULL);
	atr = init_array_attributes();
	while (argv && *argv && *argv[0] == '-')
	{
		search_letter(*argv, 'U') ? atr->U = 1 : 0;
		search_letter(*argv, 'T') ? atr->T = 1 : 0;
		search_letter(*argv, 'B') ? atr->B = 1 : 0;
		search_letter(*argv, 'S') ? atr->S = 1 : 0;
		search_letter(*argv, 'D') ? atr->D = 1 : 0;
		search_letter(*argv, 'A') ? atr->A = 1 : 0;
		search_letter(*argv, 'I') ? atr->I = 1 : 0;
		search_letter(*argv, 'C') ? atr->C = 1 : 0;
		search_letter(*argv, 'a') ? atr->a = 1 : 0;
		search_letter(*argv, 't') ? atr->t = 1 : 0;
		search_letter(*argv, 'n') ? atr->n = 1 : 0;
		search_letter(*argv, 'p') ? atr->n = 1 : 0;
		argv = argv + 1;
	}
	if (argv && *argv)
		atr->f = argv;
	else
		atr->f = NULL;
	return (atr);
}

int mmap_file (int argc, char **argv)
{
	//printf("%s\n", __func__);
	int			fd;
	struct stat	buf;
	void		*p;
	t_attr *attributes = NULL;

	if (argc && !(attributes = check_argv(argv + 1)))
		return (1);
	if ((fd = open((!attributes->f ? "a.out" : *(attributes->f)), O_RDONLY)) == -1)
		return (err(ERR_FILE, "No such file or directory"));
	if (fstat(fd, &buf) < 0 || buf.st_mode & S_IFDIR)
		return (err(ERR_FILE, "you can only use MACH-O files."));
	if ((p = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (err(ERR_SYS, "mmap: unable to map file to memory"));
	close(fd);
	g_f.lstsym = NULL;
	g_f.attributes = attributes;
	g_f.p = p;
	g_f.size = buf.st_size;
	g_f.ofset = 0;
	return (0);
}

int munmap_file(t_save_file f)
{
	//printf("%s\n", __func__);
	if (munmap(f.p, f.size))
		return (err(ERR_SYS, "munmap: unable to free file displayed in memory"));
	return (0);
}

int check_architecture(void)
{
	//printf("%s\n", __func__);
	uint32_t *magic = NULL;

	if (!(magic = get_struct(0, sizeof(*magic))))
	{
		err(ERR_SYS, "unable magic");
		return(-1);
	}
	if (*magic == MH_CIGAM || *magic == MH_MAGIC)
		return (0);
	if (*magic == MH_CIGAM_64 || *magic == MH_MAGIC_64)
		return (1);
	err(ERR_FILE, "unknown format");
	return (-1);
}