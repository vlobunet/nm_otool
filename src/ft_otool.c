#include "../includes/nm_otool.h"

void print_normal_format(char *str)
{
	uint16_t len;
	len = ft_strlen(str);
	if (len >= 2)
	{
		ft_putchar_fd(str[len - 2], STDOUT);
		ft_putchar_fd(str[len - 1], STDOUT);
	}
	else if (len == 1)
	{
		ft_putchar_fd('0', STDOUT);
		ft_putchar_fd(str[0], STDOUT);
	}
	else
		ft_putstr_fd("00", STDOUT);
	ft_putchar_fd(' ', 1);
}

void print_text_section(uint64_t size, uint64_t offset, char *text)
{
	uint64_t i;
	uint64_t j;

	g_f.attributes->f ?  ft_putendl_fd(*g_f.attributes->f, STDOUT) :
	ft_putendl_fd("a.out", STDOUT);
	ft_putstr_fd("Contents of (__TEXT,__text) section\n", STDOUT);
	i = 0;
	while (i < size)
	{
		ft_putstr_fd(ft_itoabase(offset + i, 16), STDOUT);
		ft_putchar_fd('\t', STDOUT);
		j = 0;
		while (j < 0x10 && i + j < size)
			print_normal_format(ft_itoabase(text[i + j++], 16));
		ft_putchar_fd('\n', STDOUT);
		i += 0x10;
	}
}

int main(int argc, char **argv)
{
	static t_cmanager	func_ptr[2];

	func_ptr[0] = &segment_manager_x86;
	func_ptr[1] = &segment_manager_x64;

	if (mmap_file(argc, argv, OTOOL))
		return(1);
	if ((g_f.is_64 = check_architecture()) == -1)
		return (1);
	g_f.is_64 ? main_parser_64(func_ptr[1], LC_SEGMENT_64) :
	main_parser_86(func_ptr[0], LC_SEGMENT);
	if (munmap_file(g_f))
		return (1);
	return (0);
}
