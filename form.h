/******************************************************************************\
*                           ___           ___                                  *                
*                          /  /          /         __                          *
*                         /__/ ___ ___  /__  ___ _/__/_                        *
*                        /    /   /  /    / /  / /  /                          *
*                       /    /   /__/ ___/ /__/ /  /_                          *
*                           ___          ___                                   *
*                         / __   /   /  /__/ /   /                             *
*                        /___/  /___/  / \  /___/                              *
*                                                                              *
*           Copyright (c) 1991 by Jeff Fisher @ http://prosoft.guru            *
*                                                                              *
\******************************************************************************/

/************************ INCLUDE FILE DESCRIPTION *****************************
    
    Include file for general purpose text string formatting routine. (form.c)

*************************** MODIFICATION HISTORY *******************************

        DATE    WHO     DESCRIPTION
        ----    ---     ------------------------------------------------
        4/91    JBF     Initial Coding

********************************* CONSTANTS ***********************************/

#define FORM_MAX_STRING     256     /* maximum string handled by form */

/* error returns for all form functions */
#define FORM_ESIZE          (-1)    /* output string too large */
#define FORM_EFWRITE        (-2)    /* error in fwrite (fform() and vfform() only */

/********************************** MACROS ************************************/

/************************* STRUCTURE DECLARATIONS *****************************/

/********************** GLOBAL VARIABLE DECLARATIONS **************************/

/********************** GLOBAL FUNCTION DECLARATIONS **************************/

extern char *           form();     /* format, ... */
extern char *           vform();    /* format, ap */
extern int              sform();    /* out, len, format, ... */
extern int              vsform();   /* out, len, format, ap */
extern int              fform();    /* fp, format, ap */
