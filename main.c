
#include "nm.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')


char get_symbole_type(t_my_meta meta, t_sym *data)
{
	int 	type_unmasqued;
	char	c;

	type_unmasqued = meta.n_type & N_TYPE;
	c = '?';
	if (type_unmasqued == N_UNDF){
		if(meta.n_value64 != 0)					//JE GERE QUE LE 64
		    c = 'c';
		else
			c = 'u';
	}
	if (type_unmasqued == N_ABS)
		c = 'a';
	if (type_unmasqued == N_SECT)
	{
		// printf("%d\n", type & N_EXT);
		if(meta.n_sect == data->sect_text)
			c = 't';
		else if(meta.n_sect == data->sect_data)
		    c = 'd';
		else if(meta.n_sect == data->sect_bss)
		    c = 'b';
		else
		    c = 's';
	}
	if (type_unmasqued == N_PBUD)
		c = 'u';
	if (type_unmasqued == N_INDR)
		c = 'i';
	return ((meta.n_type & N_EXT) ? ft_toupper(c) : c); //maj pour les globale , min pour les locales
}

t_my_meta	*create_my_meta_element(struct nlist_64 element, t_sym *sym)
{
	t_my_meta *new_meta;

	new_meta = (t_my_meta *)malloc(sizeof(t_my_meta));

	new_meta->index_st = element.n_un.n_strx;
	new_meta->n_type = element.n_type;
	new_meta->n_sect = element.n_sect;
	new_meta->n_desc = element.n_desc;
	new_meta->n_value64 = element.n_value;
	new_meta->is_printable = TRUE;
	new_meta->debug = k++;

	new_meta->next = NULL;
	new_meta->prev = NULL;

	return new_meta;
}

t_my_meta	*fill_meta_data(struct nlist_64 *array, uint32_t nsyms, t_sym *data)
{
	int i;
	t_my_meta *metatmp;

	i = 1;																		//ATTENTION i = 1
	
	// ft_putnbr(nsyms);
	// ft_putchar('\n');
	data->meta = metatmp = create_my_meta_element(array[0], data);				//premier maillon
	metatmp = data->meta;
	while (i < nsyms)
	{
		metatmp->next = create_my_meta_element(array[i], data);
		metatmp->next->prev = metatmp;
		metatmp = metatmp->next;
		i++;
	}
}

void	ft_copy_my_meta(t_my_meta *src, t_my_meta *dest)
{
	t_my_meta *tmp;

	tmp = dest;
	dest->index_st = src->index_st;
	dest->n_type = src->n_type;
	dest->n_sect = src->n_sect;
	dest->n_desc = src->n_desc;
	dest->n_value64 = src->n_value64;
	dest->n_value32 = src->n_value32;
	dest->is_printable = src->is_printable;
	dest->debug = src->debug;

	src->index_st = tmp->index_st;
	src->n_type = tmp->n_type;
	src->n_sect = tmp->n_sect;
	src->n_desc = tmp->n_desc;
	src->n_value64 = tmp->n_value64;
	src->n_value32 = tmp->n_value32;
	src->is_printable = tmp->is_printable;
	src->debug = tmp->debug;
}

t_my_meta *ft_sort_by_ascii(t_my_meta *meta, char *stringtable, uint32_t nsyms)
{
	
	int ret;
	int i;
	t_my_meta *tmp;
	t_my_meta *meta_sorted;
	bool	swapped = TRUE;

	i = 0;
	// meta_sorted = meta;
	while(swapped){

		swapped = FALSE;
		meta_sorted = meta;
		while (meta_sorted->next != NULL)
		{
			
			printf("%d %d\n",meta_sorted->debug, meta_sorted->next->debug);
			ret = ft_strcmp(stringtable + meta_sorted->index_st, stringtable + meta_sorted->next->index_st);
			if (stringtable + meta_sorted->index_st > stringtable + meta_sorted->next->index_st)
			{
				printf("DIFF ");
				ft_copy_my_meta(meta_sorted, meta_sorted->next);
				swapped = TRUE;		
			}
			meta_sorted = meta_sorted->next;
				
			
		}
		
	}
	return meta;
}

void print_output(void *ptr, struct symtab_command* sym, t_sym *data)
{
	int 			i;
	char			*stringtable;
	struct nlist_64 *array;
	char 			*buff;
	t_my_meta		*meta;

	array = ptr + sym->symoff;
	stringtable = (void *)ptr + sym->stroff;

	i = 0;

	fill_meta_data(array, sym->nsyms, data);
	meta = data->meta;

	meta = ft_sort_by_ascii(meta, stringtable, sym->nsyms);
	if (meta == NULL)
	{
		return ;
	}
	while (i < sym->nsyms - 1)
	{

		ft_itoa_hex(meta->n_value64, 0, buff);
		ft_putstr(buff);
		ft_putchar(' ');
		ft_putchar(get_symbole_type(*meta, data));
		ft_putchar(' ');
		ft_putnbr(meta->debug);
		ft_putchar(' ');
		printf("%s\n", stringtable + meta->index_st);


		meta = meta->next;

		//printf("NUM SECTION %i\n", array[i].n_sect + N_SECT);
		//printf("%d \n", i);
		i++;
	}

	// 	printf(" ON RENBOBINNE \n");

	// while (meta->prev != NULL)
	// {

	// 	ft_itoa_hex(meta->n_value64, 0, buff);
	// 	ft_putstr(buff);
	// 	ft_putchar(' ');
	// 	ft_putchar(get_symbole_type(*meta, data));
	// 	ft_putchar(' ');
	// 	printf("%s\n", stringtable + meta->index_st);

	// 	meta = meta->prev;

	// 	//printf("NUM SECTION %i\n", array[i].n_sect + N_SECT);
	// 	//printf("%d \n", i);
	// 	i++;
	// }
}





void get_architecture(void *ptr, t_sym *data)
{
	int magicnum;

	magicnum = *(int *)ptr;
	//ft_putnbr(magicnum);
	printf("mon magicnumber %i \n", magicnum);
	printf("64 %i \n", MH_MAGIC_64);
	printf("32 %i \n", MH_MAGIC);

	if (magicnum == MH_MAGIC_64)
	{
		data->is_64 = 1;
		handle_64(ptr, data);
	}
	else if (magicnum == MH_MAGIC)
	{
		handle_32(ptr);
	}

}

void print_error(char *error)
{
	ft_putendl(error);
}

/*to free :
options
t_sym

*/
void *open_arg(char *arg, t_options *option)
{
	int fd;
	struct stat stats;
	void *ptr;
	t_sym	 				*data;
	
	data = (t_sym*)malloc(sizeof(t_sym));
	

	ft_bzero(data, sizeof(t_sym));
	data->meta = NULL;
	fd = open(arg, O_RDONLY);
	if (fd != -1)
	{
		if (fstat(fd, &stats) == 0)
		{
			ptr = mmap(0, stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
			
			get_architecture(ptr, data);
			
			munmap(ptr, stats.st_size);
			//return (ptr == MAP_FAILED ? NULL : ptr);
		}
		else
		{
			ft_putendl("ERREUR FSTAT");
		}
	}
	else
	{
		ft_putendl("ERREUR OPEN");
	}
	return (NULL);
	
}

int k = 0;

int 			main(int argc, char **argv)
{
	int 		i;
	void 		*ptr;
	t_options	*options;
	

	k = 0;

	i = 1;
	options = (t_options *)malloc(sizeof(t_options));
	if (argc > 1)
	{
		while(i < argc)
		{
			if(argv[i][0] == '-')
			{
				if(argv[i][1] == '\0' || argv[i][1] == '-'){
					i++;
				    while(i < argc)
						open_arg(argv[i++], options);
				    return 0;
				}

				if ( argv[i][1] != '\0' && argv[i][1] != '-')
				{
					if (argv[i][1] == 'g')
						options->flag_g = 1;
					else if (argv[i][1] == 'p')
						options->flag_p = 1;
					else if (argv[i][1] == 'u')
						options->flag_u = 1;
					else
					{
						print_error("Option non reconnue : Usage [-g, -p, -u]");
						return 1;
					}
					options->have_opt = 1;
				}
			}
			else
			{
				open_arg(argv[i], options);
			}
			i++;
		}
	}
	else
	{
		open_arg("a.out", options);
	}
	
}