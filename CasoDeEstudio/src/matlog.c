#include <matlog.h>
#include <string.h>

struct Matrix_t* alloc_matrix(const size_t size[static 2])
{
    if (!size)
    {
        DEBUG_PRINT("Null size pointer\n");
        return NULL;
    }

    struct Matrix_t* mat = calloc(1, sizeof(struct Matrix_t));
    if (!mat)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new matrix");
        return NULL;
    }

    memcpy(mat->size, size, sizeof(size_t) * 2);


    int rows = size[0];
    int cols = size[1];

    double** rptr = (double**) calloc(rows, sizeof(double*));

    if (!rptr)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new rows\n");
        free(mat);
        mat = NULL;
        return NULL;
    }
    else
    {
        mat->rows = rptr;
    }

    for (int i = 0; i < rows; i++)
    {
        double* cptr = (double*) calloc(cols, sizeof(double));
        if (!cptr)
        {
            DEBUG_PRINT("Tried to allocate memory, but it didnt succeed, aborting\n");
            for (int j = 0; j < i; j++)
            {
                free(*(mat->rows + i));
                *(mat->rows + i) = NULL;
            }
            free(rptr);
            rptr = NULL;
            free(mat);
            mat = NULL;
            return NULL;
        }
        else
        {
            *(mat->rows + i) = cptr;
        }
    }

    return mat;

}

struct Matrix_t* copy_matrix(const struct Matrix_t* src)
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

    struct Matrix_t* dest = alloc_matrix(src->size);

    if (!dest)
    {
        DEBUG_PRINT("Couldn't allocate memory for the new matrix\n");
        return NULL;
    }

    size_t rows = src->size[0];
    size_t cols = src->size[1];

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            dest->rows[i][j] = src->rows[i][j];
        }
    }

    return NULL;
}

/*
    Frees the memory used by a matrix pointer, any row-data NULL pointers
    will be ignored to avoid double-free memory corruption.
*/
int free_matrix(struct Matrix_t* matrix)
{
    if (!matrix)
    {
        DEBUG_PRINT("Null pointer, ignoring to avoid double-free\n");
        return 0;
    }
    if (!matrix->size)
    {
        DEBUG_PRINT("Null pointer, ignoring to avoid double-free\n");
        return 0;
    }

    for (int i = 0; i < matrix->size[0]; i++)
    {
        if (*(matrix->rows + i))
        {
            free(*(matrix->rows + i));
            *(matrix->rows + i) = NULL;
        }
    }

    free(matrix->rows);
    matrix->rows = NULL;
    
    free(matrix);
    matrix = NULL;
    
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
int is_matrix(const term_t list, size_t dimensions[static 2])
{
    if (!dimensions)
    {
        DEBUG_PRINT("Size array is null\n");
        return 0;
    }

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
        if (!PL_skip_list(list, NULLTERM, &elements)) // Get the length of the list
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

struct Matrix_t* list_to_matrix(const term_t list)
{
    size_t dims[2] = {0, 0};

    int res = is_matrix(list, dims);

    if (!res)
    {
        DEBUG_PRINT("List is not a matrix\n");
        return NULL;
    }

    struct Matrix_t* matrix = alloc_matrix(dims);

    if (!matrix)
    {
        DEBUG_PRINT("Couldnt allocate new matrix\n");
    }

    size_t rows = dims[0];
    size_t columns = dims[1];


    term_t head = PL_new_term_ref();
    term_t tail = PL_copy_term_ref(list);
    
    for (int i = 0; i < rows; i++)
    {
        if (!PL_get_list(tail, head, tail))
        {
            DEBUG_PRINT("Error while iterating through the list\n");
            return NULL;
        }
            

        double* row = *(matrix->rows + i);
        int res = -1;
        if (rows == 1) // Row matrix
        {
            res = list_to_row(list, columns, row);
        }

        if (rows != 1 || res != -1)
        {
            res = list_to_row(head, columns, row);
        }

        if (!res)
        {
            DEBUG_PRINT("Couldn't convert the list to a matrix\n");
            return NULL;
        }
    }
    return matrix;
}

int list_to_row(const term_t list, size_t length, double* row)
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

int matrix_to_list(const struct Matrix_t* matrix, term_t list)
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


int sum_elements(const struct Matrix_t* matrix, double* result)
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

int add_matrices(const struct Matrix_t* m1, const struct Matrix_t* m2, struct Matrix_t* result)
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

int substract_matrices(const struct Matrix_t* m1, const struct Matrix_t* m2, struct Matrix_t* result)
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

int multiply_matrices(const struct Matrix_t* m1, const struct Matrix_t* m2, struct Matrix_t* result)
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


    if (m1->size[1] != m2->size[0])
    {
        DEBUG_PRINT("Matrices do not compatible dimensions\n");
        return -1;
    }

    for (int i = 0; i < result->size[0]; i++)
    {
        for (int j = 0; j < result->size[1]; j++)
        {
            double res = 0;
            for (int k = 0; k < m1->size[1]; k++)
            {
                res += m1->rows[i][k] * m2->rows[k][j];
            }
            result->rows[i][j] = res;
        }
    }
    
    return 1;
}

int multiply_matrix(const struct Matrix_t* matrix, double factor, struct Matrix_t* result)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }
    if (!matrix->size)
    {
        DEBUG_PRINT("Matrix dimensions pointer is null\n");
        return 0;
    }
    if (!matrix->rows)
    {
        DEBUG_PRINT("Matrix rows pointer is null\n");
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


    int tmp = 0;
    if (!is_same_dimensions(matrix, result, &tmp))
    {
        DEBUG_PRINT("Error checking for same dimensions\n");
        return 0;
    }
    if (!tmp)
    {
        DEBUG_PRINT("Matrix do not have the same dimensions\n");
        return 0;
    }

    for (int i = 0; i < result->size[0]; i++)
    {
        for (int j = 0; j < result->size[1]; j++)
        {
            result->rows[i][j] = matrix->rows[i][j] * factor;
        }
    }

    return 1;
}

int determinant(const struct Matrix_t* matrix, double* result)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }
    if (!matrix->size)
    {
        DEBUG_PRINT("Matrix dimensions pointer is null\n");
        return 0;
    }
    if (!matrix->rows)
    {
        DEBUG_PRINT("Matrix rows pointer is null\n");
        return 0;
    }

    if (!result)
    {
        DEBUG_PRINT("Result pointer is null\n");
        return 0;
    }


    int sq = -1;
    if (!is_squared(matrix, &sq))
    {
        DEBUG_PRINT("Error when checking for squareness of matrix\n");
        return 0;
    }

    if (sq != 1)
    {
        DEBUG_PRINT("Matrix is not squared\n");
        return 0;
    }

    if (matrix->size[0] == 1)
    {
        *result = **matrix->rows;
    }
    else if (matrix->size[0] == 2)
    {
        *result = matrix->rows[0][0] * matrix->rows[1][1] - matrix->rows[0][1] * matrix->rows[1][0];
    }
    else // Apply Laplace expansion
    {
        double det = 0;
        
        for (int i = 0; i < matrix->size[0]; i++) // For every element in first row
        {
            double temp = 0;
            if (!adjoint(matrix, 0, i, &temp))
            {
                DEBUG_PRINT("Eror calculating adjoint");
                return 0;
            }
            det += temp;
        }
        *result = det;
    }
    return 1;
}

int transpose(const struct Matrix_t* matrix, struct Matrix_t* result)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }
    if (!matrix->size)
    {
        DEBUG_PRINT("Matrix dimensions pointer is null\n");
        return 0;
    }
    if (!matrix->rows)
    {
        DEBUG_PRINT("Matrix rows pointer is null\n");
        return 0;
    }


    if (matrix->size[0] != result->size[1] || matrix->size[1] != result->size[0])
    {
        DEBUG_PRINT("Incorrect result matrix size\n");
        return 0;
    }

    
    for (int i = 0; i < result->size[0]; i++)
    {
        for (int j = 0; j < result->size[1]; j++)
        {
            result->rows[i][j] = matrix->rows[j][i];
        }
    }

    return 1;
}

int inverse(const struct Matrix_t* matrix, struct Matrix_t* result)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }
    if (!matrix->size)
    {
        DEBUG_PRINT("Matrix dimensions pointer is null\n");
        return 0;
    }
    if (!matrix->rows)
    {
        DEBUG_PRINT("Matrix rows pointer is null\n");
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

    int tmp = 0;
    if (!is_same_dimensions(matrix, result, &tmp))
    {
        DEBUG_PRINT("Error checking for same dimensions\n");
        return 0;
    }
    if (!tmp)
    {
        DEBUG_PRINT("Matrix do not have the same dimensions\n");
        return 0;
    }


    int sq = -1;
    if (!is_squared(matrix, &sq))
    {
        DEBUG_PRINT("Error when checking for squareness of matrix\n");
        return 0;
    }
    if (sq != 1)
    {
        DEBUG_PRINT("Matrix is not invertible\n");
        return 0;
    }

    double det = 0;
    if (!determinant(matrix, &det))
    {
        DEBUG_PRINT("Error checking for matrix singularity\n");
        return 0;
    }
    if (det == 0)
    {
        DEBUG_PRINT("Matrix is singular and thus is not invertible\n");
        return 0;
    }

    struct Matrix_t* cofactor = alloc_matrix(matrix->size);
    if (!cofactor)
    {
        DEBUG_PRINT("Cannot allocate new memory\n");
        return 0;
    }
    
    for (int i = 0; i < cofactor->size[0]; i++)
    {
        for (int j = 0; j < cofactor->size[1]; j++)
        {
            double temp = 0;
            if (!adjoint(matrix, i, j, &temp))
            {
                DEBUG_PRINT("Error calculating adjoints\n");
                if (!free_matrix(cofactor))
                {
                    DEBUG_PRINT("Error freeing memory\n");
                }
                return 0;
            }

            cofactor->rows[i][j] = temp;
        }
    }

    struct Matrix_t* adjugate = alloc_matrix(matrix->size);
    if (!adjugate)
    {
        DEBUG_PRINT("Cannot allocate new memory\n");
        if (!free_matrix(cofactor))
        {
            DEBUG_PRINT("Error freeing memory\n");
        }
        return 0;
    }

    if (!transpose(cofactor, adjugate))
    {
        DEBUG_PRINT("Error transposing cofactor matrix\n");
        if (!free_matrix(cofactor) | !free_matrix(adjugate))
        {
            DEBUG_PRINT("Error freeing memory\n");
        }
        return 0;
    }

    if (!multiply_matrix(adjugate, 1 / det, result))
    {
        DEBUG_PRINT("Error dividing transposed matrix\n");
        if (!free_matrix(cofactor) | !free_matrix(adjugate))
        {
            DEBUG_PRINT("Error freeing memory\n");
        }
        return 0;
    }

    if (!free_matrix(cofactor) | !free_matrix(adjugate))
    {
        DEBUG_PRINT("Error freeing memory\n");
        return 0;
    }

    return 1;
}


int is_squared(const struct Matrix_t* matrix, int* result)
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

int is_same_dimensions(const struct Matrix_t* m1, const struct Matrix_t* m2, int* result)
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

int adjoint(const struct Matrix_t* matrix, int row, int col, double* result)
{
    if (!matrix)
    {
        DEBUG_PRINT("Matrix pointer is null\n");
        return 0;
    }
    if (!matrix->size)
    {
        DEBUG_PRINT("Matrix dimensions pointer is null\n");
        return 0;
    }
    if (!matrix->rows)
    {
        DEBUG_PRINT("Matrix rows pointer is null\n");
        return 0;
    }

    if (!result)
    {
        DEBUG_PRINT("Result pointer is null\n");
        return 0;
    }

    if (row < 0 || col < 0)
    {
        DEBUG_PRINT("Invalid indices\n");
        return 0;
    }

    size_t nsize[2] = {matrix->size[0] - 1, matrix->size[1] - 1};
    struct Matrix_t* det = alloc_matrix(nsize); // Temp matrix
    if (!det)
    {
        DEBUG_PRINT("Error allocating temp memory\n");
        return 0;
    }

    int counter = 0;
    for (int i = 1; i < matrix->size[0]; i++) // Create its "adjoint"
    {
        if (i == row)
            continue;
        int inner_counter = 0;
        for (int j = 0; j < matrix->size[0]; j++)
        {
            if (j == col)
                continue;

            det->rows[counter][inner_counter] = matrix->rows[i][j];
            inner_counter++;
        }
        counter++;
    }

    double temp = 0;
    if (!determinant(det, &temp))
    {
        DEBUG_PRINT("Error calculating inner determinant\n");
        if (!free_matrix(det))
        {
            DEBUG_PRINT("Error freeing memory!\n");
        }
        return 0;
    }

    if (!free_matrix(det))
    {
        DEBUG_PRINT("Error freeing memory!\n");
        return 0;
    }

    if ((row + col) % 2 == 0)
    {
        *result = temp * matrix->rows[row][col];
    }
    else
    {
        *result = temp * matrix->rows[row][col] * -1;
    }
    
    return 1;
}