#include "test.h"
#include "leon.h"

struct mulcase {
	int	fac1;
	int	fac2;
	int	res;
};

volatile struct mulcase mula[] = { { 2, 3, 6}, { 2, -3, -6}, { 0,  1, 0},
	{ 0, -1, 0}, {  1, -1, -1}, { -1,  1, -1}, { -2,  3, -6},
	{ -2, -3, 6}, {  0,  0, 9}};

multest()
{
	int i = 0;

	report(MUL_TEST);
	if (mulscc_test() != 0x123) fail(1);

	/* skip test if divider disabled */
	if (!((lr->leonconf >> MUL_CONF_BIT) & 1)) return(0);	
	
	while (mula[i].res != 9) {
	    if ((mula[i].fac1 * mula[i].fac2) - mula[i].res) fail(1);
	    i++;
	}
	if (!mulpipe()) fail(2);
	if (!((lr->leonconf >> MAC_CONF_BIT) & 1)) return(0);	
	if (!macpipe()) fail(3);
}

int mulscctmp = 0xfffff000;

mulscc_test () {
asm (
"	.text\n"
"	.global mulscctmp\n"
"	addcc %g0, %g0, %g0\n"
"	set   mulscctmp, %o2\n"
"	mov   0, %o0\n"			
"	mov   0x246, %o1\n"	
"	mov	%g0, %y\n"
"	nop; nop; nop\n"
"	ld    [%o2], %o0\n"
"	mulscc %o1, %o0, %o0\n"
"	retl\n"
"	nop\n"
	);
}

mulpipe () {
asm (
"	.text\n"
"\n"
"	mov	%g0, %y\n"
"	set	1, %o0\n"
"	orcc	%g0, 1, %g0	! clear icc\n"
"	umulcc	%g0, 1, %g0	! set Z\n"
"	bne	1f\n"
"	nop\n"
"	umulcc	%o0, -1, %g0	! set N\n"
"	bge	1f\n"
"	nop\n"
"	smulcc	%o0, -1, %g0	! set N and Y\n"
"	mov	%y, %o1\n"
"	subcc	%o1, -1, %g0\n"
"	bne	1f\n"
"	nop\n"
"	umulcc	%o0, -1, %g0	! set N\n"
"	mov	%psr, %o1\n"
"	srl	%o1, 20, %o1\n"
"	and	%o1, 0x0f, %o1\n"
"	subcc	%o1, 8, %g0\n"
"	bne	1f\n"
"	nop\n"
"\n"
"	retl\n"
"	or	%g0, 1, %o0\n"
"\n"
"1:\n"
"	retl\n"
"	mov	%g0, %o0\n"
	);
}

macpipe () {
asm (
"	.text\n"
"	set 	0x55555555, %o1		! test %asr18\n"
"	mov	%o1, %asr18\n"
"	nop; nop; nop\n"
"	mov	%asr18, %o2\n"
"	subcc	%o1, %o2, %g0\n"
"	bne	1f\n"
"	nop\n"
"	not	%o1\n"
"	mov	%o1, %asr18\n"
"	nop; nop; nop\n"
"	mov	%asr18, %o2\n"
"	subcc	%o1, %o2, %g0\n"
"	bne	1f\n"
"	nop\n"
"\n"
"	set	0xffff, %o0\n"
"	mov	%g0, %asr18\n"
"	mov	%g0, %y\n"
"	umac	%o0, -1, %o1\n"
"	umac	%o0, -1, %o1\n"
"	umac	%o0, -1, %o1\n"
"	umac	%o0, -1, %o1\n"
"	umac	%o0, -1, %o1\n"
"	umac	%o0, -1, %o1\n"
"	umac	%o0, -1, %o1\n"
"	umac	%o0, -1, %o1\n"
"	mov	%y, %o4\n"
"	mov	%asr18, %o3\n"
"	set	0xfff00008, %o2\n"
"	subcc	%o1, %o2, %g0\n"
"	bne	1f\n"
"	subcc	%o1, %o3, %g0\n"
"	bne	1f\n"
"	subcc	%o4, 0x07, %g0\n"
"	bne	1f\n"
"\n"
"	set	0xffff, %o0\n"
"	mov	%g0, %asr18\n"
"	mov	%g0, %y\n"
"	smac	%o0, -1, %o1\n"
"	smac	%o0, -1, %o1\n"
"	smac	%o0, -1, %o1\n"
"	smac	%o0, -1, %o1\n"
"	smac	%o0, -1, %o1\n"
"	smac	%o0, -1, %o1\n"
"	smac	%o0, -1, %o1\n"
"	smac	%o0, -1, %o1\n"
"	mov	%y, %o4\n"
"	mov	%asr18, %o3\n"
"	subcc	%o1, 8, %g0\n"
"	bne	1f\n"
"	subcc	%o1, 8, %g0\n"
"	bne	1f\n"
"	subcc	%o4, 0, %g0\n"
"	bne	1f\n"
"	\n"
"	retl\n"
"	or	%g0, 1, %o0\n"
"\n"
"1:\n"
"	retl\n"
"	mov	%g0, %o0\n"
	);
}

int ddd[8] = {0,0,0,0,0,0,0,0};
