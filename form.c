/******************************************************************************\
*               ___           ___                                              *
*              /  /          /         __                                      *
*             /__/ ___ ___  /__  ___ _/__/_    ___         ___                 *
*            /    /   /  /    / /  / /  /     / __  /   / /__/ /   /           *
*           /    /   /__/ ___/ /__/ /  /_  o /___/ /___/ / \  /___/            *
*                                                                              *
\******************************************************************************/

/******************************* COPYRIGHT *************************************

    Copyright 1991-2015 Jeff Fisher @ http://prosoft.guru

    This file is part of FORM by prosoft.guru.

    FORM is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License, version 3, as published by the
    Free Software Foundation.

    FORM is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.

    You should have received a copy of the GNU General Public License along
    with FORM. If not, see http://www.gnu.org/licenses/.

*************************** PROGRAM DESCRIPTION ********************************
    
    General purpose text string formatting routine.
    See form(1) for documentation.

*************************** MODIFICATION HISTORY *******************************

        DATE    WHO     DESCRIPTION
        ----    ---     ------------------------------------------------
        4/91    JBF     Initial Coding
        2/15    JBF     Typed back in from an old printout

****************************** INCLUDE FILES **********************************/

#include "machdep.h"
#include "form.h"

/**************************** GLOBAL VARIABLES ********************************/

/********************** STATIC VARIABLE DECLARATIONS **************************/

static char     buf[FORM_MAX_STRING];   /* intermediate buffer */
static int      buflen;                 /* and its current length */
static s8       type_s8;                /* types for holding things */
static s16      type_s16;
static s32      type_s32;
static u8       type_u8;
static u16      type_u16;
static u32      type_u32;
static f64      type_f64;
static char *   type_c;

/* state used during formatting */
static char     type;                   /* default type */
static int      typearg;                /* default type arg */
static char     base;                   /* default base */
static int      basearg;                /* default base arg */
static char     just;                   /* default justification */
static int      justarg;                /* default justification arg */
static char     prec;                   /* default precision */
static int      precarg;                /* default precision arg */
static int      posarg;                 /* change position argument */

/********************** STATIC FUNCTION DECLARATIONS **************************/

static void     s2buf();
static void     u2buf();
static void     f2buf();
static void     c2buf();
static void     C2buf();

/***************************** STATIC MACROS **********************************/

/* macro to add a character to the string with error checking */
#define addchar(c)      if (p - ps > len) { \
                            return FORM_ESIZE; \
                        } else { \
                            *p++ = (c); \
                            if (p > maxp) \
                                maxp = p; \
                        }

/******************************************************************************\
    Format to static buffer area.
    Variable number of arguments.
    Returns a pointer to the beginning of the null-terminated string
    or 0 on error.
\******************************************************************************/
char *
form(format, va_alist)
    char *      format;                 /* the format string */
    va_dcl                              /* variable number of arguments */
{
    char        *ret;                   /* the return value */
    va_list     ap;                     /* argument pointer */

    va_start(ap, va_alist);
    ret = vform(format, ap);
    va_end(ap);

    return ret;
}

/******************************************************************************\
    Format to static buffer area.
    va_list argument.
    Returns a pointer to the beginning of the null-terminated string
    or 0 on error.
\******************************************************************************/
char *
vform(format, ap)
char *          format;                 /* the format string */
va_list         ap;                     /* argument pointer */
{
    int         ret;
    char *      p;                      /* pointer into buf */
    static char sbuf[1000];             /* put formatted strings here */
    static int  i;                      /* current position */

    p = &sbuf[i];                       /* set up pointer */
    ret = vsform(p, (int)sizeof(sbuf) - 1, format, ap);

    if (ret == FORM_ESIZE) {            /* if not enough room... */
        i = 0;                          /* reset pointer */
        p = &sbuf[i];                   /* set up pointer again */
        ret = vsform(p, (int)(sizeof(sbuf) - i), format, ap);   /* and try again */
        if (ret == FORM_ESIZE)          /* if still not enough room... */
            return (char *) 0;          /* return error */
    }
    i += ret;                           /* bump to next position */

    if (sbuf[i - 1] != 0)               /* force it to be null term */
        sbuf[i++] = 0;

    return p;
}
