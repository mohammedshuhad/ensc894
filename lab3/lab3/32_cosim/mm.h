#ifndef KERNERL_GEMM_H
#define KERNERL_GEMM_H

#include <stdio.h>
#include <stdlib.h>

#define dimension 32
#define NI dimension
#define NJ dimension
#define NK dimension

#define N 8

void mm(float C[NI*NJ], float A[NI*NK], float B[NK*NJ], float alpha, float beta);

#endif