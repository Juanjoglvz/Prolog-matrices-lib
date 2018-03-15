#include "/usr/lib/swi-prolog/include/SWI-Prolog.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define NULLTERM 0

int printlist(term_t list);

foreign_t
pl_printlist(term_t list)
{
	if (!PL_is_list(list))
	{
		printf("Input parameter is not a list!\n");
		PL_fail;
	}
	
	int retval = printlist(list);
	printf("\nRetval is: %d\n", retval);
	
	if (retval)
		PL_fail;
	else
		PL_succeed;
}

int printlist(term_t list)
{
	int error = 0;
	if (!PL_is_list(list))
	{
		printf("Term is not a list\n");
		return -1;
	}

	long unsigned int length;
	if (!PL_skip_list(list, NULLTERM, &length))
	{
		printf("Error getting the length of the list");
		return -2;
	}

	term_t head = PL_new_term_ref();
	term_t tail = PL_copy_term_ref(list);

	printf("[");
	int i;
	for (i = 0; i < length; i++)
	{
		if (!PL_get_list(tail, head, tail))
		{
			printf("Error getting the list\n");
			return -3;
		}
		// Recursively call this function to print nested lists
		if (PL_is_list(head))
		{
			int error = printlist(head);
			if (error)
				return error;
			printf(", ");
		}
		else if (!PL_is_integer(head))
		{
			printf("Element is not an integer\n");
			return -4;
		}
		else
		{
			int val;
			if (!PL_get_integer(head, &val))
			{
				printf("Error getting the integer");
				return -5;
			}
			printf("%d, ", val);
		}
	}
	printf("\b\b]");
	return 0;
}


install_t
install()
{ 
	PL_register_foreign("printit", 1, pl_printlist, 0);
}
