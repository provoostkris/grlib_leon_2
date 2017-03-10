#include "leon.h"
#include "test.h" 
#include <math.h> 

int __errno;
fpu_init()
{
	int tmp;

	tmp = getpsr();
	setpsr(tmp | (1 << 12));
	set_fsr(0);
}

fpu_test()
{
	if (!((lr->leonconf>>FPU_CONF_BIT)&FPU_CONF_MASK)) return(0);
	report(FPU_TEST);

	fpu_init();
	fpu_main();
}

asm(

"	.global a1, a2\n"
"	.align 8\n"
"a1:	.word 0x48000001\n"
"	.word 0\n"
"b1:	.word 0x48000000\n"
"	.word 0\n"
"c1:	.word 0x46c00000\n"
"	.word 0\n"
	);

fpu_main()
{
	volatile double a,c;
	extern double a1,b1,c1;
	float b;
	int tmp;

	a = *(double *)&a1 - *(double *)&b1;
	if (a != c1) fail(1);
	a = sqrt(3.0);
	if (fabs((a * a) - 3.0) > 1E-15) fail(2);
	b = sqrt(3.0);
	if (fabs((b * b) - 3.0) > 1E-7) fail(3);
	tmp = fpu_pipe();
	if (tmp) fail(tmp);
}

//double sqrt(){};
double f1x = -1.0;
double fq1[2];
int fsr1 = 0x80000000;
int dummy2 = 2;
int dummy3 = 2;

fpu_pipe()
{
	asm(

"	set	fsr1, %o0	! check ldfsr/stfsr interlock\n"
"	ld	[%o0], %fsr\n"
"	st	%g0, [%o0]\n"
"	ld	[%o0], %fsr\n"
"	st	%fsr, [%o0]\n"
"	ld	[%o0], %o2\n"
"	subcc	%g0, %o2, %g0\n"
"	bne,a	fail\n"
"	mov	3, %o0\n"
"\n"
"	set 0x0f800000, %o1	! check ldfsr/fpop interlock\n"
"	st	%o1, [%o0]\n"
"	ld	[%o0], %fsr\n"
"	st	%g0, [%o0]\n"
"	set	f1x, %o2\n"
"	ld	[%o2], %f0\n"
"	nop; nop\n"
"	ld	[%o0], %fsr\n"
"	fsqrts	%f0, %f1\n"
"	mov	%tbr, %o3\n"
"	and	%o3, 0x0ff0, %o3\n"
"	subcc	%o3, 0x070, %g0\n"
"	be,a	fail\n"
"	mov	4, %o0\n"
"\n"
"\n"
"	mov	0, %o0\n"
"\n"
"fail:\n"
"	nop\n"
	);
}

