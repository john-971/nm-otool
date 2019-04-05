/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 16:01:48 by jandreu           #+#    #+#             */
/*   Updated: 2019/03/31 16:01:50 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/nm.h"

char		get_sect_sym(struct nlist *n_list, t_ofile *ofile)
{
	char	c;

	if (n_list->n_sect == ofile->sect_text)
		c = 't';
	else if (n_list->n_sect == ofile->sect_data)
		c = 'd';
	else if (n_list->n_sect == ofile->sect_bss)
		c = 'b';
	else
		c = 's';
	return (c);
}

char		get_sym_32(struct nlist *n_list, t_ofile *ofile)
{
	char	c;

	c = '?';
	if ((n_list->n_type & N_TYPE) == N_UNDF)
		c = n_list->n_value != 0 ? 'c' : 'u';
	else if ((n_list->n_type & N_TYPE) == N_ABS)
		c = 'a';
	else if ((n_list->n_type & N_TYPE) == N_SECT)
		c = get_sect_sym(n_list, ofile);
	else if ((n_list->n_type & N_TYPE) == N_PBUD)
		c = 'u';
	else if ((n_list->n_type & N_TYPE) == N_INDR)
		c = 'i';
	return ((n_list->n_type & N_EXT) ? ft_toupper(c) : c);
}

t_nl		*swap(t_nl *nlist, int i, int *swapped)
{
	t_nl	tmp;

	tmp = nlist[i + 1];
	nlist[i + 1] = nlist[i];
	nlist[i] = tmp;
	*swapped = 1;
	return (nlist);
}

t_nl		*sort_32(t_nl *nlist, void *st, int nbsyms, t_file *file)
{
	int		swapped;
	int		i;
	int		cmp_ret;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = -1;
		while (i++ < nbsyms && (i + 1) < nbsyms)
		{
			cmp_ret = ft_strcmp(st + \
			get_32(nlist[i].n_un.n_strx, file), st + \
			get_32(nlist[i + 1].n_un.n_strx, file));
			if (cmp_ret > 0)
				nlist = swap(nlist, i, &swapped);
			if (cmp_ret == 0)
				if (get_32(nlist[i].n_value, file) > \
				get_32(nlist[i + 1].n_value, file))
					nlist = swap(nlist, i, &swapped);
		}
	}
	return (nlist);
}

void		print_32(void *ptr, t_symc *sym, t_ofile *ofile, t_file *file)
{
	int		i;
	char	*stringtable;
	t_nl	*nliste;
	char	buff[16];

	nliste = (struct nlist *)(ptr + get_32(sym->symoff, file));
	stringtable = (void *)ptr + get_32(sym->stroff, file);
	nliste = sort_32(nliste, stringtable, get_32(sym->nsyms, file), file);
	i = 0;
	while (i < (int)get_32(sym->nsyms, file))
	{
		if (!(nliste->n_type & N_STAB))
		{
			ft_itoa_hex(get_32(nliste->n_value, file), 0, buff);
			ft_print_addr(buff, ofile->type, get_sym_32(nliste, ofile),\
			file->end_file);
			ft_putchar(' ');
			ft_putchar(get_sym_32(nliste, ofile));
			ft_putchar(' ');
			ft_secure_putstr(stringtable + get_32(nliste->n_un.n_strx, file),\
			file->end_file, 1);
		}
		nliste += 1;
		i++;
	}
}
