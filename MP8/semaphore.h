/* 
    File: semaphore.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/11

*/

#ifndef _semaphore_H_                   // include file only once
#define _semaphore_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <pthread.h>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   s e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class semaphore {
private:
    /* -- INTERNAL DATA STRUCTURES */

    int             value;
    pthread_mutex_t value_access_lock;
    pthread_cond_t  wait_queue;

public:

    /* -- CONSTRUCTOR/DESTRUCTOR */

    semaphore(int _val) {
		value = _val;
		pthread_cond_init(&wait_queue, NULL);
		pthread_mutex_init(&value_access_lock, NULL);
	}

    ~semaphore(){
        pthread_cond_destroy(&wait_queue);
        pthread_mutex_destroy(&value_access_lock);
        value = 0;
    }

    /* -- SEMAPHORE OPERATIONS */

    void setValue(int _val) { value = _val; }
    
    void P() {
        pthread_mutex_lock(&value_access_lock);
        --value;
        if(value < 0) pthread_cond_wait(&wait_queue, &value_access_lock);
        pthread_mutex_unlock(&value_access_lock);
    }
    void V() {
        pthread_mutex_lock(&value_access_lock);
        ++value;
        if(value <= 0){ pthread_cond_signal(&wait_queue); }
        pthread_mutex_unlock(&value_access_lock);
    }
};

#endif


