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

************************* INCLUDE FILE DESCRIPTION *****************************
    
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
