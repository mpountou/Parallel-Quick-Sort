/**********************************************************************
 *
 * qsort-sequential.c -- Sequential implementation of QuickSort
 *
 * Nikos Pitsianis <nikos.pitsianis@eng.auth.gr>
 * Dimitris Floros <fcdimitr@auth.gr>
 * Time-stamp: <2018-10-10>
 *
 **********************************************************************/


#include <stdio.h>
#include <omp.h>



/* swap -- swap elements k and l of vector v */
void swap(int *v, int k, int l) {
  int temp = v[k];
  v[k] = v[l];
  v[l] = temp;
}


/* partition -- in-place update of elements */
int partition(int *v, int n) {
  int pivot = v[n-1];
  int i = 0;

  for (int j = 0; j < n - 1; j++)
    if (v[j] < pivot)
      swap(v,i++,j);

  swap(v, i, n - 1);
  return (i);
}



void qsort_seq(int *v, int n) {
  if (n > 1) {
    int p = partition(v, n);
    if(n>4000){
    #pragma omp task
    {
    qsort_seq(v,p);
    }
    qsort_seq(&v[p+1],n-p-1);
    }
    else{
     qsort_seq(v,p);
     qsort_seq(&v[p+1],n-p-1);
    }  
}
}
