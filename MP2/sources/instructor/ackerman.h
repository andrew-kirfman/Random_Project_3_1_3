/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./ackerman.h                                                       */
/* ------------------------------------------------------------------------ */

// Include guard to ensure that the file is included only once
#ifndef _ackerman_h_
#define _ackerman_h_

/* ------------------------------------------------------------------------ */
/* Ackerman Module                                                          */
/* ------------------------------------------------------------------------ */

/* 
 * Asks user for parameters n and m and computes the result of the
 * (highly recursive!) ackerman function. During every recursion step,
 * it allocates and de-allocates a portion of memory with the use of the
 * memory allocator defined in module "my_allocator.H".
 */ 
extern void ackerman_main();

extern int ackerman_main(int input_n, int input_m);

#endif
