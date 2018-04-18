#include "Matrix.hpp"

Matrix::Matrix(int rows, int cols, std::vector<double> content) : rows(rows), cols(cols)
{
    this->content = content;
}

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols)
{
}

int Matrix::getcols() const
{
    return this->cols;
}

int Matrix::getrows() const
{
    return this->rows;
}

std::vector<double> Matrix::getcontent() const
{
    return this->content;
}

int Matrix::isValid() const
{
    return this->cols * this->rows == this->content.size();
}

int Matrix::addRow(std::vector<double> row)
{
    if (row.size() != cols)
    {
        return 0;
    }
    
    for (double element : row)
    {
        content.push_back(element);
    }
    
    return 1;
}

double Matrix::operator()(size_t i, size_t j) const
{
    if (i >= this->rows || j >= this->cols)
    {
        throw std::out_of_range("Index out of bounds");
    }
    return content[i * cols + j];
}


std::vector<double> Matrix::operator+(const Matrix m2) const
{
    std::vector<double> retval;
    if (!this->same_dimensions(m2))
    {
        std::cout << "Incompatible dimensions: Trying to add " <<
                  rows << " " << cols << " with " << m2.getrows() << " "
                  << m2.getcols() << std::endl;
        return retval;
    }

    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            retval.push_back((*this)(i, j) + m2(i, j));
        }
    }

    return retval;
}

std::vector<double> Matrix::operator-(const Matrix m2) const
{
    std::vector<double> retval;
    if (!this->same_dimensions(m2))
    {
        std::cout << "Incompatible dimensions: Trying to substract " <<
                  rows << " " << cols << " with " << m2.getrows() << " "
                  << m2.getcols() << std::endl;
        return retval;
    }

    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            retval.push_back((*this)(i, j) - m2(i, j));
        }
    }

    return retval;
}

std::vector<double> Matrix::operator*(const double factor) const
{
    std::vector<double> retval;

    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            retval.push_back((*this)(i, j) * factor);
        }
    }

    return retval;
}

std::vector<double> Matrix::operator*(const Matrix m2) const
{
    std::vector<double> retval;

    if (this->cols != m2.getrows())
    {
        std::cout << "Incompatible dimensions: Trying to multiplicate " <<
                  rows << " " << cols << " with " << m2.getrows() << " "
                                                                     << m2.getcols() << std::endl;
        return retval;
    }

    for (int i = 0; i < this->getrows(); i++)
    {
        for (int j = 0; j < m2.getcols(); j++)
        {
            double sum = 0;
            for (int k = 0; k < this->getcols(); k++)
            {
                sum += (*this)(i, k) * m2(k, j);
            }
            retval.push_back(sum);
        }
    }


    return retval;
}

void Matrix::getrow(int row, double retval[]) const
{
    for (int i = 0; i < cols; i++)
    {
        retval[i] = content[row * cols + i];
    }
}

int Matrix::isSquared() const
{
	return this->cols == this->rows;
}

int Matrix::same_dimensions(const Matrix m2) const
{
	return this->cols == m2.getcols() && this->rows == m2.getrows();
}

const double Matrix::sum() const
{
	double retval = 0;
	
	for (double element : this->content)
	{
		retval += element;
	}
	
	return retval;
}

std::vector<double> Matrix::transpose() const
{
    std::vector<double> retval;

    for (int i = 0; i < this->cols; i++)
    {
        for (int j = 0; j < this->rows; j++)
        {
            retval.push_back((*this)(j, i));
        }
    }

    return retval;
}

double Matrix::determinant() const
{
    double retval = 0;

    if (!this->isSquared())
    {
        std::cout << "The matrix is not squared" << std::endl;
        return (std::numeric_limits<double>::max() * -1);
    }

    if (this->rows == 1)
    {
        retval = (*this)(0, 0);
    }
    else if (this->rows == 2)
    {
        retval = (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
    }
    else
    {
        for (int p = 0; p < this->rows; p++)
        {
            std::vector<double> aux;
            for (int i = 1; i < this->rows; i++)
            {
                for (int j = 0; j < this->rows; j++)
                {
                    if (j == p)
                    {
                        continue;
                    }

                    aux.push_back((*this)(i,j));
                }
            }
            Matrix* foo = new Matrix(this->rows - 1, this->cols - 1, aux);
            retval += (*this)(0, p) * pow(-1, p) *
                      foo->determinant();

        }
    }

    return retval;

}

std::vector<double> Matrix::inverse() const
{
    std::vector<double> retval;
    if (!this->isSquared())
    {
        std::cout << "The matrix is not squared" << std::endl;
        return retval;
    }

    double det = this->determinant();
    if (det == 0.0)
    {
        std::cout << "Matrix determinant is 0" << std::endl;
        return retval;
    }

    std::cout << "Attempting to get comatrix with det " << det << std::endl;

    std::vector<double> cof;

    for (int i = 0; i <  this->rows; i++)
    {
        for (int j = 0; j < this->rows; j++)
        {
            cof.push_back(this->adjoint(i, j));
        }
    }



    Matrix* foo = new Matrix(this->rows, this->rows, cof);

    std::cout << "Attempting to get adjugate: " << std::endl;
    foo->print();

    Matrix* foo2 = new Matrix(this->rows, this->rows, foo->transpose());

    std::cout << "adjugate: " << std::endl;

    foo2->print();

    return (*foo2) * (1/det);
}

double Matrix::adjoint(int row, int col) const
{
    std::vector<double> aux;

    int counter = 0;

    for (int i = 0; i < this->rows; i++)
    {
        if (i == row)
        {
            continue;
        }
        for (int j = 0; j < this->rows; j++)
        {
            if (j == col)
            {
                continue;
            }
            std::cout << "pushing: " << (*this)(i,j) << std::endl;
            aux.push_back((*this)(i,j));
        }
    }
    Matrix* foo = new Matrix(this->rows - 1, this->cols - 1, aux);

    std::cout << "got: " << std::endl;
    foo->print();

    if ((row + col) % 2 == 0)
    {
        return foo->determinant();
    }
    else
    {
        return foo->determinant() * -1;
    }
}

void Matrix::print() const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << content[cols * i + j] << " ";
        }
        std::cout << std::endl;
    }
}
