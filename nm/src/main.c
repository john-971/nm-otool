/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 16:00:03 by jandreu           #+#    #+#             */
/*   Updated: 2019/03/31 16:00:18 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/nm.h"

int				fill_file_struct(t_file *file, void *ptr, int ac, char *av)
{
	file->st_file = ptr;
	file->name = av;
	file->end_file = (uint8_t *)ptr + file->size;
	if (file->size < sizeof(uint32_t))
		return (1);
	if (ac > 2 && (*(uint32_t *)ptr == MH_MAGIC || \
	*(uint32_t *)ptr == MH_CIGAM || *(uint32_t *)ptr == MH_MAGIC_64 || \
	*(uint32_t *)ptr == MH_CIGAM_64))
	{
		ft_putchar('\n');
		ft_putstr(file->name);
		ft_putendl(":");
	}
	return (0);
}

int				open_arg(char *arg, t_file *file, int argc, int fd)
{
	struct stat	stats;
	void		*ptr;

	if (fstat(fd, &stats) == 0)
	{
		ptr = mmap(0, stats.st_size, PROT_READ | PROT_WRITE,\
		MAP_PRIVATE, fd, 0);
		if (ptr != MAP_FAILED)
		{
			file->size = stats.st_size;
			if (fill_file_struct(file, ptr, argc, arg) == 1)
				return (1);
			check_type_header(ptr, file);
			munmap(ptr, stats.st_size);
		}
	}
	else
		ft_putendl("ERREUR FSTAT");
	return (0);
}

int				main(int argc, char **argv)
{
	t_file		*file;
	int			fd;

	file = (t_file *)malloc(sizeof(t_file));
	if (file != NULL)
	{
		ft_bzero(file, sizeof(t_file));
		argv++;
		if (argc > 1)
			while (*argv != '\0')
			{
				fd = open(*argv, O_RDONLY);
				fd != -1 ? open_arg(*argv, file, argc, fd) : \
				ft_putendl("ERREUR OPEN");
				argv++;
			}
		else
		{
			fd = open("a.out", O_RDONLY);
			fd != -1 ? open_arg("a.out", file, argc, fd) : \
				ft_putendl("ERREUR OPEN");
		}
	}
	free(file);
}
