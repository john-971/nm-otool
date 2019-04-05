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

#include "../include/otool.h"

void				print_addr_32(uint32_t addr, t_file *file)
{
	char addr_str[16];

	ft_itoa_hex(addr, 0, addr_str);
	ft_print_addr(addr_str, file->type, file->end_file);
	ft_putstr("\t");
}

void				print_content(uint8_t *txt, uint32_t addr, t_file *file, \
int i)
{
	char hex[16];

	ft_itoa_hex(*txt, 0, hex);
	if (ft_strlen(hex) == 1)
		ft_putchar('0');
	ft_putstr(hex);
	if (file->endian == ENDIAN_LITTLE)
		ft_putchar(' ');
	else if (i % 4 == 0)
		ft_putchar(' ');
	if (i > 0 && i % 16 == 0 && i < file->ofile_size)
	{
		ft_putchar('\n');
		print_addr_32(addr, file);
	}
}

void				process_32(int addr, void *txt, t_file *file)
{
	int				i;

	i = 0;
	while (i++ < file->ofile_size)
	{
		addr++;
		print_content(txt, addr, file, i);
		txt++;
	}
}

void				handle_section_32(t_lc *lc, t_file *file, t_mh *mh)
{
	int				j;
	t_sc			*segment;
	struct section	*section;
	uint8_t			*txt;
	int				addr;

	j = -1;
	segment = (t_sc *)lc;
	section = (struct section *)((uint8_t *)segment + sizeof(t_sc));
	while (++j < (int)get_32(segment->nsects, file))
	{
		if (ft_strcmp((section + j)->sectname, SECT_TEXT) == 0 && \
		ft_strcmp((section + j)->segname, SEG_TEXT) == 0)
		{
			ft_putendl("Contents of (__TEXT,__text) section");
			file->ofile_size = get_32((section + j)->size, file);
			if (file->ofile_size == 0)
				return ;
			txt = (uint8_t *)mh + get_32((section + j)->offset, file);
			addr = get_32((section + j)->addr, file);
			print_addr_32(addr, file);
			process_32(addr, txt, file);
			return ;
		}
	}
}

void				handle_mach_o_32(t_file *file, void *file_ptr)
{
	t_mh			*mach_h;
	t_lc			*lc;
	int				i;

	mach_h = (t_mh *)(uint8_t *)file_ptr;
	lc = (t_lc*)(file_ptr + sizeof(*mach_h));
	i = -1;
	while (++i < (int)get_32(mach_h->ncmds, file))
	{
		file->offset += get_32(lc->cmdsize, file);
		if (get_32(lc->cmd, file) == LC_SEGMENT)
			handle_section_32(lc, file, mach_h);
		if (file->offset < file->size)
			lc = (t_lc *)((uint8_t *)lc + get_32(lc->cmdsize, file));
	}
}
