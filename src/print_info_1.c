#include "../includes/nm_otool.h"

void p_m(char *type, char *src)
{
	ft_putstr_fd(WHITE, STDOUT);
	ft_putstr_fd(type, STDOUT);
	ft_putstr_fd(RES, STDOUT);
	ft_putstr_fd(": ", STDOUT);
	ft_putendl_fd(src, STDOUT);
}

void print_hdr_info(uint32_t hdr_magic, uint32_t hdr_filetype)
{
	uint32_t magic;
	uint32_t filetype;

	printf("____________________ HEDADER ____________________\n\n");
	magic = get_4b(hdr_magic);
	if (magic == MH_MAGIC)
		p_m("MAGIC", M_MH_MAGIC);
	else if (magic == MH_MAGIC_64)
		p_m("MAGIC", M_MH_MAGIC_64);
	else
		p_m("MAGIC", M_MH_UNKNOWN);
	filetype = get_4b(hdr_filetype);
	filetype == MH_OBJECT ? p_m("FILETYPE", M_MH_OBJECT) : 0;
	filetype == MH_EXECUTE ? p_m("FILETYPE", M_MH_EXECUTE) : 0;
	filetype == MH_FVMLIB ? p_m("FILETYPE", M_MH_FVMLIB) : 0;
	filetype == MH_CORE	 ? p_m("FILETYPE", M_MH_CORE) : 0;
	filetype == MH_PRELOAD ? p_m("FILETYPE", M_MH_PRELOAD) : 0;
	filetype == MH_DYLIB ? p_m("FILETYPE", M_MH_DYLIB) : 0;
	filetype == MH_DYLINKER ? p_m("FILETYPE", M_MH_DYLINKER) : 0;
	filetype == MH_BUNDLE ? p_m("FILETYPE", M_MH_BUNDLE) : 0;
	filetype == MH_DYLIB_STUB ? p_m("FILETYPE", M_MH_DYLIB_STUB) : 0;
	filetype == MH_DSYM	 ? p_m("FILETYPE", M_MH_DSYM) : 0;
	filetype == MH_KEXT_BUNDLE ? p_m("FILETYPE", M_MH_KEXT_BUNDLE) : 0;
}

void part_one(uint32_t c)
{
	c == LC_SEGMENT ? p_m("LC_SEGMENT", M_LC_SEGMENT) : 0;
	c == LC_SYMTAB ? p_m("LC_SYMTAB", M_LC_SYMTAB) : 0;
	c == LC_SYMSEG ? p_m("LC_SYMSEG", M_LC_SYMSEG) : 0;
	c == LC_THREAD ? p_m("LC_THREAD", M_LC_THREAD) : 0;
	c == LC_UNIXTHREAD ? p_m("LC_UNIXTHREAD", M_LC_UNIXTHREAD) : 0;
	c == LC_LOADFVMLIB ? p_m("LC_LOADFVMLIB", M_LC_LOADFVMLIB) : 0;
	c == LC_IDFVMLIB ? p_m("LC_IDFVMLIB", M_LC_IDFVMLIB) : 0;
	c == LC_IDENT ? p_m("LC_IDENT", M_LC_IDENT) : 0;
	c == LC_FVMFILE ? p_m("LC_FVMFILE", M_LC_FVMFILE) : 0;
	c == LC_PREPAGE ? p_m("LC_PREPAGE", M_LC_PREPAGE) : 0;
	c == LC_DYSYMTAB ? p_m("LC_DYSYMTAB", M_LC_DYSYMTAB) : 0;
	c == LC_LOAD_DYLIB ? p_m("LC_LOAD_DYLIB", M_LC_LOAD_DYLIB) : 0;
	c == LC_ID_DYLIB ? p_m("LC_ID_DYLIB", M_LC_ID_DYLIB) : 0;
	c == LC_LOAD_DYLINKER ? p_m("LC_LOAD_DYLINKER", M_LC_LOAD_DYLINKER) : 0;
	c == LC_ID_DYLINKER ? p_m("LC_ID_DYLINKER", M_LC_ID_DYLINKER) : 0;
	c == LC_PREBOUND_DYLIB ? p_m("LC_PREBOUND_DYLIB", M_LC_PREBOUND_DYLIB) : 0;
	c == LC_ROUTINES ? p_m("LC_ROUTINES", M_LC_ROUTINES) : 0;	
	c == LC_SUB_FRAMEWORK ? p_m("LC_SUB_FRAMEWORK", M_LC_SUB_FRAMEWORK) : 0;
	c == LC_SUB_UMBRELLA ? p_m("LC_SUB_UMBRELLA", M_LC_SUB_UMBRELLA) : 0;
	c == LC_SUB_CLIENT ? p_m("LC_SUB_CLIENT", M_LC_SUB_CLIENT) : 0;
	c == LC_SUB_LIBRARY ? p_m("LC_SUB_LIBRARY", M_LC_SUB_LIBRARY) : 0;
	c == LC_TWOLEVEL_HINTS ? p_m("LC_TWOLEVEL_HINTS", M_LC_TWOLEVEL_HINTS) : 0;
	c == LC_PREBIND_CKSUM ? p_m("LC_PREBIND_CKSUM", M_LC_PREBIND_CKSUM) : 0;
	c == LC_LOAD_WEAK_DYLIB ? p_m("LC_LOAD_WEAK_DYLIB", M_LC_LOAD_WEAK_DYLIB) : 0;
	c == LC_SEGMENT_64 ? p_m("LC_SEGMENT_64", M_LC_SEGMENT_64) : 0;
}

void print_load_command_sector(uint32_t c)
{
	part_one(c);
	c == LC_ROUTINES_64 ? p_m("LC_ROUTINES_64", M_LC_ROUTINES_64) : 0;
	c == LC_UUID ? p_m("LC_UUID", M_LC_UUID) : 0;
	c == LC_RPATH ? p_m("LC_RPATH", M_LC_RPATH) : 0;
	c == LC_MAIN ? p_m("LC_MAIN", M_LC_MAIN) : 0;
	c == LC_DATA_IN_CODE ? p_m("LC_DATA_IN_CODE", M_LC_DATA_IN_CODE) : 0;
	c == LC_NOTE ? p_m("LC_NOTE", M_LC_NOTE) : 0;
	c == LC_CODE_SIGNATURE ? p_m("LC_CODE_SIGNATURE",
		M_LC_CODE_SIGNATURE) : 0;
	c == LC_SEGMENT_SPLIT_INFO ? p_m("LC_SEGMENT_SPLIT_INFO",
		M_LC_SEGMENT_SPLIT_INFO) : 0;
	c == LC_REEXPORT_DYLIB ? p_m("LC_REEXPORT_DYLIB", M_LC_REEXPORT_DYLIB) : 0;
	c == LC_SOURCE_VERSION ? p_m("LC_SOURCE_VERSION", M_LC_SOURCE_VERSION) : 0;
	c == LC_DYLIB_CODE_SIGN_DRS ? p_m("LC_DYLIB_CODE_SIGN_DRS",
		M_LC_DYLIB_CODE_SIGN_DRS) : 0;
	c == LC_ENCRYPTION_INFO_64 ? p_m("LC_ENCRYPTION_INFO_64",
		M_LC_ENCRYPTION_INFO_64) : 0;
	c == LC_LINKER_OPTION ? p_m("LC_LINKER_OPTION", M_LC_LINKER_OPTION) : 0;
	c == LC_LINKER_OPTIMIZATION_HINT ? p_m("LC_LINKER_OPTIMIZATION_HINT",
		M_LC_LINKER_OPTIMIZATION_HINT) : 0;
	c == LC_VERSION_MIN_TVOS ? p_m("LC_VERSION_MIN_TVOS",
		M_LC_VERSION_MIN_TVOS) : 0;
	c == LC_VERSION_MIN_WATCHOS ? p_m("LC_VERSION_MIN_WATCHOS",
		M_LC_VERSION_MIN_WATCHOS) : 0;
	c == LC_BUILD_VERSION ? p_m("LC_BUILD_VERSION", M_LC_BUILD_VERSION) : 0;
}