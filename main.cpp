#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Include for sleep

extern "C" void *thread_func(void*);
extern "C" void *thread_func2(void*);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

int main(void)
{
    int rc1, rc2;
    pthread_t thread1, thread2;

    /* Create independent threads each of which will execute functionC */

    if( (rc1=pthread_create( &thread1, NULL, &thread_func, NULL)) )
    {
        printf("Thread creation failed: %d\n", rc1);
    }

    if( (rc2=pthread_create( &thread2, NULL, &thread_func2, NULL)) )
    {
        printf("Thread creation failed: %d\n", rc2);
    }

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL); 

    printf("test thread ended\n");

    sleep(10); // Sleep for 10 seconds to allow time for attaching the debugger
    return 0;
}

void *thread_func(void*)
{
    pthread_mutex_lock( &mutex1 );
    counter++;
    printf("Counter value: %d\n",counter);
    pthread_mutex_unlock( &mutex1 );
    return 0;
}

void *thread_func2(void*)
{
    pthread_mutex_lock( &mutex1 );
    counter++;
    printf("Counter value: %d\n",counter);
    pthread_mutex_unlock( &mutex1 );
    return 0;
}