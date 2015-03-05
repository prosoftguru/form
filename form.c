/******************************************************************************\
*              ___           ___                  ___                          *
*             /  /          /         __         /                             *
*            /__/ ___ ___  /__  ___ _/__/_      / __        ___                *
*           /    /   /  /    / /  / /  /   _   /   / /   / /    /   /          *
*          /    /   /__/ ___/ /__/ /  /_  /_/ /___/ /___/ /    /___/           *
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

/******************************************************************************\
    Format to a user specified area.
    Variable number of arguments.
    Returns a the length of the output string (including trailing null, if any)
    or negative value on error.
\******************************************************************************/
int
sform(p, len, format, va_alist)
char *          p;                      /* where to build the string */
int             len;                    /* max length of the string to build */
char *          format;                 /* the format string */
va_dcl                                  /* variable arguments */
{
    int         ret;                    /* return value */
    va_list     ap;                     /* argument pointer */

    va_start(ap, va_alist);
    ret = vsform(p, len, format, ap);
    va_end(ap);

    return ret;
}

/******************************************************************************\
    Format into a user supplied string.
    va_list argument.
    Returns the length of the output string (including trailing null, if any)
    or negative value on error.
    Since the algorithm is basically a 'state machine', it is all included
    in this one--rather large--function.
\******************************************************************************/
int
vsform(p, len, format, ap)
char *          p;                      /* where to put the output */
int             len;                    /* maximum length of output string */
char *          format;                 /* the format string */
va_list         ap;                     /* argument list pointer */
{
    int         i, j;                   /* miscellaneous ints */
    char *      ps;                     /* saved version of 'p' */
    char *      maxp;                   /* maximum value of 'p' */
    char *      f;                      /* working pointer for format string */
    char        c;                      /* the current format character */
    int         informat = 0;           /* true if we are in a format desc */
    int *       formarg;                /* pointer to one of ???arg for formArg call */

    /********************** main loop of format 'reader' **********************/
    for (maxp = p, ps = p, f = format; (c = *f++) != 0;) {
        if (informat) {
            formarg = (int *) 0;        /* default to 'do not get arg' */
            switch (c) {
                case '(':               /* just print a '(' */
                    addchar('(');
                    informat = 0;
                    break;
                case 'u':               /* unsigned value with # bits */
                case 's':               /* signed value with # bits */
                    type = c;
                    typearg = (sizeof(int) * 8);
                    formarg = &typearg;
                    break;
                case 'f':               /* float value */
                    type = c;
                    typearg = 32;
                    formarg = &typearg;
                    break;
                case 'c':               /* character string */
                case 'C':               /* character string, strip spaces */
                    type = c;
                    typearg = 0;
                    formarg = &typearg;
                    break;
                case 'b':               /* output base */
                case 'B':               /* output base (in caps) */
                    base = c;
                    basearg = 10;
                    formarg = &basearg;
                    break;
                case 'l':               /* left justify */
                case 'r':               /* right justify */
                case 'z':               /* zero fill */
                case 'v':               /* variable */
                    just = c;
                    formarg = &justarg;
                    break;
                case 'i':               /* integer portion only */
                case '.':               /* digits after decimal */
                    prec = c;
                    formarg = &precarg;
                    break;
                case 'p':               /* move to a specific position in output string */
                    formarg = &posarg;
                    break;
                /***************** do the formatting *******************/
                case ')':
                    if (posarg != -1) {     /* tab to the proper position */
                        while ((p - ps) < posarg)
                            addchar(' ');   /* add any necessary chars */
                        p = ps + posarg;
                        posarg = -1;
                    }
                    buflen = 0;
                    switch (type) {
                        case 's':                   /* signed */
                            switch (typearg) {
                                case 8:
                                    type_s8 = va_arg(ap, s8);
                                    s2buf((s32) type_s8);
                                    break;
                                case 16:
                                    type_s16 = va_arg(ap, s16);
                                    s2buf((s32) type_s16);
                                    break;
                                case 32:
                                    type_s32 = va_arg(ap, s32);
                                    s2buf(type_s32);
                                    break;
                            }
                            break;
                        case 'u':                   /* unsigned */
                            switch (typearg) {
                                case 8:
                                    type_u8 = va_arg(ap, u8);
                                    s2buf((u32) type_u8);
                                    break;
                                case 16:
                                    type_u16 = va_arg(ap, u16);
                                    s2buf((u32) type_u16);
                                    break;
                                case 32:
                                    type_u32 = va_arg(ap, u32);
                                    s2buf(type_u32);
                                    break;
                            }
                            break;
                        case 'f':                   /* floating point */
                            switch (typearg) {
                                case 32:
                                    type_f64 = va_arg(ap, f64);
                                    f2buf(type_f64);
                                    break;
                                case 64:
                                    type_f64 = va_arg(ap,f64);
                                    f2buf(type_f64);
                                    break;
                            }
                            break;
                        case 'c':                   /* character string */
                            type_c = va_arg(ap, char *);
                            c2buf(type_c, typearg);
                            break;
                        case 'C':                   /* character string, strip spaces */
                            type_c = va_arg(ap, char *);
                            C2buf(type_c, typearg);
                            break;
                        default:
                            break;
                    }
                    /* add buf to the string with the proper justification */
                    if (justarg == 0)
                        justarg = buflen;           /* default to needed size */
                    switch (just) {
                        case 'l':                   /* left justified */
                            for (i = 0; i < buflen && i < justarg; i++)
                                addchar(buf[i]);
                            for (; i < justarg; i++)    /* add any trailing spaces */
                                addchar(' ');
                            break;
                        case 'r':                   /* right justified */
                            i = justarg - buflen;
                            while (i-- > 0)
                                addchar(' ');       /* add any leading spaces */
                            i = (justarg < buflen ? buflen - justarg : 0);
                            j = i + justarg;
                            for (; i < buflen && i < j; i++)
                                addchar(buf[i]);
                            break;
                        case 'z':                   /* zero filled */
                            i = justarg - buflen;
                            while (i-- > 0)
                                addchar('0');       /* add any leading zeroes */
                            i = (justarg < buflen ? buflen - justarg : 0);
                            j = i + justarg;
                            for (; i < buflen && i < j; i++)
                                addchar(buf[i]);
                            break;
                        case 'v':                   /* variable (no justification) */
                            for (i = 0; i < buflen; i++)
                                addchar(buf[i]);
                            break;
                    }
                    p = maxp;                       /* go back to the end of the string */
                    informat = 0;                   /* done with this format */
                    break;
                    /******************** end of 'do the formatting' *******************************/
                default:
                    break;
            }
            /* get the argument, if necessary */
            if (formarg) {
                if (*f == '*')
                    *formarg = va_arg(ap, int);     /* get the argument from the list */
                else {
                    if (*f >= '0' && *f <= '9') {
                        *formarg = 0;               /* get arg from format str */
                        while (*f >= '0' && *f <= '9') {
                            *formarg *= 10;
                            *formarg += (*f++ - '0');
                        }
                    }
                }
            }
        } else if (c == '(') {                      /* start of a new format */
            informat++;
            type = 's';                             /* reset all states */
            typearg = (sizeof(int) * 8);
            base = 'b';
            basearg = 10;
            just = 'v';
            justarg = 0;
            prec = '.';
            precarg = 0;
            posarg = -1;
        } else {                                    /* not in format and not '(' */
            addchar(c);                             /* just copy the char */
        }
    }
    if (!informat)                                  /* if currently NOT in a format ... */
        addchar('\000');                            /* add the trailing null */
    return (int) (p - ps);                          /* return the number of characters added */
}

/******************************************************************************\
    Format to a user specified file pointer.
    Variable number of arguments.
    Returns the length of the output string (including trailing null, if any)
    or a negative value on error.
\******************************************************************************/
int
fform(fp, format, va_alist)
FILE *          fp;                 /* file to print to */
char *          format;             /* the format string */
va_dcl
{
    int         ret;                /* return value */
    va_list     ap;                 /* argument pointer */

    va_start(ap, av_alist);
    ret = vfform(fp, format, ap);
    va_end(ap);
    return ret;
}

/******************************************************************************\
    Format to a user specified file pointer.
    Variable number of arguments.
    Returns the length of the output string (including trailing null, if any)
    or a negative value on error.
\******************************************************************************/
int
vfform(fp, format, ap)
FILE *              fp;                 /* file to print to */
char *              format;             /* the format string */
va_list             ap;                 /* argument list pointer */
{
    int             ret;                /* return value */
    char            wbuf[150];          /* where to format */

    ret = vsform(wbuf, sizeof(wbuf), format, ap);
    if (ret > 0) {
        if (wbuf[ret - 1] == 0)     /* don't write any null */
            ret--;
        if (fwrite(wbuf, ret, 1, fp) == 0)
            return FORM_EFWRITE;
    }
    return ret;
}

/******************************************************************************\
    Internal routine that takes a s32 arg and puts it in buf with the
    specified base.
\******************************************************************************/
static void
s2buf(value)
s32                 value;              /* the value to format */
{
    if (value < 0) {
        buf[buflen++] = '-';
        value *= -1;
    }
    u2buf((u32) value);
}

/******************************************************************************\
    Internal routine that takes a u32 arg and puts it in buf with the
    specified base.
\******************************************************************************/
static void
u2buf(value)
u32                 value;              /* the value to format */
{
    char *          digits;
    int             i;
    char            tbuf[34];

    if (basearg < 2 || basearg > 32)
        base = 10;

    digits = "0123456789abcdefghijklmnopqrstuv";
    if (base == 'B')
        digits = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    if (value) {
        for (i = sizeof(tbuf) - 1; i >= 0 && value; i--) {
            tbuf[i] = digits[value % basearg];
            value /= basearg;
        }
        for (i++; i < sizeof(tbuf); i++)
            buf[buflen++] = tbuf[i];
    } else {
        buf[0] = '0';
        buflen = 1;
    }
}

/******************************************************************************\
    Internal routine that takes a f64 arg and puts it in buf with the
    specified base and precision.
\******************************************************************************/
static void
f2buf(value)
f64                 value;              /* the value to format */
{
    int             i;
    int             first = 10;
    int             last = 9;
    char            tbuf[20];
    double          t;

    if (prec == 'i') {
        base = 'b';
        basearg = 10;
        s2buf((s32) value);
        return;
    }
    if (precarg < 0 || precarg > 10)
        precarg = 0;
    if (precarg)
        first += precarg - 1;
    if (value < 0.0) {
        buf[buflen++] = '-';
        value = fabs(value);
    }
    if (value > 1e11) {
        buf[buflen++] = '?';    /* won't fit in 20 digits, don't even try */
        return;
    }
    value = floor((value + 9e-11) * 1e10);  /* turn it into a big whole number */
    for (i = 19; i >= 0; i--) {
        t = fmod(value, 10.0);
        if (t >= 10.0 || t < 0.0) /* messed up fmod under certain conditions */
            t = 0.0;
        value = floor(value / 10.0);
        if (precarg == 0 && t && i > first)
            first = i;
        if (t && i < last)
            last = i;
        tbuf[i] = (u8)t + '0';
    }
    for (i=last; i <= first; i++) {
        if (i == 10)
            buf[buflen++] = '.';
        buf[buflen++] = tbuf[i];
    }
}

/******************************************************************************\
    Internal routine that moves a string from the source to buf with a length
    of the SMALLER of len, or strlen(p). (Unless typearg == 0).
\******************************************************************************/
static void
c2buf(p, len)
char *                  p;              /* the string to move */
int                     len;            /* the length */
{
        if (len == 0)
            len = FORM_MAX_STRING;

        while (*p && len--)
            buf[buflen++] = *p++;
}

/******************************************************************************\
    Internal routine that moves a string from the source to buf with a length
    of the SMALLER of len, or strlen(p). (Unless typearg == 0).
    Leading and trailing spaces are removed.
\******************************************************************************/
static void
C2buf(p, len)
char *                  p;              /* the string to move */
int                     len;            /* the length */
{
        if (len == 0)
            len = FORM_MAX_STRING;

        /* skip leading space */
        while (*p && *p == ' ')
            p++;

        /* copy the string */
        while (*p && len--)
            buf[buflen++] = *p++;

        /* back off trailing spaces */
        while (buflen > 0 && buf[buflen - 1] == ' ')
            buflen--;
}
