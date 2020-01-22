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
	if (!g_f.attributes->g || (c >= 'A' && c <= 'Z'))
	{
		if (!g_f.attributes->U && !g_f.attributes->T &&
			!g_f.attributes->B && !g_f.attributes->S &&
			!g_f.attributes->D)
			return (1);
		if ((ft_tolower(c) == 'u' && !g_f.attributes->U) ||
			(ft_tolower(c) == 't' && !g_f.attributes->T) ||
			(ft_tolower(c) == 'b' && !g_f.attributes->B) ||
			(ft_tolower(c) == 's' && !g_f.attributes->S) ||
			(ft_tolower(c) == 'd' && !g_f.attributes->D))
			return (0);
		else
			return (1);
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
		&& !sections_character_table(offset))
			return (err(ERR_SYS, __func__));
		offset += sizeof(*sect);
		if (!(sect = get_struct(offset, sizeof(*sect))))
			return (err(ERR_FILE, "bad section offset"));
	}
	return (0);
}