/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach-o_32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 16:01:01 by jandreu           #+#    #+#             */
/*   Updated: 2019/03/31 16:01:03 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/nm.h"

void				handle_section_32(t_lc *lc, t_ofile *ofile, t_file *file)
{
	int				j;
	t_sc			*segment;
	struct section	*section;

	j = 0;
	segment = (t_sc *)lc;
	section = (struct section *)((uint8_t *)segment + sizeof(t_sc));
	while (j < (int)get_32(segment->nsects, file))
	{
		if (ft_strcmp((section + j)->sectname, SECT_TEXT) == 0 && \
		ft_strcmp((section + j)->segname, SEG_TEXT) == 0)
			ofile->sect_text = ofile->sect_index + 1;
		else if (ft_strcmp((section + j)->sectname, SECT_DATA) == 0 && \
		ft_strcmp((section + j)->segname, SEG_DATA) == 0)
			ofile->sect_data = ofile->sect_index + 1;
		else if (ft_strcmp((section + j)->sectname, SECT_BSS) == 0 && \
		ft_strcmp((section + j)->segname, SEG_DATA) == 0)
			ofile->sect_bss = ofile->sect_index + 1;
		ofile->sect_index++;
		j++;
	}
}

void				handle_mach_o_32(t_file *file, void *file_ptr)
{
	t_mh			*mach_h;
	t_lc			*lc;
	int				i;
	t_ofile			*ofile;

	ofile = (t_ofile *)malloc(sizeof(t_ofile));
	if (ofile == NULL)
		ft_putendl("ERROR MALLOC");
	ft_bzero(ofile, sizeof(t_ofile));
	ofile->type = file->type;
	mach_h = (t_mh *)(uint8_t *)file_ptr;
	lc = (t_lc*)(file_ptr + sizeof(*mach_h));
	i = -1;
	while (++i < (int)get_32(mach_h->ncmds, file))
	{
		file->offset += get_32(lc->cmdsize, file);
		if (get_32(lc->cmd, file) == LC_SEGMENT)
			handle_section_32(lc, ofile, file);
		else if (get_32(lc->cmd, file) == LC_SYMTAB)
			print_32(file_ptr, (struct symtab_command *)lc, ofile, file);
		if (file->offset < file->size)
			lc = (t_lc *)((uint8_t *)lc + get_32(lc->cmdsize, file));
	}
	free(ofile);
}
