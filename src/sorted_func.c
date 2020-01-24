#include "../includes/nm_otool.h"

void		set_symbol(t_sym **cur)
{
	if ((*cur)->p)
		(*cur)->p->n = (*cur)->n;
	if ((*cur)->n)
		(*cur)->n->p = (*cur)->p;
	(*cur)->p = NULL;
	(*cur)->n = NULL;
}

static void	push(t_sym *symbol, t_sym **lst)
{
	if (*lst && symbol)
	{
		symbol->p = *lst;
		(*lst)->n = symbol;
	}
	*lst = symbol;
}

void		lst_dell(t_sym *lst)
{
	t_sym *cur;

	while (lst && lst->n)
	{
		cur = lst;
		lst = lst->n;
		free(cur);
	}
	free(lst);
}

void		sorted_symbol(t_sym *min, t_sym *cur, t_sym *sorted, int f)
{
	while (cur)
	{
		while (cur)
		{
			if ((f == 1 && min->off > cur->off) ||
				(f == 2 && min->type > cur->type) ||
				(f == 3 && ft_strcmp(min->str, cur->str) > 0))
				min = cur;
			cur = cur->n;
		}
		if (min == g_f.lstsym)
			g_f.lstsym = g_f.lstsym->n;
		set_symbol(&min);
		push(min, &sorted);
		cur = g_f.lstsym;
		min = cur;
	}
	while (sorted && sorted->p)
		sorted = sorted->p;
	print_lst(sorted);
	lst_dell(sorted);
}

void		lst_sort(void)
{
	while (g_f.lstsym && g_f.lstsym->p)
		g_f.lstsym = g_f.lstsym->p;
	if (g_f.attributes->p && g_f.lstsym)
		print_lst(g_f.lstsym);
	else if (g_f.attributes->n && g_f.lstsym)
		sorted_symbol(g_f.lstsym, g_f.lstsym, NULL, 1);
	else if (g_f.attributes->t && g_f.lstsym)
		sorted_symbol(g_f.lstsym, g_f.lstsym, NULL, 2);
	else
		sorted_symbol(g_f.lstsym, g_f.lstsym, NULL, 3);
	lst_dell(g_f.lstsym);
	free(g_f.attributes);
}
