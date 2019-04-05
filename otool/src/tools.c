/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 16:02:06 by jandreu           #+#    #+#             */
/*   Updated: 2019/03/31 16:02:09 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/otool.h"

void			ft_print_addr(char *addr, uint8_t type, uint8_t *endptr)
{
	uint16_t	padding;
	uint16_t	addr_len;
	int			i;

	padding = type == TYPE_OBJECT_64 ? 16 : 8;
	addr_len = ft_strlen(addr);
	i = -1;
	if (addr_len >= 1 && addr[0] != '0')
	{
		while (i++ < (padding - addr_len) - 1)
			ft_putchar('0');
		ft_secure_putstr(addr, endptr, 0);
	}
	else
	{
		while (i++ < padding - 1)
			ft_putchar('0');
	}
}

int				ft_secure_putstr(char *to_print, uint8_t *end_file, uint8_t nl)
{
	while ((uint8_t *)to_print != end_file && *to_print != '\0')
	{
		ft_putchar(*to_print);
		to_print++;
	}
	if (nl == 1)
		ft_putchar('\n');
	if ((uint8_t *)to_print >= end_file)
		return (0);
	return (1);
}

void			set_type_and_swap(t_file *file, int type, void *file_ptr)
{
	file->type = type;
	file->endian = get_elem_endianness(file_ptr);
	file->offset = 0;
}

int				check_types(void *ptr, t_file *file)
{
	if (*(uint32_t *)ptr == MH_MAGIC || *(uint32_t *)ptr == MH_CIGAM)
	{
		set_type_and_swap(file, TYPE_OBJECT, ptr);
		handle_mach_o_32(file, ptr);
	}
	else if (*(uint32_t *)ptr == MH_MAGIC_64 || *(uint32_t *)ptr == MH_CIGAM_64)
	{
		set_type_and_swap(file, TYPE_OBJECT_64, ptr);
		handle_mach_o_64(file, ptr);
	}
	else if (*(uint32_t *)ptr == FAT_MAGIC || *(uint32_t *)ptr == FAT_CIGAM)
	{
		set_type_and_swap(file, TYPE_FAT, ptr);
		handle_fat32(ptr, file);
	}
	else if (ft_memcmp(ptr, ARMAG, SARMAG) == 0)
	{
		set_type_and_swap(file, TYPE_ARCHIVE, ptr);
		handle_archive(file, ptr);
	}
	else
		return (0);
	return (1);
}

void			check_type_header(void *ptr, t_file *file)
{
	if (file->size < sizeof(uint32_t))
		return ;
	if (check_types(ptr, file) == 0)
	{
		ft_putstr(file->name);
		ft_putstr(": is not an object file");
	}
}
