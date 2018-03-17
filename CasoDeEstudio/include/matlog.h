#ifndef __MATLOG_H__
#define __MATLOG_H__


#include <SWI-Prolog.h>
#include <types.h>
#include <stdio.h>
#include <debug.h>

int is_matrix(term_t list, size_t* dimensions);
int list_to_matrix(term_t list, struct Matrix_t* matrix);
int list_to_row(term_t list, size_t length, double* row);

double sum_matrix(struct Matrix_t* matrix);

#endif