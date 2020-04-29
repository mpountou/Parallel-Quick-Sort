/**********************************************************************
 *
 * qsort.c -- Sequential implementation of QuickSort
 *
 * Nikos Pitsianis <nikos.pitsianis@eng.auth.gr>
 * Dimitris Floros <fcdimitr@auth.gr>
 * Time-stamp: <2018-10-10>
 *
 **********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "qsort-sequential.h"
#include <assert.h>
#include <omp.h>
#include <math.h>
//function my_pow returns: x^y
int my_pow(int x, int y);
/* local function declarations */
//verify sort results
int  test( int *a, int n);
//init array
void init( int *a, int n);
//print array
void print(int *a, int n);
//function my_pow returns: x^y
int my_pow(int x,int y){
    int result;
    if(y==0){
      result = 1;
    }
    else{
    result=x;
    while(y>1){
    result = result*x;
    --y;
    }

  }
  return result;
}



/* --- Entry POINT --- */
int main() {

  /* parse input */
  int q, p;
  //scanf q and p
  printf("Enter q:");
  //ask user to enter q
  scanf("%d", &q);
  //check if input is valid
  while (q<12||q>24) {
    printf("Invalid q \nEnter q between [12,24]\n");
    scanf("%d", &q);
  }
  //ask user for p
  printf("Enter p:");
  //get p
  scanf("%d", &p);
  //check if p is valid
  while (p<0||p>8) {
    printf("Invalid p\nEnter p between [0,8]\n");
    scanf("%d", &p);
  }
  /* variables to hold execution time */
  struct timeval startwtime, endwtime;
  double seq_time;

  /* initiate vector of random integerts */
  int n  =   my_pow(2, q);

  int *a = (int *) malloc(n * sizeof(int));

  /* initialize vector */
  init(a, n);
  //number of threads to use
  int threads_openm =  my_pow(2,p);// threads_openm = 2^p

//Change num of threads here:
//
//
//
//
//
  omp_set_num_threads(threads_openm);
//
//
//
//
//
//Change num of threads here:


  /* sort elements in original order */
  gettimeofday (&startwtime, NULL);
  #pragma omp parallel
      {
      #pragma omp single
          {
  qsort_seq(a, n);
        }
  }
  gettimeofday (&endwtime, NULL);

  /* get time in seconds */
  seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
                      + endwtime.tv_sec - startwtime.tv_sec);

  /* validate result */
  int pass = test(a, n);
  printf(" TEST %s\n",(pass) ? "PASSed" : "FAILed");
//  print(a,n);
  assert( pass != 0 );

    /* print sorted vector */
  /* print(a, n); */

  /* print execution time */
  printf("Sequential wall clock time: %f sec\n", seq_time);

  /* exit */
  return 0;

}

/** -------------- SUB-PROCEDURES  ----------------- **/

/** procedure test() : verify sort results **/
int test(int *a, int n) {

  // TODO: implement
  int pass = 1;
  for(int i=0;i<n-1;i++)
  if(a[i]>a[i+1]){
    pass=0;
  }

  return pass;

}

/** procedure init() : initialize array "a" with data **/
void init(int *a, int n) {
  int i;
  for (i = 0; i < n; i++) {
    a[i] = rand() % n; // (N - i);
  }
}

/** procedure  print() : print array elements **/
void print(int *a, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
}
