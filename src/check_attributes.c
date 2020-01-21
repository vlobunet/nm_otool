#include "../includes/nm_otool.h"

t_attr	*init_array_attributes(void)
{
	t_attr *attr;

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
	attr->f = NULL;
	return (attr);
}

int		search_letter(char *str, char c)
{
	int i;

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

t_attr	*check_atributes(char **argv)
{
	t_attr	*attr;
	int		ret;

	ret = 0;
	if (!argv || !*argv)
		ret = err_otool(ERR_USAGE, "invalid number of attributes");
	else if (!*argv || *argv[0] != '-' || !check_lines(argv, 1))
		ret = err_otool(ERR_USAGE, *argv);
	if (ret)
		return (NULL);
	attr = init_array_attributes();
	while (argv && *argv && *argv[0] == '-')
	{
		if (search_letter(*argv, 't'))
			attr->t = 1;
		argv = argv + 1;
	}
	if (argv && *argv && ((attr->f = argv)))
		return (attr);
	err_otool(ERR_USAGE, "invalid number of attributes");
	free(attr);
	return (NULL);
}