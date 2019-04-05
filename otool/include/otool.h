/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandreu <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:57:20 by jandreu           #+#    #+#             */
/*   Updated: 2019/04/03 16:57:28 by jandreu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OTOOL_H
# define OTOOL_H

# include "../../libft/includes/libft.h"
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach/machine.h>
# include <ar.h>
# include <mach-o/ranlib.h>
# include <sys/types.h>
# include <stdbool.h>
# define TYPE_UNKNOW  0
# define TYPE_FAT 1
# define TYPE_FAT_64 2
# define TYPE_ARCHIVE  3
# define TYPE_OBJECT  4
# define TYPE_OBJECT_64  5
# define PPC " (architecture ppc):"
# define PPC64 " (architecture ppc64):"
# define I386 " (architecture i386):"
# define X86_64 " (architecture x86_64):"
# define DEFAULT_ARCH " (for architecture ):"

typedef struct load_command			t_lc;
typedef struct segment_command		t_sc;
typedef struct mach_header			t_mh;
typedef struct nlist				t_nl;
typedef struct symtab_command		t_symc;

typedef struct segment_command_64	t_sc64;
typedef struct mach_header_64		t_mh64;
typedef struct nlist_64				t_nl64;
typedef struct section_64			t_sect64;

enum								e_endianness
{
	ENDIAN_UNKNOWN = 0,
	ENDIAN_BIG,
	ENDIAN_LITTLE
};

typedef struct						s_file
{
	char							*name;
	uint8_t							*st_file;
	uint8_t							*end_file;
	uint8_t							type;
	uint8_t							endian;
	uint64_t						size;
	uint64_t						offset;
	uint8_t							addr_add;
	int								addr;
	int								ofile_size;
}									t_file;

/*
** tools.c
*/
void								ft_print_addr(char *addr, uint8_t type, \
									uint8_t *endptr);
void								check_type_header(void *ptr, t_file *file);
int									get_ar_name_size(char *ar_name);
int									ft_secure_putstr(char *to_print, \
									uint8_t *end_file, uint8_t nl);

/*
** mach-o_64.c
*/
void								handle_sect_64(struct load_command *\
									lc, t_file *file, t_mh64 *mh);
void								handle_mach_o_64(t_file *file, \
									void *file_ptr);

/*
** mach-o_32.c
*/
void								handle_section_32(struct load_command *\
									load_command, t_file *file, t_mh *mh);
void								handle_mach_o_32(t_file *file,\
									void *file_ptr);

/*
** archive.c
*/
void								handle_archive(t_file *file, void *\
									file_ptr);

/*
** fat_32.c
*/
void								handle_fat32(void *ptr, t_file *file);

/*
** manage_endian.c
*/
enum e_endianness					get_elem_endianness(void *magic);
uint32_t							get_32(uint32_t val, t_file *file);
uint64_t							get_uint64(uint64_t val, t_file *file);

#endif
