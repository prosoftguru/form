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


NAME
----

form - General purpose, easy to use and extremely flexible text formatting library.

SYNOPSYS
--------

char * **form**(char * *format*, ...);

DESCRIPTION
-----------

The **form** library routines perform general purpose text formatting. This library is copyright 1991 by Jeff Fisher @ ProSoft.Guru. All rights are reserve. See source code for more license information.

There are six variations of the form routine. The variations differ in what they do with, and where they create, the output string.


**Form** and **vform** create the output string in a static data area and return a pointer to the beginning of the output string. The static data area is eventually reused by subsequent calls to **form** and **vform** so don't expect these strings to last forever.

**Sform** and **vsform** create the output string in the area specified by the *buf* argument. The output string is not allowed to be longer than *size* bytes. (Including any null termination.) The length of the output string (including any null termination) is returned.

**Fform** and **vfform** fwrite the output string to the specified *fp*. The length of the output string (not including any null termination) is returned.

The routines beginning with 'v' take a variable argument list pointer **ap**. (Refer to varargs.h or stdarg.h.) The other routines accept a variable number of arguments (...), similar to **printf(3)**.

