#ifndef NM_H
# define NM_H

#include "libft/includes/libft.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <stdbool.h>

extern int k;

typedef struct 	s_options
{
	size_t 		flag_g;
	size_t		flag_p;
	size_t 		flag_u;
	size_t		have_opt;
}				t_options;

typedef struct  s_my_meta
{
	uint32_t 				index_st;
	uint8_t					n_type;
	uint8_t 				n_sect;        	/* section number or NO_SECT */
    uint16_t 				n_desc;       	/* see <mach-o/stab.h> */
    uint64_t 				n_value64;      /* value of this symbol (or stab offset) */
    uint32_t 				n_value32;
    bool					is_printable;
    struct s_my_meta 				*next;
    struct s_my_meta 				*prev;
    int debug;
}				t_my_meta;


typedef struct 	s_sym 
{
	size_t						sect_text;
	size_t						sect_data;
	size_t						sect_bss;
	size_t						is_64;
	struct segment_command_64 	**segments;
	t_my_meta					*meta;
}				t_sym;


void *open_arg(char *arg, t_options *options);
void handle_64(void *ptr, t_sym *data);
void handle_32(void *ptr);
t_my_meta	*fill_meta_data(struct nlist_64 *array, uint32_t nsyms, t_sym *data);
#endif