#include "../includes/nm_otool.h"

t_attr	*init_array_attributes(void)
{
	//printf("[%s]\n", __func__);
	t_attr *attr;

	if (!(attr = malloc(sizeof(t_attr))))
		return (NULL);
	attr->t = 0;
	attr->files = NULL;
	return (attr);
}

int		search_letter(char *str, char c)
{
	//printf("[%s]\n", __func__);
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
	//printf("[%s]\n", __func__);
	t_attr	*attr;
	int		ret;

	ret = 0;
	if (!argv || !*argv)
		ret = err_otool(ERR_USAGE, "invalid number of attributes");
	else if (!*argv || *argv[0] != '-' || !check_lines(argv))
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
	if (argv && *argv && ((attr->files = argv)))
		return (attr);
	err_otool(ERR_USAGE, "invalid number of attributes");
	free(attr);
	return (NULL);
}