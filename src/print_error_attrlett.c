#include "../includes/nm_otool.h"

int	err_otool(const int err, const char *str)
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
		ft_putstr_fd("usage: ./ft_otool -t [fname..]\n", STDERR);
	}
	return (1);
}
		
void print_error_atribit_letter(char let)
{
	//printf("[%s]\n", __func__);
	ft_putstr_fd(RED, STDERR);
	ft_putstr_fd("Bad usage attributes: ", STDERR);
	ft_putstr_fd(RES, STDERR);
	ft_putchar_fd(let, STDERR);
	ft_putchar_fd('\n', STDERR);
}

int		control_attr(char *str, char *valid_letter, int i, int j)
{
	//printf("[%s]\n", __func__);
	int ret;

	if (!str || !*str)
		return (0);
	while (str[i])
	{
		j = 0;
		ret = 0;
		while (valid_letter[j])
		{
			if (str[i] == valid_letter[j])
				ret++;
			j++;
		}
		if (!ret)
			return (0);
		i++;
	}
	return (1);
}

int check_word(char *word)
{
	//printf("%s\n", __func__);
	int i;

	i = 1;
	if (!word || !*word)
		return (0);
	while (word[i])
		if (ft_isalpha(word[i]))
			i++;
		else
			return (0);
	return (1);
}



int check_lines(char **argv, uint8_t is_nm)
{
	//printf("%s\n", __func__);
	while (argv && *argv && *argv[0] == '-')
	{
		if (!check_word(*argv) || !control_attr(*argv, (is_nm ? "UTBSDCIAgantp" : "t"), 1, 0))
		{
			is_nm ? err(ERR_USAGE, *argv) : err_otool(ERR_USAGE, *argv);
			return (0);
		}
		argv++;
	}
	return (1);
}