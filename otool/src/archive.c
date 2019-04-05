/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 16:00:32 by jandreu           #+#    #+#             */
/*   Updated: 2019/03/31 16:00:35 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/otool.h"

int							get_ar_name_size(char *ar_name)
{
	char					*test_name_trunc;

	test_name_trunc = ft_strstr(ar_name, AR_EFMT1);
	if (test_name_trunc != NULL)
		return (ft_atoi(ft_strsub(test_name_trunc, ft_strlen(AR_EFMT1), 3)));
	else
		return (ft_strlen(ar_name));
}

int							manage_output(t_file *file, struct ar_hdr *ar_hdr)
{
	if (file->ofile_size != 0)
		ft_putchar('\n');
	ft_putstr(file->name);
	ft_putchar('(');
	if (ft_secure_putstr(((char *)ar_hdr + sizeof(struct ar_hdr)), \
		file->end_file, 0) == 0)
		return (1);
	ft_putendl("):");
	return (0);
}

void						handle_archive(t_file *file, void *file_ptr)
{
	uint32_t				offset;
	struct ranlib			*ranl;
	struct ar_hdr			*ar_hdr;

	offset = SARMAG;
	ar_hdr = (struct ar_hdr *)(file_ptr + offset);
	offset += sizeof(struct ar_hdr) + ft_atoi(ar_hdr->ar_size);
	ranl = (struct ranlib*)((uint8_t *)(file_ptr) + sizeof(struct ar_hdr) + \
	get_ar_name_size(ar_hdr->ar_name) + sizeof(uint32_t));
	ft_putstr("Archive : ");
	ft_putstr(file->name);
	while (offset < file->size)
	{
		ar_hdr = (struct ar_hdr *)((uint8_t *)(file_ptr) + offset);
		if (manage_output(file, ar_hdr) == 1)
			return ;
		if ((offset + (sizeof(struct ar_hdr) + ft_atoi(ar_hdr->ar_size))) \
		<= file->size)
			check_type_header(((uint8_t *)ar_hdr + sizeof(struct ar_hdr) + \
			get_ar_name_size(ar_hdr->ar_name)), file);
		offset += (sizeof(struct ar_hdr) + ft_atoi(ar_hdr->ar_size));
	}
}
