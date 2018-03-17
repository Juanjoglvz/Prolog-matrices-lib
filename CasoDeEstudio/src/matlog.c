#include <matlog.h>

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
        if (!PL_skip_list(list, NULLTERM, &rows))
        {
            DEBUG_PRINT("Error while getting the list length\n");
            return 0;
        }
        else
        {
            term_t head = PL_new_term_ref();
	        term_t tail = PL_copy_term_ref(list);

            for (int i = 0; i < rows; i++)
            {
                if (!PL_get_list(tail, head, tail))
                {
                    DEBUG_PRINT("Error while iterating through the list\n");
                    return 0;
                }

                if (PL_is_list(head))
                {
                    // Cannot have a row matrix with nested lists 
                    // (that would be multiple rows)
                    if (row_matrix) 
                    {
                        DEBUG_PRINT("row_matrix is true\n");
                        return 0;
                    }

                    size_t cols = (size_t) is_matrix(head, NULL);
                    if (columns == 0)
                    {
                        columns = cols;
                    }

                    if (cols == 0 || cols != columns)
                    {
                        DEBUG_PRINT("Different number of columns, %lu vs %lu\n", cols, columns);
                        return 0;
                    }
                }
                else // Row Matrix
                {
                    if (PL_is_number(head))
                    {
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
        if (rows > 1)
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

double sum_matrix(struct Matrix_t* matrix)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
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

    return sum;
}