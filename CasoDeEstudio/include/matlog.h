#ifndef __MATLOG_H__
#define __MATLOG_H__


#include <SWI-Prolog.h>
#include <types.h>
#include <stdio.h>
#include <debug.h>

int copy_matrix(struct Matrix_t* src, struct Matrix_t* dest);

int is_matrix(term_t list, size_t* dimensions);
int list_to_matrix(term_t list, struct Matrix_t* matrix);
int list_to_row(term_t list, size_t length, double* row);
int matrix_to_list(struct Matrix_t* matrix, term_t list);
int row_to_list(double* row, size_t length, term_t list);

int is_squared(struct Matrix_t* matrix, int* result);
int is_same_dimensions(struct Matrix_t* m1, struct Matrix_t* m2, int* result);
int sum_matrix(struct Matrix_t* matrix, double* result);
int add_matrices(struct Matrix_t* m1, struct Matrix_t* m2, struct Matrix_t* result);
int substract_matrices(struct Matrix_t* m1, struct Matrix_t* m2, struct Matrix_t* result);

#endif