FORM
====

PURPOSE
-------
Ready for something far better than **printf(3)**? You've found it.

I wrote the form library a long time ago when I was doing a lot of consulting work. I was writing a ton of C code on various platforms. After years of using **printf(3)** for all my string formatting I was getting pretty sick of it. The syntax seemed to me to be difficult and arcane to use. It's variable arguments coupled with machine dependent argument specifications in the format string were a constant headache when porting software. And printf simply has limitations that slowed me down.

So I wrote the form library. I coupled several ideas that I had used, in full or in part, on several previous projects with a new format string.

1. With printf, it takes several statements to create a formatted string in a buffer. You have to specify the buffer, sprintf into it (watching for overflow), use the buffer (maybe just strcpy to another location), then free the buffer. I had been using a small wrapper for sprintf that placed strings sequentially in a static buffer that was eventually reused. Very handy, and efficient.

2. At one point I was working on no less than six different platforms at the same time. No two machines handled char, int, short, long, float, and double the same! Plus some of the code was to run on more than one machine, so the differences in architechture were driving me crazy. The solution was simple, and a technique I use to this day. I created an include file, **machdep.h**, that makes the new types that are a letter and the number of bits long.

   > u = Unsigned  
   > s = Signed  
   > f = Floating point  
   >  
   > 8, 16, 32, 64 The number of bits long.

   Therefore, I could use, for instance, u32 (which is an unsigned long on many machines) and always be sure I was getting the type and size variable I expected. Simple. I just had different machdep.h files for the different machines.

   I used this convention in the format string in form. It really cut down on argument allignment problems.

3. Lastly, I added some features that I felt were sorely lacking from printf, such as zero fill, absolute positioning, base anything printing, left and right justification of numbers within fields, and supression of the terminating null character.

Form is tiny, simple, fast, and portable. It doesn't use any external string functions and only uses floor() and fmod() to do its own floating point printing. I've used it in tiny embedded system projects and in mainframe code. Once you use form on a project, you'll wonder how you ever got along without it!

<jeff@prosoft.guru>

NAME
----

form - General purpose, easy to use and extremely flexible text formatting library.

SYNOPSYS
--------

char * **form**(char * *format*, ...);

DESCRIPTION
-----------

The **form** library routines perform general purpose text formatting.

There are six variations of the form routine. The variations differ in what they do with, and where they create, the output string.

**Form** and **vform** create the output string in a static data area and return a pointer to the beginning of the output string. The static data area is eventually reused by subsequent calls to **form** and **vform** so don't expect these strings to last forever.

**Sform** and **vsform** create the output string in the area specified by the *buf* argument. The output string is not allowed to be longer than *size* bytes. (Including any null termination.) The length of the output string (including any null termination) is returned.

**Fform** and **vfform** fwrite the output string to the specified *fp*. The length of the output string (not including any null termination) is returned.

The routines beginning with 'v' take a variable argument list pointer **ap**. (Refer to varargs.h or stdarg.h.) The other routines accept a variable number of arguments (...), similar to **printf(3)**.


COPYRIGHT
---------

Copyright 1991-2015 Jeff Fisher @ <http://prosoft.guru>

This file is part of FORM by prosoft.guru.

FORM is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License, version 3, as published by the
Free Software Foundation.

FORM is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.

You should have received a copy of the GNU General Public License along
with FORM. If not, see http://www.gnu.org/licenses/.


FORMAT STRING
-------------

The *format* string can contain any characters except the null character. Format definitions within the format string are enclosed in parenthesis. All characters not enclosed within parenthesis are copied directly to the output string. To include a '(' character in the output string, use two '(' characters in a row. There may be zero or more format definitions within a *format* string. There must be one argument in the argument list for for every format definition in the *format* string.

A format definition consists of zero or more keyletters, each keyletter may be followed by a count (decimal number) or an '*'. The asterisk indicated that the count for this keyletter is to be taken from the next argument in the argument list.

There are five classes of keyletters that can be specified within a format definition: The input type, the output base, the output justification, the output precision, and repositioning information.

INPUT TYPES
-----------

The input type keyletter specifies what kind of argument to expect on the argument list. It is vital that this is specified correctly as incorrect information will cause the argument list to get out of sync with the format definitions. The keyletters for specifying the input type are:
> **s#**  =  Signed value with # bits. (Default is sizeof int * 8) Valid values are 8, 16, and 32.  
> **u#**  =  Unsigned value with # bits. (Default is sizeof int * 8) Valid values are 8, 16, and 32.  
> **f#**  =  Floating point value with # bits. (Default 32) Valid values are 32 and 64.  
> **c#**  =  Character string of # characters. (Default 0 = Null terminated.)
> **C#**  =  Same as 'c' except that leading and trailing spaces are removed.

The default input type is 's'. (A regular int.)

OUTPUT BASE
-----------

The output base keyletter specifies the base in which to print numeric data:
> **b#**  =  Print the value in base # using lower case for alpha digits.  
> **B#**  =  Print the value in base # using upper case for alpha digits.  

The default output base is 'b10'. (Base 10.) The maximum output base is 'b37'.

OUTPUT JUSTIFICATION
--------------------

The output justification keyletter specifies the way to justify the output and the size of the output field:
> **l#**  =  Left justify in a # character field. If the size of the data to output is greater than the field size, the *leftmost* characters will be output.  
> **r#**  =  Right justify in a # character field. If the size of the data to output is greater than the field size, the *rightmost* characters will be output.  
> **z#**  =  Zero fill in # character field. If the size of the data to output is greater than the field size, the *rightmost* characters will be output.  
> **v**  =  Variable length field. Use just as many caracters as is necessary.

The default output justification is 'v'.

OUTPUT PRECISION
----------------

The output precision keyletter specifies the number of digits after the decimal point to print for floating point values.
> **i**  =  Print the integer portion of the number only and no decimal point.  
> **.#**  =  Print the value with # digits after the decimal point. (Default is 0, which means as many digits as is necessary.)  

The default output precision is 'i'. Note that the output precision keyletter is only relevant when used with the 'f' (floating point input type) keyletter.

OUTPUT POSITION
---------------

The 'p' keyletter may be used to alter the position within the output string that the field will be printed. When 'p#' is specified within a format definition, that field will begin at position '#' in the output string. If the string is not yet that long, spaces will be added to make it that long. If the string already extends past that point, the new field will overwrite whatever is at position '#'. Note that the length of the output string will not be reduced by backwards positioning. Any further nonformat characters in the *format* string and fields printed without a format position change will be appended to the output string in the usual way.

OUTPUT TERMINATION
------------------

The output string will receive a null termination character *unless* the last character of the *format* string is a '(' format begin character. This feature is only useful when calling **sform** and **vsform**.

EXAMPLES
--------

    fform(stdout, "'this is a test'\n");                    // 'this is a test'
    fform(stdout, "'()'\n", 100);                           // '100'
    fform(stdout, "'()'\n",-100);                           // '-100'
    fform(stdout, "'(ub16)'\n",-1);                         // '-1'
    fform(stdout, "'()(p40)()()'\n",1,2,3,4);               // '1                                      234'
    fform(stdout, "'()(p40)()(p30)'\n",1,2,3,4);            // '1                            4         23'
    fform(stdout, "'(r10)'\n", 100);                        // '       100'
    fform(stdout, "'(z10)'\n", 100);                        // '0000000100'
    fform(stdout, "'(l10)'\n", 100);                        // '100       '
    fform(stdout, "'(b16)'\n", 100);                        // '64'
    fform(stdout, "'(c10l20)'\n", "This is a test");        // 'This is a           '
    fform(stdout, "'(c10r20)'\n", "This is a test");        // '          This is a '
    fform(stdout, "'(cr20)'\n", "This is a test");          // '      This is a test'
    fform(stdout, "'(cl20)'\n", "This is a test");          // 'This is a test      '
    fform(stdout, "'(Cl20)'\n", "    This is a test");      // 'This is a test      '
    fform(stdout, "'(Cr20)'\n", "    This is a test    ");  // '      This is a test'
    fform(stdout, "'(C)'\n", "This is a test");             // 'This is a test'
    fform(stdout, "'(f)'\n", 0.123);                        // '0.123'
    fform(stdout, "'(f)'\n", 123.456);                      // '123.456'
    fform(stdout, "'(f)'\n", .00000000001);                 // '0.0000000001'
    fform(stdout, "'(f)'\n", .0000000001);                  // '0.0000000001'
    fform(stdout, "'(f)'\n", .000000001);                   // '0.000000001'
    fform(stdout, "'(f)'\n", 123.000000001);                // '123.000000001'
    fform(stdout, "'(f)'\n", 1234567890.);                  // '1234567890.0'
    fform(stdout, "'(f)'\n", 1.);                           // '1.0'
    fform(stdout, "'(f.2)'\n", 0.123);                      // '0.12'
    fform(stdout, "'(fz5.2)'\n", 0.123);                    // '00.12'
    fform(stdout, "'(fr3.2)'\n", 0.123);                    // '.12'
    fform(stdout, "'(fr5.2)'\n", 0.123);                    // ' 0.12'


DIAGNOSTICS
-----------

Floating point values > 1e11 or < -1e11 will print as a single '?'.

**Form** and **vform** return NULL if there is not enough room in the static memory area for the output string.

**Sform** and **vsform** return the count of bytes placed in *buf*, including any null termination, or they return -1 if the string would be longer than *size* bytes.

**Fform** and **vform** return the count of bytes written, not including any null termination, or they return -1 if the output string would not fit in the static storage area, or they return -2 if the write to *fp* failed.

SEE ALSO
--------

**printf(3)**
