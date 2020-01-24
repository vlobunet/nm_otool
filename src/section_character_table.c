#include "../includes/nm_otool.h"

char		char_table(const size_t offset)
{
	static char		ch[256];
	static uint8_t	s;
	char			*name;

	if (offset == 0 && !(s = 0))
		ft_bzero(ch, 256);
	else if (FIRST_BIT_ON_64 & offset)
		return (ch[offset & 0xff]);
	else
	{
		if (!(name = get_struct(offset, 16)))
			return (err(ERR_FILE, "bad section name offset"));
		if (!ft_strncmp(name, "__text", 6))
			ch[++s] = 't';
		else if (!ft_strncmp(name, "__data", 6))
			ch[++s] = 'd';
		else if (!ft_strncmp(name, "__bss", 5))
			ch[++s] = 'b';
		else
			ch[++s] = 's';
	}
	return ('1');
}

int check_type(char c)
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

	if (!(seg = get_struct(start_offset, sizeof(*seg))))
		return (err(ERR_FILE, "bad segment offset"));
	offset = start_offset + sizeof(*seg);
	if (!(sect = get_struct(offset, sizeof(*sect))))
		return (err(ERR_FILE, "bad section offset"));
	nsects = get_4b(seg->nsects);
	while (nsects--)
	{
		if (!char_table(offset))
			return (err(ERR_SYS, __func__));
		offset += sizeof(*sect);
		if (!(sect = get_struct(offset, sizeof(*sect))))
			return (err(ERR_FILE, "bad section offset"));
	}
	return (0);
}