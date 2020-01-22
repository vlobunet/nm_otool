#include "../includes/nm_otool.h"

static void push(t_sym *symbol, t_sym **lst)
{
	if (*lst && symbol)
	{
		symbol->p = *lst;
		(*lst)->n = symbol;
	}
	*lst = symbol;
}

char	get_type_64(const uint64_t n_value, struct nlist_64 *nlist_64, const uint16_t n_desc)
{
	char		type = '?';

	(N_STAB & nlist_64->n_type) ? type = '-' : 0;
	if ((N_TYPE & nlist_64->n_type) == N_UNDF)
		type = n_value ? 'c' : 'u';
	((N_TYPE & nlist_64->n_type) == N_ABS) ? type = 'a' : 0;
	if ((N_TYPE & nlist_64->n_type) == N_SECT &&
		!(type = sections_character_table(FIRST_BIT_ON_64 |
			nlist_64->n_sect)))
		type = '?';
	((N_TYPE & nlist_64->n_type) == N_PBUD) ? type = 'u' : 0;
	((N_TYPE & nlist_64->n_type) == N_INDR) ? type = 'i' : 0;
	(n_desc & N_WEAK_REF) ? type = 'W' : 0;
	(N_EXT & nlist_64->n_type) ? type = ft_toupper(type) : 0;
	return (type);
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
	m.i = 0;
	while (m.i < m.nsyms)
	{
		m.value = get_8b(m.nlist_64[m.i].n_value);
		push_nlist_64((struct nlist_64*)&m.nlist_64[m.i], m.value, m.sym_cmd);
		m.i++;
	}
	return (0);
}

int main_parser_64(t_cmanager ptr_func, uint32_t type)
{
	t_manager m;

	m.ofset = sizeof(struct mach_header_64);
	if (!(m.hdr_64 = get_struct(0, sizeof(*(m.hdr_64)))))
		return (err(ERR_SYS, __func__));
	m.ncmds = get_4b(m.hdr_64->ncmds);
	//print_hdr_info(m.hdr_64->magic, m.hdr_64->filetype);
	if (!(m.lc = get_struct(m.ofset, sizeof(*(m.lc)))))
		return (err(ERR_SYS, __func__));
	while (m.ncmds--)
	{
		m.cmd_type = get_4b(m.lc->cmd);
		//print_load_command_sector(m.cmd_type);
		if (m.cmd_type == type)
			ptr_func(m.ofset);
		m.ofset = m.ofset + get_4b(m.lc->cmdsize);
		if (!(m.lc = get_struct(m.ofset, sizeof(*(m.lc)))))
			return (err(ERR_SYS, __func__));
	}
	return (0);
}
