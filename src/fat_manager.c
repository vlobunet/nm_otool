/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlobunet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 19:40:09 by vlobunet          #+#    #+#             */
/*   Updated: 2020/02/04 19:40:11 by vlobunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/nm_otool.h"

int		fat_manager_86(uint32_t nfat_arch, size_t offset,
	size_t *target_offset, uint32_t *magic)
{
	struct fat_arch			*arch;
	uint32_t				*magic_ptr;

	while (nfat_arch--)
	{
		if (!(arch = get_struct(offset, sizeof(*arch))))
			return (err(ERR_FILE, "bad fat arch offset"));
		if (!(magic_ptr = get_struct(get_4b(arch->offset), sizeof(*magic_ptr))))
			return (err(ERR_FILE, "bad fat arch magic offset"));
		if (*magic_ptr == MH_CIGAM_64 || *magic_ptr == MH_MAGIC_64)
			*target_offset = get_4b(arch->offset);
		else if (!(*target_offset) && (*magic_ptr == MH_CIGAM ||
			*magic_ptr == MH_MAGIC))
			*target_offset = get_4b(arch->offset);
		offset += sizeof(*arch);
	}
	if (!(*magic = *(uint32_t*)get_struct(*target_offset, sizeof(*magic))))
		return (err(ERR_FILE, "bad fat arch magic offset"));
	return (1);
}

int		fat_manager_64(uint32_t nfat_arch, size_t offset,
	size_t *target_offset, uint32_t *magic)
{
	struct fat_arch			*arch;
	uint32_t				*magic_ptr;

	while (nfat_arch--)
	{
		if (!(arch = get_struct(offset, sizeof(*arch))))
			return (err(ERR_FILE, "bad fat arch offset"));
		if (!(magic_ptr = get_struct(get_4b(arch->offset), sizeof(*magic_ptr))))
			return (err(ERR_FILE, "bad fat arch magic offset"));
		if (*magic_ptr == MH_CIGAM_64 || *magic_ptr == MH_MAGIC_64)
			*target_offset = get_4b(arch->offset);
		else if (!(*target_offset) && (*magic_ptr == MH_CIGAM || \
			*magic_ptr == MH_MAGIC))
			*target_offset = get_4b(arch->offset);
		offset += sizeof(*arch);
	}
	if (!(*magic = *(uint32_t*)get_struct(*target_offset, sizeof(*magic))))
		return (err(ERR_FILE, "bad fat arch magic offset"));
	return (1);
}
