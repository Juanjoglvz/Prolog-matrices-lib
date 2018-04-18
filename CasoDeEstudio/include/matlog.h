#ifndef __MATLOG_H__
#define __MATLOG_H__


#include <SWI-Prolog.h>
#include <types.h>
#include <stdio.h>
#include <debug.h>

struct Matrix_t* alloc_matrix(const size_t size[static 2]);
struct Matrix_t* copy_matrix(const struct Matrix_t* src);
int free_matrix(struct Matrix_t* matrix);

int is_matrix(const term_t list, size_t dimensions[static 2]);
struct Matrix_t* list_to_matrix(const term_t list);
int list_to_row(const term_t list, size_t length, double* row);
int matrix_to_list(const struct Matrix_t* matrix, term_t list);
int row_to_list(double* row, size_t length, term_t list);

int sum_elements(const struct Matrix_t* matrix, double* result);
int add_matrices(const struct Matrix_t* m1, const struct Matrix_t* m2, struct Matrix_t* result);
int substract_matrices(const struct Matrix_t* m1, const struct Matrix_t* m2, struct Matrix_t* result);
int multiply_matrix(const struct Matrix_t* m1, double factor, struct Matrix_t* result);
int multiply_matrices(const struct Matrix_t* m1, const struct Matrix_t* m2, struct Matrix_t* result);
int determinant(const struct Matrix_t* matrix, double* result);
int transpose(const struct Matrix_t* matrix, struct Matrix_t* result);
int inverse(const struct Matrix_t* matrix, struct Matrix_t* result);

int is_squared(const struct Matrix_t* matrix, int* result);
int is_same_dimensions(const struct Matrix_t* m1, const struct Matrix_t* m2, int* result);
int minor(const struct Matrix_t* matrix, int row, int col, double* result);
#endif