/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_endian.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 16:01:26 by jandreu           #+#    #+#             */
/*   Updated: 2019/03/31 16:01:28 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/nm.h"

static enum e_endianness	get_host_endianness(void)
{
	uint32_t				e;

	e = (ENDIAN_BIG << 24) | ENDIAN_LITTLE;
	return ((enum e_endianness)*((char *)&e));
}

enum e_endianness			get_elem_endianness(void *magic)
{
	if (*(uint32_t *)magic == MH_MAGIC || *(uint32_t *)magic == MH_MAGIC_64 || \
	*(uint32_t *)magic == FAT_MAGIC || \
	*(uint32_t *)magic == FAT_MAGIC_64 || ft_memcmp(magic, ARMAG, SARMAG) == 0)
		return (get_host_endianness());
	else
	{
		return (get_host_endianness() == ENDIAN_BIG ? \
		ENDIAN_LITTLE : ENDIAN_BIG);
	}
}

uint32_t					get_32(uint32_t val, t_file *file)
{
	int						swap;

	swap = get_host_endianness() != file->endian;
	return (swap ? OSSwapInt32(val) : val);
}

uint64_t					get_uint64(uint64_t val, t_file *file)
{
	int						swap;

	swap = get_host_endianness() != file->endian;
	return (swap ? OSSwapInt64(val) : val);
}
