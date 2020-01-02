#include "../includes/nm_otool.h"

int	err(const int err, const char *str)
{
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

int mmap_file (int argc, char **argv, t_save_file *f)
{
	int			fd;
	struct stat	buf;
	void		*p;


	if (argc > 2)
		return (err(ERR_USAGE, "erroneous input."));
	if ((fd = open((argc == 1 ? "a.out" : argv[1]), O_RDONLY)) == -1)
		return (err(ERR_FILE, "No such file or directory"));
	if (fstat(fd, &buf) < 0 || buf.st_mode & S_IFDIR)
		return (err(ERR_FILE, "you can only use MACH-O files."));
	if ((p = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (err(ERR_SYS, "mmap: unable to map file to memory"));
	close(fd);
	f->p = p;
	f->size = buf.st_size;
	f->ofset = 0;
	return (0);
}

int munmap_file(t_save_file f)
{
	if (munmap(f.p, f.size))
		return (err(ERR_SYS, "munmap: unable to free file displayed in memory"));
	return (0);
}

void *safe(const uint64_t offset, const size_t size, t_save_file *f)
{
	if (offset + size < offset)
		return (NULL);
	return ((void *)((size_t)(f->p + f->ofset + offset) * \
		(f->ofset + offset + size <= f->size)));
}

int check_architecture(t_save_file file)
{
	uint32_t *magic = NULL;

	if (!(magic = safe(0, sizeof(*magic), &file)))
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

uint32_t			endian(uint32_t n, int is_64)
{
	if (!is_64)
		return ((n >> 24) | ((n & 0xff0000) >> 8) | \
			((n & 0xff00) << 8) | (n << 24));
	return (n);
}

int print_segment_command(size_t ofset, t_save_file f)
{
	struct segment_command *seg_cmd;
	unsigned long cmdsize;
	char *segname;

	seg_cmd = safe(ofset, sizeof(*seg_cmd), &f);
	if (!seg_cmd)
		return (err(ERR_SYS, __func__));
	cmdsize = endian(seg_cmd->cmdsize, f.is_64);
	printf("cmdsize:\t%ld\n", cmdsize);
	segname = ft_strndup(seg_cmd->segname, 16);
	printf("segname:\t%s\n", segname);
	return (0);
}


int	nm_symbol_allocate(t_sym_sort *syms, const uint64_t nsyms)
{
	ft_bzero(syms, sizeof(*syms));
	if (nsyms)
	{
		syms->symbols = ft_memalloc(nsyms * sizeof(t_strsymbol));
		if (!syms->symbols)
			return (err(ERR_SYS, "malloc failed"));
		syms->symbols_sort = ft_memalloc(nsyms * sizeof(void *));
		if (!syms->symbols_sort)
			return (err(ERR_SYS, "malloc failed"));
	}
	return (0);
}


int print_symtab_command(size_t ofset, t_save_file f)
{
	struct symtab_command	*sym_cmd;
	uint32_t				nsyms;
	uint32_t				i;
	struct nlist			*nlist;
	struct nlist_64			*nlist_64;
	uint32_t				symoff;
	uint8_t					n_type;
	t_sym_sort syms;
	unsigned long value;

	i = 0;

	printf("SYMTAB_COMMAND\n");
	sym_cmd = safe(ofset, sizeof(*sym_cmd), &f);
	if (!sym_cmd)
		return (err(ERR_SYS, __func__));
	nsyms = endian(sym_cmd->nsyms, f.is_64);
	if (!nsyms)
		return (err(ERR_SYS, __func__));
	printf("nsyms:\t%d\n", nsyms);
	symoff = endian(sym_cmd->symoff, f.is_64);
	if (!f.is_64)
	{
		if (!(nlist = safe(symoff, sizeof(*nlist) * nsyms, &f)))
			return (err(ERR_SYS, "bad offset or size (symbol table)."));
		n_type = endian(nlist->n_type, f.is_64);
	}
	else
	{
		if (!(nlist_64 = safe(symoff, sizeof(*nlist_64) * nsyms, &f)))
			return (err(ERR_SYS, "bad offset or size (symbol table)."));
		n_type = endian(nlist_64->n_type, f.is_64);
	}
	if (!safe(symoff, endian(sym_cmd->strsize, f.is_64), &f))
		return (err(ERR_SYS, __func__));
	if (nm_symbol_allocate(&syms, nsyms) == -1)
		return(-1);
	n_type == N_STAB ? printf("n_type:\tN_STAB\n") : 0;
	n_type == N_PEXT ? printf("n_type:\tN_PEXT\n") : 0;
	n_type == N_TYPE ? printf("n_type:\tN_TYPE") : 0;
	n_type == N_EXT ? printf("n_type:\tN_EXT\n") : 0;
	if (n_type == N_TYPE)
	{
		n_type == N_UNDF ? printf(": N_UNDF (%#X)\n", n_type) : 0;
		n_type == N_ABS ? printf(": N_ABS (%#X)\n", n_type) : 0;
		n_type == N_SECT ? printf(": N_SECT (%#X)\n", n_type) : 0;
		n_type == N_PBUD ? printf(": N_PBUD (%#X)\n", n_type) : 0;
		n_type == N_INDR ? printf(": N_INDR (%#X)\n", n_type) : 0;
	}
	while (i < nsyms)
	{
		value = (!f.is_64 ? endian(nlist[i].n_value, f.is_64) :
			endian(nlist_64[i].n_value, f.is_64));
		printf("%#lXs\n", value);

		i++;
	}
	return (0);
}

int main_parser(t_save_file f)
{
	size_t ofset;
	struct mach_header		*hdr;
	struct mach_header_64	*hdr_64;
	struct load_command		*lc;
	uint32_t				ncmds;
	uint32_t				target;
	uint32_t				cmd_type;


	ofset = (!f.is_64 ? sizeof(struct mach_header) : sizeof(struct mach_header_64));

	if (!f.is_64)
	{
		hdr = safe(0, sizeof(*hdr), &f);
		if (!hdr)
			return (err(ERR_SYS, __func__));
		ncmds = endian(hdr->ncmds, f.is_64);
		target = LC_SEGMENT;
		print_hdr_info(hdr->magic, hdr->filetype, f.is_64);
	}
	else
	{
		hdr_64 = safe(0, sizeof(*hdr_64), &f);
		if (!hdr_64)
			return (err(ERR_SYS, __func__));
		ncmds = endian(hdr_64->ncmds, f.is_64);
		target = LC_SEGMENT_64;
		print_hdr_info(hdr_64->magic, hdr_64->filetype, f.is_64);
	}
	printf("\n____________________ LC_SEGMENT_X ______________________\n\n");
	lc = safe(ofset, sizeof(*lc), &f);
	if (!lc)
		return (err(ERR_SYS, __func__));
	while (ncmds--)
	{
		cmd_type = endian(lc->cmd, f.is_64);
		print_load_command_sector(cmd_type);
		if (cmd_type == target)
			print_segment_command(ofset, f);
		if (cmd_type == LC_SYMTAB)
			print_symtab_command(ofset, f);
		ofset = ofset + endian(lc->cmdsize, f.is_64);
		lc = safe(ofset, sizeof(*lc), &f);
		if (!hdr)
			return (err(ERR_SYS, __func__));
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_save_file f;

	if (mmap_file(argc, argv, &f))
		return(1);
	if ((f.is_64 = check_architecture(f)) == -1)
		return (1);
	main_parser(f);
	if (munmap_file(f))
		return (1);
	return (0);
}
