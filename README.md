# Library for manipulating matrices in prolog
Built in C and C++11 for SWI-Prolog interpreter.
Imported from one of my other repositories.
## Opeartions supported

- is_squared(+M). Returns TRUE if the matrix is squared.
- same_dimensions(+M1, +M2). Returns TRUE if both matrices have the same dimensions.
- mat_sum(+M1, -d). Unifies d with the sum of the elements of M1.
- sum_mat(+M1, +M2, -M3). Unifies M3 with the sum of M1 and M2.
- res_mat(+M1, +M2, -M3). Unifies M3 with the substarction of M1 and M2.
- mult_mat(+M1, +M2, -M3). Unifies M3 with the multiplication of M1 and M2.
- mult_mat_factor(+M1, +d, -M2). Unifies M3 with the multiplication of M1 times a double d.
- transpose(+M1, -M2). Unifies M2 with the result of transposing M1.
- determinant(+M1, -d). Unifies d with the determinant of M1, if it has it.
- inv_mat(+M1, -M2). Unifies M2 with the inverse of M1, if it has it.

## Compiling and linking
Inside each folder there is a makefile. Inside that directory you can run make and a dynamic library will be generated.
Simply import it from prolog using load_foreign_library/1.

