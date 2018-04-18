#include <SWI-Prolog.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <matlog.h>

foreign_t
pl_add_matrices(term_t m1, term_t m2, term_t result);
foreign_t
pl_sum_elements_matrix(term_t m1, term_t result);
foreign_t
pl_substract_matrices(term_t m1, term_t m2, term_t result);
foreign_t
pl_multiply_matrices(term_t m1, term_t m2, term_t result);
foreign_t
pl_multiply_matrix(term_t m1, term_t m2, term_t result);
foreign_t
pl_determinant_matrix(term_t m1, term_t result);
foreign_t
pl_transpose_matrix(term_t m1, term_t result);
foreign_t
pl_inverse_matrix(term_t m1, term_t result);

foreign_t
pl_is_squared(term_t matrix);
foreign_t
pl_is_same_dimensions(term_t m1, term_t m2);


foreign_t
pl_add_matrices(term_t m1, term_t m2, term_t result)
{
	struct Matrix_t* matrix1;
	struct Matrix_t* matrix2;
	struct Matrix_t* mresult = NULL;

	matrix1 = list_to_matrix(m1);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}

	matrix2 = list_to_matrix(m2);
	if (!matrix2)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
		PL_fail;
	}


	mresult = alloc_matrix(matrix1->size);
    if (!mresult)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new matrix\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
    }
	
	if (!add_matrices(matrix1, matrix2, mresult))
	{
		DEBUG_PRINT("Cannot add matrices\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}

	term_t mlist = PL_new_term_ref();
	if (!matrix_to_list(mresult, mlist))
	{
		DEBUG_PRINT("Error converting the matrix back to list\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1) | !free_matrix(matrix2) | !free_matrix(mresult))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	return PL_unify(result, mlist);
}

foreign_t
pl_sum_elements_matrix(term_t m1, term_t result)
{
	struct Matrix_t* matrix1;

	matrix1 = list_to_matrix(m1);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}
	
	double mresult = 0;
	if (!sum_elements(matrix1, &mresult))
	{
		DEBUG_PRINT("Cannot add elements\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	term_t temp = PL_new_term_ref();
	
	return PL_put_float(temp, mresult) && PL_unify(result, temp);
}

foreign_t
pl_substract_matrices(term_t m1, term_t m2, term_t result)
{	
	struct Matrix_t* matrix1;
	struct Matrix_t* matrix2;
	struct Matrix_t* mresult = NULL;

	matrix1 = list_to_matrix(m1);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}

	matrix2 = list_to_matrix(m2);
	if (!matrix2)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
		PL_fail;
	}


	mresult = alloc_matrix(matrix1->size);
    if (!mresult)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new matrix\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
    }
	
	if (!substract_matrices(matrix1, matrix2, mresult))
	{
		DEBUG_PRINT("Cannot add matrices\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}

	term_t mlist = PL_new_term_ref();
	if (!matrix_to_list(mresult, mlist))
	{
		DEBUG_PRINT("Error converting the matrix back to list\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1) | !free_matrix(matrix2) | !free_matrix(mresult))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	return PL_unify(result, mlist);
}

foreign_t
pl_multiply_matrices(term_t m1, term_t m2, term_t result)
{
	struct Matrix_t* matrix1;
	struct Matrix_t* matrix2;
	struct Matrix_t* mresult = NULL;

	matrix1 = list_to_matrix(m1);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}

	matrix2 = list_to_matrix(m2);
	if (!matrix2)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
		PL_fail;
	}

	size_t nsize[2] = {0};
	nsize[0] = matrix1->size[0];
	nsize[1] = matrix2->size[1];
	mresult = alloc_matrix(nsize);
    if (!mresult)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new matrix\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
    }
	
	if (!multiply_matrices(matrix1, matrix2, mresult))
	{
		DEBUG_PRINT("Cannot multiply matrices\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}

	term_t mlist = PL_new_term_ref();
	if (!matrix_to_list(mresult, mlist))
	{
		DEBUG_PRINT("Error converting the matrix back to list\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1) | !free_matrix(matrix2) | !free_matrix(mresult))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	return PL_unify(result, mlist);
}

foreign_t
pl_multiply_matrix(term_t m1, term_t m2, term_t result)
{
	struct Matrix_t* matrix1;
	double factor;
	struct Matrix_t* mresult = NULL;

	matrix1 = list_to_matrix(m1);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}

	int ival = 0;
	double dval = 0;
	switch (PL_term_type(m2))
	{
		case PL_INTEGER:
			if (!PL_get_integer(m2, &ival))
			{
				DEBUG_PRINT("Error while getting the value of the factor\n");
				if (!free_matrix(matrix1))
				{
					DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
				}
				return 0;
			}

			factor = (double) ival;
			break;
		
		case PL_FLOAT:
			if (!PL_get_float(m2, &dval))
			{
				DEBUG_PRINT("Error while getting the value of the factor\n");
				if (!free_matrix(matrix1))
				{
					DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
				}
				return 0;
			}

			factor = dval;
			break;

		default:
			DEBUG_PRINT("Element is not numeric\n");
			if (!free_matrix(matrix1))
			{
				DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
				return 0;
			}
			return 0;
	}

	mresult = alloc_matrix(matrix1->size);
    if (!mresult)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new matrix\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
    }
	
	if (!multiply_matrix(matrix1, factor, mresult))
	{
		DEBUG_PRINT("Cannot multiply matrix\n");
		if (!free_matrix(matrix1) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}

	term_t mlist = PL_new_term_ref();
	if (!matrix_to_list(mresult, mlist))
	{
		DEBUG_PRINT("Error converting the matrix back to list\n");
		if (!free_matrix(matrix1) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1) | !free_matrix(mresult))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	return PL_unify(result, mlist);
}

foreign_t
pl_determinant_matrix(term_t m1, term_t result)
{
	struct Matrix_t* matrix1;

	matrix1 = list_to_matrix(m1);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}
	
	double mresult = 0;
	if (!determinant(matrix1, &mresult))
	{
		DEBUG_PRINT("Cannot calculate determinant\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	term_t temp = PL_new_term_ref();
	
	return PL_put_float(temp, mresult) && PL_unify(result, temp);
}

foreign_t
pl_transpose_matrix(term_t m1, term_t result)
{
	struct Matrix_t* matrix1;
	struct Matrix_t* mresult = NULL;

	matrix1 = list_to_matrix(m1);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}

	size_t nsize[2] = {0};
	nsize[0] = matrix1->size[1];
	nsize[1] = matrix1->size[0];
	mresult = alloc_matrix(nsize);
    if (!mresult)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new matrix\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
    }
	
	if (!transpose(matrix1, mresult))
	{
		DEBUG_PRINT("Cannot transpose matrix\n");
		if (!free_matrix(matrix1) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}

	term_t mlist = PL_new_term_ref();
	if (!matrix_to_list(mresult, mlist))
	{
		DEBUG_PRINT("Error converting the matrix back to list\n");
		if (!free_matrix(matrix1) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1) | !free_matrix(mresult))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	return PL_unify(result, mlist);
}

foreign_t
pl_inverse_matrix(term_t m1, term_t result)
{
	struct Matrix_t* matrix1;
	struct Matrix_t* mresult = NULL;

	matrix1 = list_to_matrix(m1);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}

	size_t nsize[2] = {0};
	nsize[0] = matrix1->size[1];
	nsize[1] = matrix1->size[0];
	mresult = alloc_matrix(nsize);
    if (!mresult)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new matrix\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
    }
	
	if (!inverse(matrix1, mresult))
	{
		DEBUG_PRINT("Cannot invert matrix\n");
		if (!free_matrix(matrix1) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}

	term_t mlist = PL_new_term_ref();
	if (!matrix_to_list(mresult, mlist))
	{
		DEBUG_PRINT("Error converting the matrix back to list\n");
		if (!free_matrix(matrix1) | !free_matrix(mresult))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1) | !free_matrix(mresult))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	return PL_unify(result, mlist);
}


foreign_t
pl_is_squared(term_t matrix)
{
	struct Matrix_t* matrix1;

	matrix1 = list_to_matrix(matrix);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}
	
	int sq = 0;
	if (!is_squared(matrix1, &sq))
	{
		DEBUG_PRINT("Cannot check matrix\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	return sq > 0;
}

foreign_t
pl_is_same_dimensions(term_t m1, term_t m2)
{
	struct Matrix_t* matrix1;
	struct Matrix_t* matrix2;

	matrix1 = list_to_matrix(m1);
	if (!matrix1)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		PL_fail;
	}

	matrix2 = list_to_matrix(m2);
	if (!matrix2)
	{
		DEBUG_PRINT("Cannot parse list to matrix\n");
		if (!free_matrix(matrix1))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
		PL_fail;
	}
	
	int sd = 0;
	if (!is_same_dimensions(matrix1, matrix2, &sd))
	{
		DEBUG_PRINT("Cannot check matrices\n");
		if (!free_matrix(matrix1) | !free_matrix(matrix2))
		{
			DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		}
        PL_fail;
	}
	
	if (!free_matrix(matrix1) | !free_matrix(matrix2))
	{
		DEBUG_PRINT("Error freeing matrix memory, memory corruption??\n");
		PL_fail;
	}

	return sd > 0;
}

install_t
install()
{
	PL_register_foreign("sum_elements", 2, pl_sum_elements_matrix, 0);
	PL_register_foreign("add_matrices", 3, pl_add_matrices, 0);
	PL_register_foreign("substract_matrices", 3, pl_substract_matrices, 0);
	PL_register_foreign("multiply_matrices", 3, pl_multiply_matrices, 0);
	PL_register_foreign("multiply_matrix", 3, pl_multiply_matrix, 0);
	PL_register_foreign("determinant", 2, pl_determinant_matrix, 0);
	PL_register_foreign("transpose", 2, pl_transpose_matrix, 0);
	PL_register_foreign("inverse", 2, pl_inverse_matrix, 0);
	PL_register_foreign("is_squared", 1, pl_is_squared, 0);
	PL_register_foreign("is_same_dimensions", 2, pl_is_same_dimensions, 0);
}
