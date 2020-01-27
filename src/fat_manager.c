#include "../includes/nm_otool.h"

int		known_magic_retriever_86(uint32_t nfat_arch, size_t offset, size_t *target_offset, uint32_t *magic)
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

int		known_magic_retriever_64(uint32_t nfat_arch, size_t offset, size_t *target_offset, uint32_t *magic)
{
	struct fat_arch			*arch;
	uint32_t				*magic_ptr;

	// loop through architectures looking for known magic
	while (nfat_arch--)
	{
		//retrieve safe pointers
		if (!(arch = safe(offset, sizeof(*arch))))
			return (errors(ERR_FILE, "bad fat arch offset"));
		if (!(magic_ptr = safe(endian_4(arch->offset), sizeof(*magic_ptr))))
			return (errors(ERR_FILE, "bad fat arch magic offset"));

		// check for known magic
		if (*magic_ptr == MH_CIGAM_64 || *magic_ptr == MH_MAGIC_64)
			*target_offset = endian_4(arch->offset);
		else if (!(*target_offset) && (*magic_ptr == MH_CIGAM || \
			*magic_ptr == MH_MAGIC))
			*target_offset = endian_4(arch->offset);
		offset += sizeof(*arch);
	}
	if (!(*magic = *(uint32_t*)safe(*target_offset, sizeof(*magic))))
		return (errors(ERR_FILE, "bad fat arch magic offset"));
	return (true);
}