#include <SWI-Prolog.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <matlog.h>

int printlist(term_t list)
{
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

foreign_t
pl_is_matrix(term_t list)
{
	size_t dimensions[2];
	int res = is_matrix(list, dimensions);
	if (res)
	{
		DEBUG_PRINT("Number of rows: %lu  Number of columns: %lu\n", dimensions[0], dimensions[1]);
	}
	return res == 0 ? 0 : 1;
}

foreign_t
pl_matrix_size(term_t list, term_t row, term_t column)
{
	size_t dimensions[2] = {0};

	int res = is_matrix(list, dimensions);

	if (!res)
	{
		PL_fail;
	}
	else
	{
		term_t tRow = PL_new_term_ref();
		term_t tColumn = PL_new_term_ref();

		PL_put_integer(tRow, dimensions[0]);
		PL_put_integer(tColumn, dimensions[1]);

		PL_unify(row, tRow);
		return PL_unify(column, tColumn);
	}
}

foreign_t
pl_sum_matrix(term_t list, term_t out)
{
	struct Matrix_t* matrix = (struct Matrix_t*) calloc(1, sizeof(struct Matrix_t));
	if (!matrix)
	{
		DEBUG_PRINT("Cannot allocate memory for the matrix\n");
		PL_fail;
	}

	int res = list_to_matrix(list, matrix);
	if (!res)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}

	double sum = sum_matrix(matrix);

	term_t result = PL_new_term_ref();
	PL_put_float(result, sum);
	return PL_unify(out, result);
}

install_t
install()
{
	PL_register_foreign("printit", 1, pl_printlist, 0);
	PL_register_foreign("ismatrix", 1, pl_is_matrix, 0);
	PL_register_foreign("summatrix", 2, pl_sum_matrix, 0);
	PL_register_foreign("matrix_size", 3, pl_matrix_size, 0);
}
