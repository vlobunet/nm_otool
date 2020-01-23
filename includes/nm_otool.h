#ifndef _NM_OTOOL_H_
# define _NM_OTOOL_H_

# include "../libft/includes/libft.h"
# include "defines.h"
# include "struct.h"
# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <mach-o/nlist.h>


#include <stdio.h>

void		print_load_command_sector(uint32_t c);
void		print_hdr_info(uint32_t hdr_magic, uint32_t hdr_filetype);
void		p_m(char *type, char *src);

int			check_architecture(void);
t_attr		*check_atributes(char **argv);
int check_lines(char **argv, uint8_t is_nm);
int			err(const int err, const char *str);
int			err_otool(const int err, const char *str);
int mmap_file (int argc, char **argv, uint8_t is_nm);
int			munmap_file(t_save_file f);

int			check_symbol_type(char c);
char		sections_character_table(const size_t offset);

int			main_parser_86(t_cmanager ptr_func, uint32_t type);
int			symtab_manager_86(size_t ofset);
void		push_nlist_86(struct nlist *nlist, unsigned long value,
	struct symtab_command *symc);
char		get_type_86(const uint64_t n_value, struct nlist *nlist,
	const uint16_t n_desc);

int			main_parser_64(t_cmanager ptr_func, uint32_t type);
int			symtab_manager_64(size_t ofset);
void		push_nlist_64(struct nlist_64 *nlist_64, uint64_t value,
	struct symtab_command *symc);
char		get_type_64(const uint64_t n_value, struct nlist_64 *nlist_64,
	const uint16_t n_desc);

int			segment_command_manager(size_t start_offset);

uint16_t	get_2b(uint16_t n);
uint32_t	get_4b(uint32_t n);
uint64_t	get_8b(uint64_t n);
void		*get_struct(const uint64_t offset, const size_t size);

t_attr		*init_array_attributes(void);
int			search_letter(char *str, char c);

void		lst_sort(void);
void		sorted_symbol(t_sym *min, t_sym *cur, t_sym *sorted, int f);
void		print_lst(t_sym *sort);

int segment_manager(size_t start_offset);

int segment_manager_x86(size_t start_offset);
int otool_print_sector_x86(size_t start_offset);
int segment_manager_x64(size_t start_offset);
int otool_print_sector_64(size_t start_offset);

void print_text_section(uint64_t size, uint64_t offset, char *text);

#endif