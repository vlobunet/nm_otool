#ifndef _STRUCT_H_
# define _STRUCT_H_

# include "nm_otool.h"
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <mach-o/nlist.h>

typedef struct				s_sym
{
	uint64_t				off;
	char					*str;
	uint32_t				size;
	char					type;
	struct s_sym			*n;
	struct s_sym			*p;
}							t_sym;

typedef struct				s_attr
{
	uint8_t					U;	// nm | U symbol type
	uint8_t					T;	// nm | T/t symbol type
	uint8_t					B;	// nm | B/b symbol type
	uint8_t					S;	// nm | S/s symbol type
	uint8_t					D;	// nm | D/d symbol type
	uint8_t					C;
	uint8_t					I;
	uint8_t					A;
	uint8_t					g; // nm | only global symbol
	uint8_t					n;	// nm | sort by numeric
	uint8_t					t;	// nm | sort by type
	uint8_t					a;	// nm | sort by alpha
	uint8_t					p;	// mn | sort off

	char					**f;
}							t_attr;

typedef struct				s_save_file
{
	int						is_64;
	void					*p;
	struct mach_header		hdr;
	struct mach_header_64	hdr_64;
	struct load_command		lc;
	size_t					size;
	size_t					ofset;
	t_attr					*attributes;
	t_sym					*lstsym;

}							t_save_file;

typedef struct				s_strsymbol
{
	uint64_t				offset;
	char					*string;
	uint32_t				str_max_size;
	char					type;
}							t_strsymbol;

typedef struct				s_sym_sort
{
	t_strsymbol				*symbols;
	size_t					nsyms_sort;
}							t_sym_sort;

t_save_file					g_f;

typedef struct				s_manager
{
	size_t					ofset;
	struct mach_header		*hdr;
	struct mach_header_64	*hdr_64;
	struct load_command		*lc;
	uint32_t				ncmds;
	uint32_t				cmd_type;
}							t_manager;

typedef struct				s_cymmanager
{
	struct symtab_command	*sym_cmd;
	uint32_t				nsyms;
	uint32_t				i;
	struct nlist			*nlist;
	struct nlist_64			*nlist_64;
	uint32_t				symoff;
	uint8_t					n_type;
	t_sym_sort				syms;
	uint64_t				value;
}							t_cymmanager;

typedef void	(*t_lc)(const size_t);
typedef int		(*t_cmanager)(size_t start_offset);

#endif