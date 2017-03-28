/****************************** myBoolean.h ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

A very basic boolean type

****************************************************************************/

#ifndef __MY_BOOLEAN_H__
#define __MY_BOOLEAN_H__


/* create boolean */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define TOGGLE(a) a = (a + 1) % 2

typedef int bool;

#endif

