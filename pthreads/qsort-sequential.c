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
#include <pthread.h>
#include <stdlib.h>
//mutex variable
pthread_mutex_t mt;

// default number of max threads
// MAXTHREADS changes
int MAXTHREADS =8;

// number of threads that ended qsort_seq
int threadsEnded=0;

//qsort sequential
void qsort_seq(int *v, int n);

// qsort parallel
void qsort_parallel(int *v, int n,int pthreads) ;

/* swap -- swap elements k and l of vector v */
void swap(int *v, int k, int l) {
  int temp = v[k];
  v[k] = v[l];
  v[l] = temp;
}

//struct params
typedef struct {
  int *v;
  int n;
} params;

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

// void * for pthreads
void *parallelthread(void* arg){
  // get params
  params *pa = (params *) arg;
  //get array: v
  int *v = pa->v;
  //get size of array: n
  int n = pa->n;

  /*
  * Check if array is larger than 4096
  * if not, we sort it sequentialy
  * because its faster.
  */
  if(n>4096){

  //mutex_lock because we change global variables
  pthread_mutex_lock(&mt);
  //decrease threads remaining
  --MAXTHREADS;
  // get MAXTHREADS as local variable
  int remaining_threads = MAXTHREADS;
  // get number of threads that ended sorting as local variable
  int threads_respawn = threadsEnded;
  // decrease global variable
  // threads_respawn does not change
  if(threadsEnded>=1)
  {
    --threadsEnded;
  }
  //mutex_unlock to continue parallel implementation
  pthread_mutex_unlock(&mt);

  // if threads remaining or threads_respawn are available, create them.
  if(remaining_threads>=0||threads_respawn>0){
  //creates a partition
  int p = partition(v,n);
  //create params 1
  params *pa1 = malloc(sizeof(params *));
  pa1->v =v;
  pa1->n = p;
  //create params 2
  params *pa2 = malloc(sizeof(params *));
  pa2->v = &v[p+1];
  pa2->n = n-p-1;
  //spawn one thread
  pthread_t thread;
  pthread_create(&thread,NULL,parallelthread,pa1);
  parallelthread(pa2);
  pthread_join(thread,NULL);
  //save memory to be faster
  free(pa1);
  free(pa2);
  }
  else
  {

  //if threads not available sorting sequential
  qsort_seq(v,n);
  //lock mutex to access global variable
  pthread_mutex_lock(&mt);
  //increase threadsEnded
  ++threadsEnded;
  //mutex_unlock to continue parallel implementation
  pthread_mutex_unlock(&mt);
  }

  }

  else
  {
  //if array too small. Do it sequential
  qsort_seq(v,n);
  //lock mutex to access global variable
  pthread_mutex_lock(&mt);
  //increase threadsEnded
  ++threadsEnded;
  //mutex_unlock to continue parallel implementation
  pthread_mutex_unlock(&mt);

  }



}

/* qsortseq -- Entry point for QuickSort */
void qsort_seq(int *v, int n) {
  if (n > 1) {
    int p = partition(v, n);
    qsort_seq(v,p);
    qsort_seq(&v[p+1],n-p-1);
  }
}



void qsort_parallel(int *v, int n,int pthreads) {
  //init mutex
  pthread_mutex_init(&mt, NULL);
  //create params to pass
  params item;
  params *p =&item;
  item.v = v;
  item.n = n;
  //change MAXTHREADS
  MAXTHREADS = pthreads;//default is 8
  //call parallel void*
  parallelthread(p);
}






//thats all my code
//thank you :)
