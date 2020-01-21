#include "../includes/nm_otool.h"

int print_segment_command(size_t ofset)
{
	struct segment_command *seg_cmd;
	unsigned long cmdsize;
	char *segname;

	seg_cmd = get_struct(ofset, sizeof(*seg_cmd));
	if (!seg_cmd)
		return (err(ERR_SYS, __func__));
	cmdsize = get_4b(seg_cmd->cmdsize);
	printf("cmdsize:\t%ld\n", cmdsize);
	segname = ft_strndup(seg_cmd->segname, 16);
	printf("segname:\t%s\n", segname);
	return (0);
}

char		nm_sections_character_table(const size_t offset)
{
	static char				sections_character[256];
	static uint8_t			sections = 0;
	char					*sectname;

	if (offset == 0)
	{
		ft_bzero(sections_character, 256);
		sections = 0;
	}
	else if (FIRST_BIT_ON_64 & offset)
	{
		return (sections_character[offset & 0xff]);
	}
	else
	{
		if (!(sectname = get_struct(offset, 16)))
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

void push(t_sym *symbol, t_sym **lst)
{
	if (*lst && symbol)
	{
		symbol->p = *lst;
		(*lst)->n = symbol;
	}
	*lst = symbol;
}

int check_symbol_type(char c)
{
	if ((ft_tolower(c) == 'u' && !g_f.attributes->U) ||
		(ft_tolower(c) == 't' && !g_f.attributes->T) ||
		(ft_tolower(c) == 'b' && !g_f.attributes->B) ||
		(ft_tolower(c) == 's' && !g_f.attributes->S) ||
		(ft_tolower(c) == 'd' && !g_f.attributes->D) ||
		(ft_tolower(c) == 'c' && !g_f.attributes->C) ||
		(ft_tolower(c) == 'i' && !g_f.attributes->I) ||
		(ft_tolower(c) == 'a' && !g_f.attributes->A))
		return (0);
	return (1);
}
void push_nlist(struct nlist *nlist, unsigned long value, struct symtab_command *symc)
{
	uint32_t off;
	uint32_t size;
	uint32_t stroff;
	t_sym *symbol;

	off = get_4b(symc->stroff);
	size = get_4b(symc->strsize);
	stroff = off + get_4b(nlist->n_un.n_strx);
	if (!check_symbol_type(get_type(value, nlist, get_2b(nlist->n_desc))))
		return ;
	symbol = (t_sym *)malloc(sizeof(t_sym));
	symbol->size = off + size - stroff;
	symbol->str = get_struct(stroff, symbol->size);
	symbol->type = get_type(value, nlist, get_2b(nlist->n_desc));
	symbol->off = value;
	symbol->n = NULL;
	symbol->p = NULL;
	if (stroff < off || stroff > off + size)
		symbol->str = (char *)"bad string index";
	push(symbol, &(g_f.lstsym));
}

void push_nlist_64(struct nlist_64 *nlist_64, uint64_t value, struct symtab_command *symc)
{
	uint32_t	off;
	uint32_t	size;
	uint32_t	stroff;
	t_sym		*symbol;

	symbol = (t_sym *)malloc(sizeof(t_sym));
	off = get_4b(symc->stroff);
	size = get_4b(symc->strsize);
	stroff = off + get_4b(nlist_64->n_un.n_strx);
	if (!check_symbol_type(get_type_64(value, nlist_64, get_2b(nlist_64->n_desc))))
		return ;
	symbol->size = off + size - stroff;
	symbol->str = get_struct(stroff, symbol->size);
	symbol->type = get_type_64(value, nlist_64, get_2b(nlist_64->n_desc));
	symbol->off = value;
	symbol->n = NULL;
	symbol->p = NULL;
	if (stroff < off || stroff > off + size)
		symbol->str = (char *)"bad string index";
	push(symbol, &(g_f.lstsym));
}

int symtab_manager(size_t ofset)
{
	t_cymmanager m;

	if (!(m.sym_cmd = get_struct(ofset, sizeof(*(m.sym_cmd)))))
		return (err(ERR_SYS, __func__));
	if (!(m.nsyms = get_4b(m.sym_cmd->nsyms)))
		return (err(ERR_SYS, __func__));
	m.symoff = get_4b(m.sym_cmd->symoff);
	if (!(m.nlist = get_struct(m.symoff, sizeof(*(m.nlist)) * m.nsyms)))
		return (err(ERR_SYS, "bad offset or size (symbol table)."));
	m.n_type = get_4b(m.nlist->n_type);
	if (!get_struct(m.symoff, get_4b(m.sym_cmd->strsize)))
		return (err(ERR_SYS, __func__));
	// if (nm_symbol_allocate(&m.syms, m.nsyms) == -1)
	// 	return(-1);
	m.i = 0;
	while (m.i < m.nsyms)
	{
		m.value = get_8b(m.nlist[m.i].n_value);
		push_nlist((struct nlist*)&m.nlist[m.i], m.value, m.sym_cmd);
		m.i++;
	}
	return (0);
}

int symtab_manager_64(size_t ofset)
{
	t_cymmanager m;

	if (!(m.sym_cmd = get_struct(ofset, sizeof(*(m.sym_cmd)))))
		return (err(ERR_SYS, __func__));
	if (!(m.nsyms = get_4b(m.sym_cmd->nsyms)))
		return (err(ERR_SYS, __func__));
	m.symoff = get_4b(m.sym_cmd->symoff);
	if (!(m.nlist_64 = get_struct(m.symoff, sizeof(*(m.nlist_64)) * m.nsyms)))
		return (err(ERR_SYS, "bad offset or size (symbol table)."));
	m.n_type = get_4b(m.nlist_64->n_type);
	if (!get_struct(m.symoff, get_4b(m.sym_cmd->strsize)))
		return (err(ERR_SYS, __func__));
	// if (nm_symbol_allocate(&m.syms, m.nsyms) == -1)
	// 	return(-1);
	m.i = 0;
	while (m.i < m.nsyms)
	{
		m.value = get_8b(m.nlist_64[m.i].n_value);
		push_nlist_64((struct nlist_64*)&m.nlist_64[m.i], m.value, m.sym_cmd);
		m.i++;
	}
	return (0);
}

int segment_manager(size_t start_offset)
{
	struct segment_command_64	*seg;
	struct section_64			*sect;
	size_t						offset;
	uint32_t					nsects;
	const char					*target_segment = NULL;
	const char					*target_section = NULL;

	if (!(seg = get_struct(start_offset, sizeof(*seg))))
		return (err(ERR_FILE, "bad segment offset"));
	offset = start_offset + sizeof(*seg);
	if (!(sect = get_struct(offset, sizeof(*sect))))
		return (err(ERR_FILE, "bad section offset"));
	nsects = get_4b(seg->nsects);
	while (nsects--)
	{
		if ((!target_section || !ft_strncmp(sect->sectname, target_section, 16))
		&& (!target_segment || !ft_strncmp(sect->segname, target_segment, 16))
		&& !nm_sections_character_table(offset))
			return (err(ERR_SYS, __func__));
		offset += sizeof(*sect);
		if (!(sect = get_struct(offset, sizeof(*sect))))
			return (err(ERR_FILE, "bad section offset"));
	}
	return (0);
}

static int main_parser(t_cmanager ptr_func, uint32_t type)
{
	t_manager m;

	m.ofset = sizeof(struct mach_header);
	if (!(m.hdr = get_struct(0, sizeof(*(m.hdr)))))
		return (err(ERR_SYS, __func__));
	m.ncmds = get_4b(m.hdr->ncmds);
	print_hdr_info(m.hdr->magic, m.hdr->filetype);
	if (!(m.lc = get_struct(m.ofset, sizeof(*(m.lc)))))
		return (err(ERR_SYS, __func__));
	while (m.ncmds--)
	{
		m.cmd_type = get_4b(m.lc->cmd);
		if (m.cmd_type == type)
			ptr_func(m.ofset);
		m.ofset = m.ofset + get_4b(m.lc->cmdsize);
		if (!(m.lc = get_struct(m.ofset, sizeof(*m.lc))))
			return (err(ERR_SYS, __func__));
	}
	return (0);
}

static int main_parser_64(t_cmanager ptr_func, uint32_t type)
{
	t_manager m;

	m.ofset = sizeof(struct mach_header_64);
	if (!(m.hdr_64 = get_struct(0, sizeof(*(m.hdr_64)))))
		return (err(ERR_SYS, __func__));
	m.ncmds = get_4b(m.hdr_64->ncmds);
	if (!(m.lc = get_struct(m.ofset, sizeof(*(m.lc)))))
		return (err(ERR_SYS, __func__));
	while (m.ncmds--)
	{
		m.cmd_type = get_4b(m.lc->cmd);
		if (m.cmd_type == type)
			ptr_func(m.ofset);
		m.ofset = m.ofset + get_4b(m.lc->cmdsize);
		if (!(m.lc = get_struct(m.ofset, sizeof(*(m.lc)))))
			return (err(ERR_SYS, __func__));
	}
	return (0);
}

void set_symbol(t_sym **cur)
{
	if ((*cur)->p)
		(*cur)->p->n = (*cur)->n;
	if ((*cur)->n)
		(*cur)->n->p = (*cur)->p;
	(*cur)->p = NULL;
	(*cur)->n = NULL;
}

void print_lst(t_sym *sort)
{
	while (sort)
	{

		!sort->off ? printf("                 ") :
					printf("0000000%llX ", sort->off);
		printf("%c ", sort->type);
		printf("%s\n", sort->str);
		sort = sort->n;
	}
}

void go_start_lst(void)
{
	while (g_f.lstsym->p)
		g_f.lstsym = g_f.lstsym->p;
}

void sorted_symbol(t_sym *min, t_sym *cur, t_sym *sorted, int f)
{
	while (cur)
	{
		while (cur)
		{
			if ((f == 1 && min->off > cur->off) ||
				(f == 2 && min->type > cur->type) ||
				(f == 3 && ft_strcmp(min->str, cur->str) > 0))
				min = cur;
			cur = cur->n;
		}
		if (min == g_f.lstsym)
			g_f.lstsym = g_f.lstsym->n;
		set_symbol(&min);
		push(min, &sorted);
		cur = g_f.lstsym;
		min = cur;
	}
	while (sorted->p)
		sorted = sorted->p;
	print_lst(sorted);
}

void lst_sort(void)
{
	while (g_f.lstsym->p)
		g_f.lstsym = g_f.lstsym->p;

	if (g_f.attributes->n && g_f.lstsym)
		sorted_symbol(g_f.lstsym, g_f.lstsym->n, NULL, 1);
	else if (g_f.attributes->t && g_f.lstsym)
		sorted_symbol(g_f.lstsym, g_f.lstsym->n, NULL, 2);
	else if (g_f.attributes->a && g_f.lstsym)
		sorted_symbol(g_f.lstsym, g_f.lstsym->n, NULL, 3);
	else
		print_lst(g_f.lstsym);
}

int main(int argc, char **argv)
{
	static t_cmanager func_ptr[3];

	func_ptr[0] = &segment_manager;
	func_ptr[1] = &symtab_manager;
	func_ptr[2] = &symtab_manager_64;
	if (mmap_file(argc, argv))
		return(1);
	if ((g_f.is_64 = check_architecture()) == -1)
		return (1);
	g_f.is_64 ? main_parser_64(func_ptr[0], LC_SEGMENT_64) :
	main_parser(func_ptr[0], LC_SEGMENT);

	g_f.is_64 ? main_parser_64(func_ptr[2], LC_SYMTAB) :
	main_parser(func_ptr[1], LC_SYMTAB);
	lst_sort();
	//lst_print();
	if (munmap_file(g_f))
		return (1);
	return (0);
}
