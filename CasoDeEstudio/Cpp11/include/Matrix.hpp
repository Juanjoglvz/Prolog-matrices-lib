
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <math.h>

class Matrix
{
public:
    Matrix(int rows, int cols, std::vector<double> content);
    Matrix(int rows, int cols);
    
    int getcols() const;
    int getrows() const;
    std::vector<double> getcontent() const;
    
    int addRow(std::vector<double> row);
	
	const double sum() const;
	std::vector<double> transpose() const;
	double determinant() const;
	std::vector<double> inverse() const;

	double adjoint(int row, int col) const;

    double operator()(size_t i, size_t j) const;
    std::vector<double> operator+(const Matrix m2) const;
    std::vector<double> operator-(const Matrix m2) const;
    std::vector<double> operator*(const double factor) const;
    std::vector<double> operator*(const Matrix m2) const;

    void getrow(int row, double retval[]) const;
    
    int isValid() const;
	int isSquared() const;
	int same_dimensions(const Matrix m2) const;

	void print() const;
private:
    std::vector<double> content;
    const int cols;
    const int rows;
    
};
