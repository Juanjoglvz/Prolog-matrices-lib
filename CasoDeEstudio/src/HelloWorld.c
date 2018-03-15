#include "/usr/lib/swi-prolog/include/SWI-Prolog.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int printlist(term_t list);

foreign_t
pl_printlist(term_t list)
{
	FUNCTOR_dot2;
	if (!PL_is_list(list))
	{
		printf("Input parameter is not a list!\n");
		PL_fail;
	}
	
	printf("Retval is: %d\n", printlist(list));
	
	PL_succeed;
}

foreign_t
pl_write_atoms(term_t l)
{ term_t head = PL_new_term_ref();   /* the elements */
  term_t list = PL_copy_term_ref(l); /* copy (we modify list) */

  while( PL_get_list(list, head, list) )
  { char *s;

    if ( PL_get_atom_chars(head, &s) )
      printf("%s\n", s);
    else
      PL_fail;
  }

  return PL_get_nil(list);            /* test end for [] */
}

int printlist(term_t list)
{
	int error = 0;
	if (!PL_is_list(list))
	{
		printf("Term is not a list\n");
		return -1;
	}

	term_t head = PL_new_term_ref();
	term_t tail = PL_new_term_ref();

	if (!PL_get_list(list, head, tail))
	{
		printf("Error getting the list: %d\n", error);
		return -2;
	}

	printf("[");
	while (!PL_get_nil(tail))
	{
		if (PL_is_list(head))
		{
			printlist(head); 
			
			if (PL_get_list(tail, head, tail))
			{
				printf("Error getting the list\n");
				return -6;
			}
			continue;
		}
		if (!PL_is_integer(head))
		{
			printf("Element is not an integer\n");
			return -3;
		}
		int val;
		if (PL_get_integer(head, &val))
		{
			printf("Error getting the integer");
			return -4;
		}
		printf("%d ", val);


		if (PL_get_list(tail, head, tail))
		{
			printf("Error getting the list\n");
			return -5;
		}
	}
}


install_t
install_helloworld()
{ 
	PL_register_foreign("printit", 1, pl_printlist, 0);
	PL_register_foreign("pl_write_atoms", 1, pl_write_atoms, 0);
}
