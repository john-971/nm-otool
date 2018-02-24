
#include "nm.h"

void handle_section_64(struct section_64 *section, t_sym *data, uint32_t nsects)
{
	int j;

	j = 0;

	while (j < nsects)
	{
		printf(" SECTION => %s : \t\t %s \n", (section + j)->sectname, (section + j)->segname);

		if(ft_strcmp((section + j)->sectname, SECT_TEXT) == 0 && strcmp((section + j)->segname, SEG_TEXT) == 0)
			    data->sect_text = j + 1;
		else if(strcmp((section + j)->sectname, SECT_DATA) == 0 && strcmp((section + j)->segname, SEG_DATA) == 0)
		    data->sect_data = j + 1;
		else if(strcmp((section + j)->sectname, SECT_BSS) == 0 && strcmp((section + j)->segname, SEG_DATA) == 0)
		    data->sect_bss = j + 1;
		// section = (void *)section + sizeof(struct section_64);
		j++;
	}
}

void handle_64(void *ptr, t_sym *data)
{
	struct mach_header_64 	*header;
	struct load_command		*load_command;
	int						num_command;
	struct symtab_command	*sym;
	struct segment_command_64 *segment;
	int 					i;
	struct section_64		*section;
	

	i = 0;

	header = (struct mach_header_64 *) ptr;
	//printf("%u\n %d\n %u\n %u\n Load Comands %u\n %u\n %u\n", header->magic, header->cputype, header->cpusubtype, header->filetype, header->ncmds, header->sizeofcmds, header->flags);
	num_command = header->ncmds;
	load_command = (void *)ptr + sizeof(*header);
	
	printf(" NOMBRE  COMMAND : %d \n", num_command);
	while(i < num_command)
	{
	
		printf("Num LOAD COMMAND : %d \n", load_command->cmd);	
		if (load_command->cmd == LC_SEGMENT_64)
		{

			segment = (void *)load_command;
			printf(" Segment : NB section %s , %d \n", segment->segname, segment->nsects);

			section = (void *)segment + sizeof(struct segment_command_64);

			handle_section_64(section, data, segment->nsects);
			
		}



		if (load_command->cmd == LC_SEGMENT)
		{
			//TODO 
		}




		else if (load_command->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)load_command;
			print_output(ptr, sym, data);
		}

		load_command = (void*) load_command + load_command->cmdsize;
		i++;
	}
	printf(" TEXT => %lu : DATA => %lu : BSS => %lu \n", data->sect_text, data->sect_data, data->sect_bss);
}

void handle_32()
{
	;
}