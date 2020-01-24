#include "../includes/nm_otool.h"

t_attr	*init_array_attributes(void)
{
	t_attr	*attr;

	if (!(attr = (t_attr *)malloc(sizeof(t_attr))))
		return (NULL);
	attr->U = 0;
	attr->T = 0;
	attr->B = 0;
	attr->S = 0;
	attr->D = 0;
	attr->C = 0;
	attr->I = 0;
	attr->A = 0;
	attr->a = 0;
	attr->n = 0;
	attr->t = 0;
	attr->p = 0;
	attr->f = NULL;
	return (attr);
}

int		search_letter(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (1);
		else
			i++;
	}
	return (0);
}

void	print_other(void)
{
	ft_putstr_fd("\t-U -- Display symbols only U/u type.\n", STDERR);
	ft_putstr_fd("\t-T -- Display symbols only T/t type.\n", STDERR);
	ft_putstr_fd("\t-B -- Display symbols only B/b type.\n", STDERR);
	ft_putstr_fd("\t-S -- Display symbols only S/s type.\n", STDERR);
	ft_putstr_fd("\t-D -- Display symbols only D/d type.\n", STDERR);
	ft_putstr_fd("\t-C -- Display symbols only C/c type.\n", STDERR);
	ft_putstr_fd("\t-A -- Display symbols only A/a type.\n", STDERR);
	ft_putstr_fd("\t-g -- Display only global symbol\n", STDERR);
	ft_putstr_fd("\t-a -- Sort by name (alphabet)(default)\n", STDERR);
	ft_putstr_fd("\t-n -- Sort by offset\n", STDERR);
	ft_putstr_fd("\t-t -- Sort by type\n", STDERR);
	ft_putstr_fd("\t-p -- Display as in the symbol table (no sort)\n", STDERR);
}

int		err(const int err, const char *str)
{
	static const char	*msg[ERR_NUMBER] =
	{
		"Fatal Error: ",
		"Bad usage: ",
		"Bad file: ",
	};

	ft_putstr_fd(RED, STDERR);
	ft_putstr_fd(msg[err], STDERR);
	ft_putstr_fd(RES, STDERR);
	ft_putstr_fd(str, STDERR);
	ft_putstr_fd("\n", STDERR);
	if (err == ERR_USAGE)
	{
		ft_putstr_fd("usage: ./ft_nm [-UTBSDCIAgantp] <files> \n", STDERR);
		print_other();
	}
	return (1);
}
