/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_manager_x86.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 19:46:22 by vlobunet          #+#    #+#             */
/*   Updated: 2020/01/27 19:46:24 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/nm_otool.h"

int	otool_print_sector_x86(size_t start_offset)
{
	struct section	*sect;
	char			*text;

	if (!(sect = get_struct(start_offset, sizeof(*sect))))
		return (err(ERR_FILE, "bad offset section"));
	if (!(text = get_struct(get_4b(sect->addr), get_4b(sect->size))))
		return (err(ERR_FILE, "bad ofset text"));
	print_text_section(get_4b(sect->size), get_4b(sect->addr), text);
	return (1);
}

int	segment_manager_x86(size_t start_offset)
{
	struct segment_command		*seg_x86;
	struct section				*sec_x86;
	size_t						off;
	uint32_t					n;

	if (!(seg_x86 = get_struct(start_offset, sizeof(*seg_x86))))
		return (err(ERR_FILE, "bad segment offset"));
	off = start_offset + sizeof(*seg_x86);
	if (!(sec_x86 = get_struct(off, sizeof(*sec_x86))))
		return (err(ERR_FILE, "bad section offset"));
	n = get_4b(seg_x86->nsects);
	while (n--)
	{
		if ((!ft_strncmp(sec_x86->sectname, "__text", 16))
		&& (!ft_strncmp(sec_x86->segname, "__TEXT", 16))
		&& !otool_print_sector_x86(off))
			return (err(ERR_FILE, __func__));
		off += sizeof(*sec_x86);
		if (!(sec_x86 = get_struct(off, sizeof(*sec_x86))))
			return (err(ERR_FILE, "bad section offset"));
	}
	return (1);
}
