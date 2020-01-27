/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_manager_x64.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 19:46:13 by vlobunet          #+#    #+#             */
/*   Updated: 2020/01/27 19:46:14 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/nm_otool.h"

int		otool_print_sector_64(size_t start_offset)
{
	struct section_64	*sect_64;
	char				*text;

	if (!(sect_64 = get_struct(start_offset, sizeof(*sect_64))))
		return (err(ERR_FILE, "bad offset section"));
	if (!(text = get_struct(get_4b(sect_64->addr), get_8b(sect_64->size))))
		return (err(ERR_FILE, "bad ofset text"));
	print_text_section(get_8b(sect_64->size), get_8b(sect_64->addr), text);
	return (1);
}

int		segment_manager_x64(size_t start_offset)
{
	struct segment_command_64	*seg_x64;
	struct section_64			*sect_64;
	size_t						off;
	uint32_t					n;

	if (!(seg_x64 = get_struct(start_offset, sizeof(*seg_x64))))
		return (err(ERR_FILE, "bad segment offset"));
	off = start_offset + sizeof(*seg_x64);
	if (!(sect_64 = get_struct(off, sizeof(*sect_64))))
		return (err(ERR_FILE, "bad section offset"));
	n = get_4b(seg_x64->nsects);
	while (n--)
	{
		if ((!ft_strncmp(sect_64->sectname, "__text", 16))
		&& (!ft_strncmp(sect_64->segname, "__TEXT", 16))
		&& !otool_print_sector_64(off))
			return (err(ERR_FILE, __func__));
		off += sizeof(*sect_64);
		if (!(sect_64 = get_struct(off, sizeof(*sect_64))))
			return (err(ERR_FILE, "bad section offset"));
	}
	return (1);
}
