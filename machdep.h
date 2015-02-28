/******************************************************************************\
*               ___           ___                                              *                
*              /  /          /         __                                      *
*             /__/ ___ ___  /__  ___ _/__/_    ___         ___                 *
*            /    /   /  /    / /  / /  /     / __  /   / /__/ /   /           *
*           /    /   /__/ ___/ /__/ /  /_  o /___/ /___/ / \  /___/            *
*                                                                              *
*           Copyright (c) 1991 by Jeff Fisher @ http://prosoft.guru            *
*                                                                              *
\******************************************************************************/

/************************ INCLUDE FILE DESCRIPTION *****************************
    
    Machine dependent macros and definitions.

*************************** MODIFICATION HISTORY *******************************

        DATE    WHO     DESCRIPTION
        ----    ---     ------------------------------------------------
        7/90    JBF     Initial Coding

********************** MACHINE DEPENDENT INCLUDE FILES ************************/

#ifdef ANSI
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <math.h>
#include <stdio.h>

/******************************** CONSTANTS ***********************************/

#define u8              unsigned char
#define s8              char
#define u16             unsigned short
#define u32             unsigned long
#define f32             float
#define f64             double

/********************************** MACROS ************************************/

/* sort out incompatibilities between ANSI and non ANSI varargs. */
#ifdef ANSI
#define va_alist        ...
#define va_dcl
#else
#undef  va_start
#define va_start(list, foo) list = (char *) &va_alist
#endif

/************************* STRUCTURE DECLARATIONS *****************************/

/********************** GLOBAL VARIABLE DECLARATIONS **************************/

/********************** GLOBAL FUNCTION DECLARATIONS **************************/

extern char *           form();     /* format, ... */
extern char *           vform();    /* format, ap */
extern int              sform();    /* out, len, format, ... */
extern int              vsform();   /* out, len, format, ap */
extern int              fform();    /* fp, format, ap */
