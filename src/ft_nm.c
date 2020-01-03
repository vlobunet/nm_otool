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

int mmap_file (int argc, char **argv)
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
	g_f.p = p;
	g_f.size = buf.st_size;
	g_f.ofset = 0;
	return (0);
}

int munmap_file(t_save_file f)
{
	if (munmap(f.p, f.size))
		return (err(ERR_SYS, "munmap: unable to free file displayed in memory"));
	return (0);
}

void *safe(const uint64_t offset, const size_t size)
{
	if (offset + size < offset)
		return (NULL);
	return ((void *)((size_t)(g_f.p + g_f.ofset + offset) * \
		(g_f.ofset + offset + size <= g_f.size)));
}

int check_architecture(void)
{
	uint32_t *magic = NULL;

	if (!(magic = safe(0, sizeof(*magic))))
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

int print_segment_command(size_t ofset)
{
	struct segment_command *seg_cmd;
	unsigned long cmdsize;
	char *segname;

	seg_cmd = safe(ofset, sizeof(*seg_cmd));
	if (!seg_cmd)
		return (err(ERR_SYS, __func__));
	cmdsize = get_4b(seg_cmd->cmdsize);
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

char		nm_sections_character_table(const size_t offset)
{
	static char				sections_character[256];
	static uint8_t			sections = 0;
	char					*sectname;
	if (offset == 0)
	{
		printf("HERE_1\n");
		ft_bzero(sections_character, 256);
		sections = 0;
	}
	else if (FIRST_BIT_ON_64 & offset)
	{
		printf("HERE_2\n");
		return (sections_character[offset & 0xff]);
	}
	else
	{
		printf("HERE_3\n");
		if (!(sectname = safe(offset, 16)))
			return (err(ERR_FILE, "bad section name offset"));
		if (!ft_strncmp(sectname, "__text", 6))
			sections_character[++sections] = 't';
		else if (!ft_strncmp(sectname, "__data", 6))
			sections_character[++sections] = 'd';
		else if (!ft_strncmp(sectname, "__bss", 5))
			sections_character[++sections] = 'b';
		else
			sections_character[++sections] = 's';
	}
	return ('1');
}

static char	get_type(const uint64_t n_value, struct nlist *nlist, const uint16_t n_desc)
{
	char		type = '?';

	(N_STAB & nlist->n_type) ? type = '-' : 0;
	if ((N_TYPE & nlist->n_type) == N_UNDF)
		type = n_value ? 'c' : 'u';
	((N_TYPE & nlist->n_type) == N_ABS) ? type = 'a' : 0;
	if ((N_TYPE & nlist->n_type) == N_SECT && !(type = nm_sections_character_table(FIRST_BIT_ON_64 | nlist->n_sect)))
		type = '?';
	((N_TYPE & nlist->n_type) == N_PBUD) ? type = 'u' : 0;
	((N_TYPE & nlist->n_type) == N_INDR) ? type = 'i' : 0;
	(n_desc & N_WEAK_REF) ? type = 'W' : 0;
	(N_EXT & nlist->n_type) ? type = ft_toupper(type) : 0;
	return (type);
}

static char	get_type_64(const uint64_t n_value, struct nlist_64 *nlist_64, const uint16_t n_desc)
{
	char		type = '?';

	(N_STAB & nlist_64->n_type) ? type = '-' : 0;
	if ((N_TYPE & nlist_64->n_type) == N_UNDF)
		type = n_value ? 'c' : 'u';
	((N_TYPE & nlist_64->n_type) == N_ABS) ? type = 'a' : 0;
	if ((N_TYPE & nlist_64->n_type) == N_SECT && !(type = nm_sections_character_table(FIRST_BIT_ON_64 | nlist_64->n_sect)))
		type = '?';
	((N_TYPE & nlist_64->n_type) == N_PBUD) ? type = 'u' : 0;
	((N_TYPE & nlist_64->n_type) == N_INDR) ? type = 'i' : 0;
	(n_desc & N_WEAK_REF) ? type = 'W' : 0;
	(N_EXT & nlist_64->n_type) ? type = ft_toupper(type) : 0;
	return (type);
}

void push_nlist(struct nlist *nlist, unsigned long value, struct symtab_command *symc/*, t_sym_sort *syms*/)
{
	uint32_t off;
	uint32_t size;
	uint32_t stroff;
	t_sym symbol;

	off = get_4b(symc->stroff);
	size = get_4b(symc->strsize);
	stroff = off + get_4b(nlist->n_un.n_strx);

	symbol.size = off + size - stroff;
	symbol.str = safe(stroff, symbol.size);
	symbol.type = get_type(value, nlist, get_2b(nlist->n_desc));
	symbol.off = value;
	if (stroff < off || stroff > off + size)
		symbol.str = (char *)"bad string index";
	printf("%#lX\t[typpe]\t%s\n", value, symbol.str);
}

void push_nlist_64(struct nlist_64 *nlist_64, unsigned long value, struct symtab_command *symc/*, t_sym_sort *sym*/)
{
	uint32_t	off;
	uint32_t	size;
	uint32_t	stroff;
	t_sym		symbol;

	off = get_4b(symc->stroff);
	size = get_4b(symc->strsize);
	stroff = off + get_4b(nlist_64->n_un.n_strx);

	symbol.size = off + size - stroff;
	symbol.str = safe(stroff, symbol.size);
	symbol.type = get_type_64(value, nlist_64, get_2b(nlist_64->n_desc));
	symbol.off = value;
	if (stroff < off || stroff > off + size)
		symbol.str = (char *)"bad string index";
	printf("%#lX %c %s\n", value, symbol.type, symbol.str);
}

int print_symtab_command(size_t ofset)
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

	if (!(sym_cmd = safe(ofset, sizeof(*sym_cmd))))
		return (err(ERR_SYS, __func__));
	if (!(nsyms = get_4b(sym_cmd->nsyms)))
		return (err(ERR_SYS, __func__));
	printf("nsyms:\t%d\n", nsyms);
	symoff = get_4b(sym_cmd->symoff);
	if (!g_f.is_64)
	{
		if (!(nlist = safe(symoff, sizeof(*nlist) * nsyms)))
			return (err(ERR_SYS, "bad offset or size (symbol table)."));
		n_type = get_4b(nlist->n_type);
	}
	else
	{
		if (!(nlist_64 = safe(symoff, sizeof(*nlist_64) * nsyms)))
			return (err(ERR_SYS, "bad offset or size (symbol table)."));
		n_type = get_4b(nlist_64->n_type);
	}
	if (!safe(symoff, get_4b(sym_cmd->strsize)))
		return (err(ERR_SYS, __func__));
	if (nm_symbol_allocate(&syms, nsyms) == -1)
		return(-1);
	i = 0;
	while (i < nsyms)
	{
		value = (!g_f.is_64 ? get_8b(nlist[i].n_value) :
			get_8b(nlist_64[i].n_value));
		!g_f.is_64 ? push_nlist((struct nlist*)&nlist[i], value, sym_cmd/*, &syms*/) :
		push_nlist_64((struct nlist_64*)&nlist_64[i], value, sym_cmd/*, &syms*/);
		i++;
	}
	return (0);
}


int main_parser(void)
{
	size_t ofset;
	struct mach_header		*hdr;
	struct mach_header_64	*hdr_64;
	struct load_command		*lc;
	uint32_t				ncmds;
	uint32_t				cmd_type;


	ofset = (!g_f.is_64 ? sizeof(struct mach_header) : sizeof(struct mach_header_64));
	if (!g_f.is_64)
	{
		if (!(hdr = safe(0, sizeof(*hdr))))
			return (err(ERR_SYS, __func__));
		ncmds = get_4b(hdr->ncmds);
		print_hdr_info(hdr->magic, hdr->filetype);
	}
	else
	{
		if (!(hdr_64 = safe(0, sizeof(*hdr_64))))
			return (err(ERR_SYS, __func__));
		ncmds = get_4b(hdr_64->ncmds);
		print_hdr_info(hdr_64->magic, hdr_64->filetype);
	}

	if (!(lc = safe(ofset, sizeof(*lc))))
		return (err(ERR_SYS, __func__));
	while (ncmds--)
	{
		cmd_type = get_4b(lc->cmd);
		print_load_command_sector(cmd_type);
		if (cmd_type == (g_f.is_64 ? LC_SEGMENT_64 : LC_SEGMENT))
			print_segment_command(ofset);
		if (cmd_type == LC_SYMTAB)
			print_symtab_command(ofset);
		ofset = ofset + get_4b(lc->cmdsize);
		if (!(lc = safe(ofset, sizeof(*lc))))
			return (err(ERR_SYS, __func__));
	}
	return (0);
}

int main(int argc, char **argv)
{
	if (mmap_file(argc, argv))
		return(1);
	if ((g_f.is_64 = check_architecture()) == -1)
		return (1);
	main_parser();
	if (munmap_file(g_f))
		return (1);
	return (0);
}
