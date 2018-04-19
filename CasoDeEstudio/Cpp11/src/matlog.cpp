#include "SWI-cpp.h"
#include <vector>
#include <memory>
#include <limits>
#include "SWI-Prolog.h"
#include <Matrix.hpp>

Matrix* list_to_Matrix(PlTerm term);
Matrix* list_to_matrix_one_row(PlTerm term);
int row_to_list(double *row, size_t length, term_t list);
int matrix_to_list(term_t list, Matrix* matrix);

PREDICATE(is_squared, 1)
{
	std::unique_ptr<Matrix> m1(list_to_Matrix(A1));
	
	if (m1.get() == nullptr || !m1->isSquared())
	{
		return FALSE;
	}
	
	return TRUE;
}

PREDICATE(same_dimensions, 2)
{
	std::unique_ptr<Matrix> m1(list_to_Matrix(A1));
    std::unique_ptr<Matrix> m2(list_to_Matrix(A2));
	
	if (m1.get() == nullptr || m2.get() == nullptr || !m1->same_dimensions(*m2))
    {
        return FALSE;
    }
	
	return TRUE;
}


PREDICATE(mat_sum, 2)
{
    std::unique_ptr<Matrix> m1(list_to_Matrix(A1));
	
	if (m1.get() == nullptr)
	{
		return FALSE;
	}
	
	return A2 = m1->sum();
}

PREDICATE(sum_mat, 3)
{
    std::unique_ptr<Matrix> m1(list_to_Matrix(A1));
    std::unique_ptr<Matrix> m2(list_to_Matrix(A2));

    if (m1.get() == nullptr || m2.get() == nullptr)
    {
        return FALSE;
    }

    //Get new matrix from the sum
    
    std::unique_ptr<Matrix> m_result(new Matrix(m1->getrows(), m1->getcols(), *m1 + *m2));

    if (!m_result->isValid())
    {
        return FALSE;
    }
    
    //C code to build the new list

    term_t nlist = PL_new_term_ref();

    matrix_to_list(nlist, m_result.get());
    
    //End of C code

    //Unify result

    PlTerm result(nlist);
    
    return A3 = result;
}

PREDICATE(res_mat, 3)
{
    std::unique_ptr<Matrix> m1(list_to_Matrix(A1));
    std::unique_ptr<Matrix> m2(list_to_Matrix(A2));

    if (m1.get() == nullptr || m2.get() == nullptr)
    {
        return FALSE;
    }

    //Get new matrix from the sum

    std::unique_ptr<Matrix> m_result(new Matrix(m1->getrows(), m1->getcols(), *m1 - *m2));
    if (!m_result->isValid())
    {
        return FALSE;
    }

    //C code to build the new list

    term_t nlist = PL_new_term_ref();

    matrix_to_list(nlist, m_result.get());

    //End of C code

    //Unify result

    PlTerm result(nlist);

    return A3 = result;
}

PREDICATE(mult_mat_factor, 3)
{
    std::unique_ptr<Matrix> m(list_to_Matrix(A1));

    if (m.get() == nullptr)
    {
        return FALSE;
    }

    double factor;

    try
    {
        factor = (double) A2;
    }
    catch(...)
    {
        std::cout << "Second argument must be a double" << std::endl;
        return FALSE;
    }

    std::unique_ptr<Matrix> m_result(new Matrix(m->getrows(), m->getcols(), *m * factor));

    if (!m_result->isValid())
    {
        return FALSE;
    }

    //C code to build the new list

    term_t nlist = PL_new_term_ref();

    matrix_to_list(nlist, m_result.get());

    //End of C code

    //Unify result

    PlTerm result(nlist);
	
    return A3 = result;
}

PREDICATE(mult_mat, 3)
{
    std::unique_ptr<Matrix> m1(list_to_Matrix(A1));
    std::unique_ptr<Matrix> m2(list_to_Matrix(A2));

    if (m1.get() == nullptr || m2.get() == nullptr)
    {
        return FALSE;
    }


    std::unique_ptr<Matrix> m_result(new Matrix(m1->getrows(), m2->getcols(), *m1 * *m2));

    if (!m_result->isValid())
    {
        return FALSE;
    }

    //C code to build the new list

    term_t nlist = PL_new_term_ref();

    matrix_to_list(nlist, m_result.get());

    //End of C code

    //Unify result

    PlTerm result(nlist);

    return A3 = result;
}

PREDICATE(transpose_mat, 2)
{
    std::unique_ptr<Matrix> m(list_to_Matrix(A1));

    if (m == nullptr)
    {
        return FALSE;
    }

    std::unique_ptr<Matrix> m_result(new Matrix(m->getcols(), m->getrows(), m->transpose()));

    if (!m_result->isValid())
    {
        return FALSE;
    }

    //C code to build the new list

    term_t nlist = PL_new_term_ref();

    matrix_to_list(nlist, m_result.get());

    //End of C code

    //Unify result

    PlTerm result(nlist);

    return A2 = result;
}

PREDICATE(determinant_mat, 2)
{
    std::unique_ptr<Matrix> m(list_to_Matrix(A1));

    if (m.get() == nullptr)
    {
        return FALSE;
    }

    double factor = m->determinant();

    if (factor == (std::numeric_limits<double>::max() * -1))
    {
        return FALSE;
    }

    return A2 = factor;
}

PREDICATE(inv_mat, 2)
{
    std::unique_ptr<Matrix> m(list_to_Matrix(A1));

    if (m == nullptr)
    {
        return FALSE;
    }

    std::unique_ptr<Matrix> m_result(new Matrix(m->getcols(), m->getrows(), m->inverse()));

    if (!m_result->isValid())
    {
        return FALSE;
    }

    //C code to build the new list

    term_t nlist = PL_new_term_ref();

    matrix_to_list(nlist, m_result.get());

    //End of C code

    //Unify result

    PlTerm result(nlist);

    return A2 = result;
}

Matrix* list_to_Matrix(PlTerm term)
{
    int rows = 0, cols = -1;
    int i = 0, j;
    std::vector<double> content;
    try {
        PlTail list(term);


        PlTerm e1;

        while (list.next(e1)) {
            j = 0;
            PlTerm e2;

            try
            {
                PlTail list_row(e1);
                while (list_row.next(e2)) {
                    content.push_back((double) e2);
                    j++;
                }
                //Check if matrix is well formed
                if (cols != -1 && cols != j)
                {
                    throw std::runtime_error(0);
                }
            }
            catch (PlTypeError &ex)
            {
                return list_to_matrix_one_row(term);
            }
            cols = j;
            i++;
        }
    }
    catch (...)
    {
        std::cout << "Invalid matrix" << std::endl;
        return nullptr;
    }

    rows = i;
    cols = j;

    return new Matrix(rows, cols, content);
}

Matrix* list_to_matrix_one_row(PlTerm term)
{
    int rows = 0, cols = 0;
    int j = 0;
    std::vector<double> content;
    try {
        PlTail list(term);


        PlTerm e1;

        while (list.next(e1)) {
            content.push_back((double) e1);
            j++;
        }
    }
    catch (...)
    {
        std::cout << "Invalid matrix" << std::endl;
        return nullptr;
    }

    rows = 1;
    cols = j;

    return new Matrix(rows, cols, content);
}


//Function written in C and using C interface
//Receives a prolog list and a Matrix
//Converts the reference of the list into the matrix
int matrix_to_list(term_t list, Matrix* matrix)
{
    size_t rows = matrix->getrows();
    size_t cols = matrix->getcols();

    term_t lists = PL_new_term_refs(rows);

    for (int i = 0; i < rows; i++)
    {
        double row_from_matrix[cols];
        matrix->getrow(i, row_from_matrix);
        if (!row_to_list(row_from_matrix, cols, lists + i))
        {
            return 0;
        }
    }

    PL_put_nil(list);

    for (int i = rows - 1; i >= 0; i--)
    {
        if(!PL_cons_list(list, lists + i, list))
        {
            return 0;
        }
    }

    return 1;
}

//Function written in C and using C interface
//Receives a matrix row and a prolog list, needs the length of the row
//Converts the list into the matrix row
int row_to_list(double *row, size_t length, term_t list)
{
    //Taken from matlog.c
    if (!row)
    {
        return 0;
    }
    
    PL_put_nil(list);
    
    term_t num = PL_new_term_ref();
    
    for (int i = length - 1; i >= 0; i--)
    {
        int res = PL_put_float(num, row[i]);
        if (res != TRUE){
            return 0;
        }
        
        res = PL_cons_list(list, num, list);
        if (res != TRUE)
        {
            return 0;
        }
    }
    
    return 1;
}

