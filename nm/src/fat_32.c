/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 16:00:46 by jandreu           #+#    #+#             */
/*   Updated: 2019/03/31 16:00:51 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/nm.h"

int						farch(struct fat_arch *fat_hdr, uint32_t nb_arch,\
t_file *file)
{
	int					i;
	uint8_t				*ptr;

	i = -1;
	while (++i < (int)nb_arch)
	{
		if (get_32(fat_hdr[i].cputype, file) == CPU_TYPE_X86_64)
		{
			file->size = get_32(fat_hdr[i].size, file);
			ptr = (uint8_t *)(file->st_file + get_32(fat_hdr[i].offset, file));
			check_type_header(ptr, file);
			return (1);
		}
	}
	return (0);
}

void					print(int i, int nb_arch, t_file *file, \
struct fat_arch *fat_hdr)
{
	if (i > 0 || (nb_arch > 1 && i == 0))
		ft_putchar('\n');
	if (nb_arch > 1)
	{
		ft_putstr(file->name);
		if (get_32(fat_hdr[i].cputype, file) == CPU_TYPE_X86)
			ft_putendl(I386);
		else if (get_32(fat_hdr[i].cputype, file) == CPU_TYPE_X86_64)
			ft_putendl(X86_64);
		else if (get_32(fat_hdr[i].cputype, file) == CPU_TYPE_POWERPC && \
		get_32(fat_hdr[i].cpusubtype, file) == CPU_SUBTYPE_POWERPC_ALL)
			ft_putendl(PPC);
		else
			ft_putendl(DEFAULT_ARCH);
	}
	else
	{
		ft_putstr(file->name);
		ft_putendl(":");
	}
}

void					handle_fat32(void *ptr, t_file *file)
{
	int					i;
	int					nb_arch;
	struct fat_arch		*fat_hdr;
	void				*member;
	int					tmp_endian;

	i = -1;
	tmp_endian = file->endian;
	nb_arch = get_32((*(struct fat_header *)ptr).nfat_arch, file);
	fat_hdr = (struct fat_arch *)(ptr + sizeof(struct fat_header));
	if (!farch(fat_hdr, nb_arch, file))
	{
		while (++i < nb_arch)
		{
			file->endian = tmp_endian;
			file->size = get_32(fat_hdr[i].size, file);
			print(i, nb_arch, file, fat_hdr);
			member = (uint8_t *)(ptr + get_32(fat_hdr[i].offset, file));
			check_type_header(member, file);
		}
	}
}
