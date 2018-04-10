#include <matlog.h>
#include <string.h>

int copy_matrix(struct Matrix_t* src, struct Matrix_t* dest)
{
    if (!src)
    {
        DEBUG_PRINT("Null source pointer\n");
        return 0;
    }
    if (!src->size)
    {
        DEBUG_PRINT("Null source pointer\n");
        return 0;
    }
    if (!src->rows)
    {
        DEBUG_PRINT("Null source pointer\n");
        return 0;
    }

        memcpy(dest->size, src->size, sizeof(double) * 2);

    int rows = src->size[0];
    int cols = src->size[1];

    double** rptr = (double**) calloc(rows, sizeof(double*));

    if (!rptr)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new rows\n");
        return 0;
    }
    else
    {
        dest->rows = rptr;
    }

    for (int i = 0; i < rows; i++)
    {
        double* cptr = (double*) calloc(cols, sizeof(double));
        if (!cptr)
        {
            DEBUG_PRINT("Tried to allocate memory, but it didnt succeed, aborting\n");
            return 0;
        }
        else
        {
            *(dest->rows + i) = cptr;
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            dest->rows[i][j] = src->rows[i][j];
        }
    }

    return 1;
}



/*
    Accepts a SWI-Prolog term and returns zero if the term is not a matrix
    or a nonzero value indicating the number of rows the matrix has.
    If a non-null size_t pointer is passed then it is filled with the
    dimensions of the matrix. The pointer is interpreted as a two element
    array, the first element is the number of rows, the second the
    number of columns
*/
int is_matrix(term_t list, size_t* dimensions)
{
    size_t elements = 0;
    size_t rows = 0;
    size_t columns = 0;
    int row_matrix = 0; // Tells whether the matrix is a row matrix

    if (!PL_is_list(list))
    {
        DEBUG_PRINT("Term is not a list\n");
        return 0;
    }
    else
    {
        if (!PL_skip_list(list, NULLTERM, &elements))
        {
            DEBUG_PRINT("Error while getting the list length\n");
            return 0;
        }
        else
        {
            term_t head = PL_new_term_ref();
	        term_t tail = PL_copy_term_ref(list);

            for (int i = 0; i < elements; i++)
            {
                if (!PL_get_list(tail, head, tail))
                {
                    DEBUG_PRINT("Error while iterating through the list\n");
                    return 0;
                }

                if (PL_is_list(head))
                {
                    rows++;
                    // Cannot have a row matrix with nested lists 
                    // (that would be multiple rows)
                    if (row_matrix) 
                    {
                        DEBUG_PRINT("row_matrix is true\n");
                        return 0;
                    }

                    size_t dims[2] = {0};
                    size_t res = (size_t) is_matrix(head, dims);

                    if (res == 0)
                    {
                        DEBUG_PRINT("Error checking the row matrices\n");
                        return 0;
                    }

                    size_t cols = dims[1];
                    
                    if (columns == 0)
                    {
                        columns = cols;
                    }

                    if (cols != columns)
                    {
                        DEBUG_PRINT("Different number of columns, %lu vs %lu\n", cols, columns);
                        return 0;
                    }

                }
                else // Row Matrix
                {
                    if (PL_is_number(head))
                    {
                        rows = 1;
                        row_matrix = 1;
                        columns++;
                    }
                    else
                    {
                        DEBUG_PRINT("One of the elements is not a number\n");
                        return 0;
                    }
                }
            }
        }
    }
    if (dimensions)
    {
        dimensions[0] = rows;
        dimensions[1] = columns;
    }
    return (int) rows;
}

int list_to_matrix(term_t list, struct Matrix_t* matrix)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is NULL\n");
        return 0;
    }

    int res = is_matrix(list, matrix->size);

    if (!res)
    {
        DEBUG_PRINT("List is not a matrix\n");
        return 0;
    }

    size_t rows = *(matrix->size);
    size_t columns = *(matrix->size + 1);
    // Allocate space for the matrix data

    double** rptr = (double**) calloc(rows, sizeof(double*));
    if (!rptr)
    {
        DEBUG_PRINT("Tried to allocate memory, but it didnt succeed, aborting\n");
        return 0;
    }
    else
    {
        matrix->rows = rptr;
    }


    for (int i = 0; i < rows; i++)
    {
        double* ptr = (double*) calloc(columns, sizeof(double));
        if (!ptr)
        {
            DEBUG_PRINT("Tried to allocate memory, but it didnt succeed, aborting\n");
            return 0;
        }
        else
        {
            *(matrix->rows + i) = ptr;
        }
    }


    term_t head = PL_new_term_ref();
    term_t tail = PL_copy_term_ref(list);
    
    for (int i = 0; i < rows; i++)
    {
        if (!PL_get_list(tail, head, tail))
        {
            DEBUG_PRINT("Error while iterating through the list\n");
            return 0;
        }
            

        double* row = *(matrix->rows + i);
        int res = 0;
        if (rows == 1) // Row matrix
        {
            res = list_to_row(list, columns, row);
        }
        else
        {
            res = list_to_row(head, columns, row);
        }

        if (!res)
        {
            DEBUG_PRINT("Couldn't convert the list to a matrix\n");
            return 0;
        }
    }
    return 1;
}

int list_to_row(term_t list, size_t length, double* row)
{
    if (!row)
    {
        DEBUG_PRINT("Row pointer is null\n");
        return 0;
    }

    term_t head = PL_new_term_ref();
    term_t tail = PL_copy_term_ref(list);

    for (int i = 0; i < length; i++)
    {
        if (!PL_get_list(tail, head, tail))
        {            
            DEBUG_PRINT("Error while iterating through the list\n");
            return 0;
        }

        int ival;
        double dval;
        switch (PL_term_type(head))
        {
            case PL_INTEGER:
                if (!PL_get_integer(head, &ival))
                {
                    DEBUG_PRINT("Error while getting the value of the element\n");
                }

                *(row + i) = (double) ival;
                break;
            
            case PL_FLOAT:
                if (!PL_get_float(head, &dval))
                {
                    DEBUG_PRINT("Error while getting the value of the element\n");
                }

                *(row + i) = dval;
                break;

            default:
                DEBUG_PRINT("Element is not numeric\n");
                return 0;
        }
    }
    
    return 1;
}

int matrix_to_list(struct Matrix_t* matrix, term_t list)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }
    if (!matrix->size)
    {
        DEBUG_PRINT("Dimensions pointer is null\n");
        return 0;
    }
    if (!matrix->rows)
    {
        DEBUG_PRINT("Rows pointer is null\n");
        return 0;
    }

    size_t rows = matrix->size[0];
    size_t cols = matrix->size[1];
    term_t lists = PL_new_term_refs(rows);
    
    for (int i = 0; i < rows; i++)
    {
        if (!row_to_list(*(matrix->rows + i), cols, lists + i))
        {
            DEBUG_PRINT("Error converting row to list\n");
            return 0;
        }
    }

    term_t nlist = PL_new_term_ref();
    PL_put_nil(nlist);
    
    for (int i = rows - 1; i >= 0; i--)
    {
        
        int res = PL_cons_list(nlist, lists + i, nlist);
        if (res != TRUE)
        {
            DEBUG_PRINT("Cannot construct list\n");
            return 0;
        }
    }

    return PL_unify(list, nlist);
    
}

int row_to_list(double* row, size_t length, term_t list)
{
    if (!row)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }

    PL_put_nil(list);

    term_t num = PL_new_term_ref();
    
    for (int i = length - 1; i >= 0; i--)
    {
        int res = PL_put_float(num, row[i]);
        if (res != TRUE)
        {
            DEBUG_PRINT("Cannot put float\n");
            return 0;
        }

        res = PL_cons_list(list, num, list);
        if (res != TRUE)
        {
            DEBUG_PRINT("Cannot construct list\n");
            return 0;
        }
    }
    return 1;
}


int is_squared(struct Matrix_t* matrix, int* result)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }

    if (!matrix->size)
    {
        DEBUG_PRINT("Dimensions pointer is null\n");
        return 0;
    }

    if (!result)
    {
        DEBUG_PRINT("Result pointer is null\n");
        return 0;
    }

    *result = matrix->size[0] == matrix->size[0];
    
    return 1;
}

int is_same_dimensions(struct Matrix_t* m1, struct Matrix_t* m2, int* result)
{
    if (!m1)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }    
    if (!m1->size)
    {
        DEBUG_PRINT("Dimensions pointer is null\n");
        return 0;
    }

    if (!m2)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }    
    if (!m2->size)
    {
        DEBUG_PRINT("Dimensions pointer is null\n");
        return 0;
    }

    if (!result)
    {
        DEBUG_PRINT("Result pointer is null\n");
        return 0;
    }

    *result = m1->size[0] == m2->size[0] && m1->size[1] == m2->size[1];
    return 1;
}


int sum_matrix(struct Matrix_t* matrix, double* result)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }

    if (!result)
    {
        DEBUG_PRINT("Result pointer is null\n");
        return 0;
    }

    size_t rows = *(matrix->size);
    size_t columns = *(matrix->size + 1);

    double sum = 0;

    for (int i = 0; i < rows; i++)
    {
        double* row = *(matrix->rows + i);
        for (int j = 0; j < columns; j++)
        {
            sum += *(row + j);
        }        
    }

    *result = sum;
    return 1;
}

int add_matrices(struct Matrix_t* m1, struct Matrix_t* m2, struct Matrix_t* result)
{
    if (!m1)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }
    if (!m1->size)
    {
        DEBUG_PRINT("Dimensions pointer is null\n");
        return 0;
    }
    if (!m1->rows)
    {
        DEBUG_PRINT("Rows pointer is null\n");
        return 0;
    }

    if (!m2)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }    
    if (!m2->size)
    {
        DEBUG_PRINT("Dimensions pointer is null\n");
        return 0;
    }
    if (!m2->rows)
    {
        DEBUG_PRINT("Rows pointer is null\n");
        return 0;
    }

    if (!result)
    {
        DEBUG_PRINT("Result matrix pointer is null\n");
        return 0;
    }
    if (!result->size)
    {
        DEBUG_PRINT("Result matrix dimensions pointer is null\n");
        return 0;
    }
    if (!result->rows)
    {
        DEBUG_PRINT("Result rows pointer is null\n");
        return 0;
    }


    int same_dims = 0;
    if (!is_same_dimensions(m1, m2, &same_dims))
    {
        DEBUG_PRINT("Error while checking dimensions\n");
        return 0;
    }

    if (!same_dims)
    {
        DEBUG_PRINT("Matrices do not have the same dimensions\n");
        return -1;
    }

    for (int i = 0; i < result->size[0]; i++)
    {
        for (int j = 0; j < result->size[1]; j++)
        {
            result->rows[i][j] = m1->rows[i][j] + m2->rows[i][j];
        }
    }

    return 1;
}

int substract_matrices(struct Matrix_t* m1, struct Matrix_t* m2, struct Matrix_t* result)
{
    if (!m1)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }
    if (!m1->size)
    {
        DEBUG_PRINT("Dimensions pointer is null\n");
        return 0;
    }
    if (!m1->rows)
    {
        DEBUG_PRINT("Rows pointer is null\n");
        return 0;
    }

    if (!m2)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }    
    if (!m2->size)
    {
        DEBUG_PRINT("Dimensions pointer is null\n");
        return 0;
    }
    if (!m2->rows)
    {
        DEBUG_PRINT("Rows pointer is null\n");
        return 0;
    }

    if (!result)
    {
        DEBUG_PRINT("Result matrix pointer is null\n");
        return 0;
    }
    if (!result->size)
    {
        DEBUG_PRINT("Result matrix dimensions pointer is null\n");
        return 0;
    }
    if (!result->rows)
    {
        DEBUG_PRINT("Result rows pointer is null\n");
        return 0;
    }


    int same_dims = 0;
    if (!is_same_dimensions(m1, m2, &same_dims))
    {
        DEBUG_PRINT("Error while checking dimensions\n");
        return 0;
    }

    if (!same_dims)
    {
        DEBUG_PRINT("Matrices do not have the same dimensions\n");
        return -1;
    }

    for (int i = 0; i < result->size[0]; i++)
    {
        for (int j = 0; j < result->size[1]; j++)
        {
            result->rows[i][j] = m1->rows[i][j] - m2->rows[i][j];
        }
    }
    
    return 1;
}