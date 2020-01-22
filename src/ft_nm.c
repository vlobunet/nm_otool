#include "../includes/nm_otool.h"

char		sections_character_table(const size_t offset)
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
		return (sections_character[offset & 0xff]);
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

int check_symbol_type(char c)
{
	if (!g_f.attributes->U && !g_f.attributes->T &&
		!g_f.attributes->B && !g_f.attributes->S &&
		!g_f.attributes->D && !g_f.attributes->C &&
		!g_f.attributes->I && !g_f.attributes->A)
		return (1);
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
		&& !sections_character_table(offset))
			return (err(ERR_SYS, __func__));
		offset += sizeof(*sect);
		if (!(sect = get_struct(offset, sizeof(*sect))))
			return (err(ERR_FILE, "bad section offset"));
	}
	return (0);
}

void print_lst(t_sym *sort)
{
	while (sort)
	{

		!sort->off ? printf("                 ") :
		printf(">> 0000000%llX ", sort->off);
		printf("%c ", sort->type);
		printf("%s\n", sort->str);
		sort = sort->n;
	}
}

int main(int argc, char **argv)
{
	static t_cmanager	func_ptr[3];

	func_ptr[0] = &segment_manager;
	func_ptr[1] = &symtab_manager_86;
	func_ptr[2] = &symtab_manager_64;
	if (mmap_file(argc, argv))
		return(1);
	if ((g_f.is_64 = check_architecture()) == -1)
		return (1);
	g_f.is_64 ? main_parser_64(func_ptr[0], LC_SEGMENT_64) :
	main_parser_86(func_ptr[0], LC_SEGMENT);
	g_f.is_64 ? main_parser_64(func_ptr[2], LC_SYMTAB) :
	main_parser_86(func_ptr[1], LC_SYMTAB);
	lst_sort();
	if (munmap_file(g_f))
		return (1);
	return (0);
}
