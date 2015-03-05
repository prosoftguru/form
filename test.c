#include "machdep.h"
#include "form.h"

int main() { 
    fform(stdout, "'this is a test'\n");
    fform(stdout, "'()'\n", 100);
    fform(stdout, "'()'\n",-100);
    fform(stdout, "'(ub16)'\n",-1);
    fform(stdout, "'()(p40)()()'\n",1,2,3,4);
    fform(stdout, "'()(p40)()(p30)'\n",1,2,3,4);
    fform(stdout, "'(r10)'\n", 100);
    fform(stdout, "'(z10)'\n", 100);
    fform(stdout, "'(l10)'\n", 100);
    fform(stdout, "'(b16)'\n", 100);
    fform(stdout, "'(c10l20)'\n", "This is a test");
    fform(stdout, "'(c10r20)'\n", "This is a test");
    fform(stdout, "'(cr20)'\n", "This is a test");
    fform(stdout, "'(cl20)'\n", "This is a test");
    fform(stdout, "'(Cl20)'\n", "    This is a test");
    fform(stdout, "'(Cr20)'\n", "    This is a test    ");
    fform(stdout, "'(C)'\n", "This is a test");
    fform(stdout, "'(f)'\n", 0.123);
    fform(stdout, "'(f)'\n", 123.456);
    fform(stdout, "'(f)'\n", .00000000001);
    fform(stdout, "'(f)'\n", .0000000001);
    fform(stdout, "'(f)'\n", .000000001);
    fform(stdout, "'(f)'\n", 123.000000001);
    fform(stdout, "'(f)'\n", 1234567890.);
    fform(stdout, "'(f)'\n", 1.);
    fform(stdout, "'(f.2)'\n", 0.123);
    fform(stdout, "'(fz5.2)'\n", 0.123);
    fform(stdout, "'(fr3.2)'\n", 0.123);
    fform(stdout, "'(fr5.2)'\n", 0.123);
    getch();
}