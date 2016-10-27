/* 
    File: semaphore.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 04/12/13

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <assert.h>
#include <pthread.h>
#include <iostream>

#include "semaphore.H"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

Semaphore::Semaphore(int _val) {
  value = _val;
  assert(pthread_mutex_init(&m, NULL) == 0);
  assert(pthread_cond_init(&c, NULL) == 0);
}

Semaphore::~Semaphore() {
  pthread_cond_destroy(&c);
  pthread_mutex_destroy(&m);
}

int Semaphore::P() {
  //  cout << "Semaphore::P(): before mutex_lock\n" << flush;
  assert(pthread_mutex_lock(&m) == 0);
  //  cout << "Semaphore::P(): after mutex_lock (value = " << value << ")\n" << flush;
  value --;
  //  cout << "Semaphore::P(): after value-- (" << value << ")\n" << flush;
  if(value < 0) 
    assert(pthread_cond_wait(&c, &m) == 0);
  assert(pthread_mutex_unlock(&m) == 0);
  return 0;
}

int Semaphore::V() {
  assert(pthread_mutex_lock(&m) == 0);
  value++;
  if (value <= 0)
    assert(pthread_cond_signal(&c) == 0);
  assert(pthread_mutex_unlock(&m) == 0);
  return 0;
}



