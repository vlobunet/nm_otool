#ifndef _DEFINES_H_
# define _DEFINES_H_

# include "nm_otool.h"

# define BLACK      "\033[1;30m"
# define RED        "\033[1;31m"
# define GREEN      "\033[01;38;05;46m"
# define YELLOW     "\033[01;38;05;226m"
# define BLUE       "\033[03;38;05;21m"
# define VIOLET     "\033[01;38;05;201m"
# define CYAN       "\033[1;36m"
# define WHITE      "\033[01;38;05;15m"
# define RES		"\033[m"

# define ERR_NUMBER 3
# define ERR_SYS	0
# define ERR_USAGE	1
# define ERR_FILE	2

# define STDERR		2
# define STDOUT		1
# define STDIN		0

# define NM	1
# define OTOOL 0

#define MH_MAGIC		0xfeedface    /* the mach magic number */
#define MH_CIGAM		0xcefaedfe    /* NXSwapInt(MH_MAGIC) */
#define MH_MAGIC_64		0xfeedfacf    /* the 64-bit mach magic number */
#define MH_CIGAM_64		0xcffaedfe    /* NXSwapInt(MH_MAGIC_64) */
#define FAT_MAGIC		0xcafebabe    /* the fat magic number */
#define FAT_CIGAM		0xbebafeca    /* NXSwapLong(FAT_MAGIC) */
#define FAT_MAGIC_64	0xcafebabf    /* the 64-bit fat magic number */
#define FAT_CIGAM_64	0xbfbafeca    /* NXSwapLong(FAT_MAGIC_64) */

# define ARCHIVE_CIGAM		0x72613C21
# define ARCHIVE_MAGIC		0x213C6172
# define FIRST_BIT_ON_64	0x8000000000000000L

#define N_STAB	0xe0            /* if any of these bits set, a symbolic debugging entry */
#define N_PEXT	0x10            /* private external symbol bit */
#define N_TYPE	0x0e            /* mask for the type bits */
#define N_EXT	0x01            /* external symbol bit, set for external symbols */

#define N_UNDF	0x0             /* undefined, n_sect == NO_SECT */
#define N_ABS	0x2             /* absolute, n_sect == NO_SECT */
#define N_SECT	0xe             /* defined in section number n_sect */
#define N_PBUD	0xc             /* prebound undefined (defined in a dylib) */
#define N_INDR	0xa             /* indirect */

# define OTOOL_SECTION		"__text"
# define OTOOL_SEGMENT		"__TEXT"

# define M_MH_OBJECT "MH_OBJECT (Relocatable object file)"
# define M_MH_EXECUTE "MH_EXECUTE (Demand paged executable file)"
# define M_MH_FVMLIB "MH_FVMLIB (Fixed VM shared library file)"
# define M_MH_CORE "MH_CORE (Core file)"
# define M_MH_PRELOAD "MH_PRELOAD (Preloaded executable file)"
# define M_MH_DYLIB "MH_DYLIB (Dynamically bound shared library)"
# define M_MH_DYLINKER "MH_DYLINKER (Dynamic link editor)"
# define M_MH_BUNDLE "MH_BUNDLE (Dynamically bound bundle file)"
# define M_MH_DYLIB_STUB "MH_DYLIB_STUB (Shared library stub for static link)"
# define M_MH_DSYM "MH_DSYM (Companion file with only debug sections)"
# define M_MH_KEXT_BUNDLE "MH_KEXT_BUNDLE (x86_64 kexts)"

# define M_MH_MAGIC "MH_MAGIC"
# define M_MH_MAGIC_64 "MH_MAGIC_64"
# define M_MH_UNKNOWN "unknown magic"

# define M_LC_SEGMENT "(segment of this file to be mapped)"
# define M_LC_SYMTAB "(link-edit stab symbol table info)"
# define M_LC_SYMSEG "(link-edit gdb symbol table info (obsolete))"
# define M_LC_THREAD "(thread)"
# define M_LC_UNIXTHREAD "(unix thread (includes a stack))"
# define M_LC_LOADFVMLIB "(load a specified fixed VM shared library)"
# define M_LC_IDFVMLIB "(fixed VM shared library identification)"
# define M_LC_IDENT "(object identification info (obsolete))"
# define M_LC_FVMFILE "(fixed VM file inclusion (internal use))"
# define M_LC_PREPAGE "(prepage command (internal use))"
# define M_LC_DYSYMTAB "(dynamic link-edit symbol table info)"
# define M_LC_LOAD_DYLIB "(load a dynamically linked shared library)"
# define M_LC_ID_DYLIB "(dynamically linked shared lib ident)"
# define M_LC_LOAD_DYLINKER "(load a dynamic linker)"
# define M_LC_ID_DYLINKER "(dynamic linker identification)"
# define M_LC_PREBOUND_DYLIB "(modules prebound for a dynamically link library)"
# define M_LC_ROUTINES "(image routines)"
# define M_LC_SUB_FRAMEWORK "(sub framework)"
# define M_LC_SUB_UMBRELLA "(sub umbrella)"
# define M_LC_SUB_CLIENT "(sub client)"
# define M_LC_SUB_LIBRARY "(sub library)"
# define M_LC_TWOLEVEL_HINTS "(two-level namespace lookup hints)"
# define M_LC_PREBIND_CKSUM "(prebind checksum)"
# define M_LC_LOAD_WEAK_DYLIB "()"
# define M_LC_SEGMENT_64 "(64-bit segment of this file to be mapped)"
# define M_LC_ROUTINES_64 "(64-bit image routines)"
# define M_LC_UUID "(the uuid)"
# define M_LC_RPATH "(runpath additions)"
# define M_LC_CODE_SIGNATURE "(local of code signature)"
# define M_LC_SEGMENT_SPLIT_INFO "(local of info to split segments)"
# define M_LC_REEXPORT_DYLIB "(load and re-export dylib like environment var)"
# define M_LC_MAIN "(replacement for LC_UNIXTHREAD)"
# define M_LC_DATA_IN_CODE "(table of non-instructions in __text)"
# define M_LC_SOURCE_VERSION "(source version used to build binary)"
# define M_LC_DYLIB_CODE_SIGN_DRS "(Code signing DRs copied from linked dylibs)"
# define M_LC_ENCRYPTION_INFO_64 "(64-bit encrypted segment information)"
# define M_LC_LINKER_OPTION "(linker options in MH_OBJECT files)"
# define M_LC_LINKER_OPTIMIZATION_HINT "(optimization hints in MH_OBJECT files)"
# define M_LC_VERSION_MIN_TVOS "(build for AppleTV min OS version)"
# define M_LC_VERSION_MIN_WATCHOS "(build for Watch min OS version)"
# define M_LC_NOTE "(arbitrary data included within a Mach-O file)"
# define M_LC_BUILD_VERSION "(build for platform min OS version)"

#endif