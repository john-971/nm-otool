/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach-o_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 16:01:12 by jandreu           #+#    #+#             */
/*   Updated: 2019/03/31 16:01:14 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/otool.h"

void				print_addr_64(uint64_t addr, t_file *file)
{
	char			addr_str[16];

	if (file->addr_add)
		ft_itoa_hex(addr + 0x100000000, 0, addr_str);
	else
		ft_itoa_hex(addr, 0, addr_str);
	ft_print_addr(addr_str, file->type, file->end_file);
	ft_putstr("\t");
}

void				print_content_64(uint8_t *txt, uint64_t addr, t_file *file,\
int i)
{
	char			hex[16];

	ft_itoa_hex(*txt, 0, hex);
	if (ft_strlen(hex) == 1)
		ft_putchar('0');
	if (file->endian == ENDIAN_LITTLE)
	{
		ft_putstr(hex);
		ft_putchar(' ');
	}
	else
	{
		ft_putstr(hex);
		if (i % 4 == 0)
			ft_putchar(' ');
	}
	if (i > 0 && i % 16 == 0 && i < file->ofile_size)
	{
		ft_putchar('\n');
		print_addr_64(addr, file);
	}
}

void				process_64(int addr, void *txt, t_file *file)
{
	int				i;

	i = 0;
	while (i++ < file->ofile_size)
	{
		addr++;
		print_content_64(txt, addr, file, i);
		txt++;
	}
}

void				handle_sect_64(t_lc *lc, t_file *file, t_mh64 *mh)
{
	int				j;
	t_sc64			*segment;
	t_sect64		*section;

	j = -1;
	segment = (struct segment_command_64 *)lc;
	section = (struct section_64 *)((uint8_t *)segment + sizeof(t_sc64));
	while (++j < (int)get_32(segment->nsects, file))
	{
		if (ft_strcmp((section + j)->sectname, SECT_TEXT) == 0 && \
		ft_strcmp((section + j)->segname, SEG_TEXT) == 0)
		{
			ft_putendl("Contents of (__TEXT,__text) section");
			file->ofile_size = get_uint64((section + j)->size, file);
			if (file->ofile_size == 0)
				return ;
			if (segment->vmaddr == 0x100000000)
				file->addr_add = 1;
			file->addr = get_uint64((section + j)->addr, file);
			print_addr_64(file->addr, file);
			process_64(file->addr, (uint8_t *)mh + get_32((section + j)->offset\
			, file), file);
			return ;
		}
	}
}

void				handle_mach_o_64(t_file *file, void *file_ptr)
{
	t_mh64			*mach_h;
	t_lc			*lc;
	int				i;

	mach_h = (t_mh64 *)(uint8_t *)file_ptr;
	lc = (t_lc *)(file_ptr + sizeof(*mach_h));
	i = -1;
	while (i++ < (int)get_32(mach_h->ncmds, file) - 1)
	{
		file->offset += get_32(lc->cmdsize, file);
		if (get_32(lc->cmd, file) == LC_SEGMENT_64)
			handle_sect_64(lc, file, mach_h);
		if (file->offset < file->size)
			lc = (t_lc *)((uint8_t *)lc + get_32(lc->cmdsize, file));
	}
}
