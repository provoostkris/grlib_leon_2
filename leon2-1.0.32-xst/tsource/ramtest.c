#include "leon.h"
#include "test.h" 

#define CCTRL_IFP (1<<15)
#define CCTRL_DFP (1<<14)

ramfill()
{
	int ilinesz, dlinesz, dbytes, ibytes, itmask, dtmask, isets, dsets; 
	int cachectrl, leonconf;
	leonconf = lr->leonconf;
	isets = ((lr->cachectrl >> 26) & 3) + 1;
	dsets = ((lr->cachectrl >> 24) & 3) + 1;
	ilinesz = 1 << (((leonconf >> 15) & 3) + 2);
	ibytes = (1 << (((leonconf >> 17) & 7) + 10)) * isets;
	dlinesz = 1 << (((leonconf >> 10) & 3) + 2);
	dbytes = (1 << (((leonconf >> 12) & 7) + 10)) * dsets;
	lr->cachectrl = 0x10000;        /* disable icache & dcache */
	ifill(ibytes);
	dfill(dbytes);
	flush();
	lr->cachectrl = 0x1000f;        /* enable icache & dcache */
}

ifill(int bytes)
{
	asm (

"	subcc	%o0, 4, %o0\n"
"1:\n"
"\n"
"	sta	%g0, [%o0] 0xd\n"
"	subcc	%o0, 4, %o0\n"
"	sta	%g0, [%o0] 0xd\n"
"	subcc	%o0, 4, %o0\n"
"	sta	%g0, [%o0] 0xd\n"
"	subcc	%o0, 4, %o0\n"
"	sta	%g0, [%o0] 0xd\n"
"	sta	%g0, [%o0] 0xc\n"
"	subcc	%o0, 4, %o0\n"
"	bg	1b\n"
"	nop\n"
	);
}

dfill(int bytes, int dlinesz)
{
	asm (
"	subcc	%o0, 4, %o0\n"
"1:\n"
"	sta	%g0, [%o0] 0xf\n"
"	subcc	%o0, 4, %o0\n"
"	sta	%g0, [%o0] 0xf\n"
"	subcc	%o0, 4, %o0\n"
"	sta	%g0, [%o0] 0xf\n"
"	subcc	%o0, 4, %o0\n"
"	sta	%g0, [%o0] 0xf\n"
"	sta	%g0, [%o0] 0xe\n"
"	subcc	%o0, 4, %o0\n"
"	bg	1b\n"
"	nop\n"

	);
}

ramtest()
{
	volatile double mrl[1024 + 8];
	int i; 
	int ilinesz, dlinesz, dbytes, ibytes, itmask, dtmask, isets, dsets; 
	int leonconf;

	flush();
	lr->cachectrl |= 0x0f;        /* enable icache & dcache */

	report(DPRAM_TEST);
	if (dpramtest() != 1) fail(13);

	report(CRAM_TEST);
	leonconf = lr->leonconf;

	isets = ((lr->cachectrl >> 26) & 3) + 1;
	dsets = ((lr->cachectrl >> 24) & 3) + 1;
	ilinesz = 1 << (((leonconf >> 15) & 3) + 2);
	ibytes = (1 << (((leonconf >> 17) & 7) + 10)) * isets;
	itmask = (ilinesz - 1) | (0x80000000 - ibytes);
	dlinesz = 1 << (((leonconf >> 10) & 3) + 2);
	dbytes = (1 << (((leonconf >> 12) & 7) + 10)) * dsets;
	dtmask = (dlinesz - 1) | (0x80000000 - dbytes);

	while(lr->cachectrl & (CCTRL_IFP|CCTRL_DFP) ) {} /* wait for flush to complete */

	/* dcache data ram */

	if (ddramtest1(dbytes, mrl,0x55555555)) fail(1);
	if (ddramtest2(dbytes, mrl,0xaaaaaaaa)) fail(2);

	lr->cachectrl = 0x10000;        /* disable icache & dcache */

	/* dcache tag ram */

	if (dtramtest(dbytes, (0xaaaaaa00 & dtmask), dtmask, dlinesz,
	    0xaaaaaaaa)) fail(3);
	if (dtramtest(dbytes, (0x55555500 & dtmask), dtmask, dlinesz,
	    0x55555555)) fail(4);

	/* icache data ram */

	if (idramtest(ibytes, 0x55555555)) fail(5);
	if (idramtest(ibytes, 0xaaaaaaaa)) fail(6);

	/* icache tag ram */

	if (itramtest(ibytes, itmask, ilinesz, 0xaaaaaaaa)) fail(7);
	if (itramtest(ibytes, itmask, ilinesz, 0x55555555)) fail(8);
	flush();
	lr->cachectrl = 0x1000f;        /* enable icache & dcache */

}



/* test dcache data ram */
ddramtest1(int bytes, int *buf, int init)
{
	asm (

"	mov	%o0, %o4\n"
"	mov	%o2, %o3\n"
"1:\n"
"	subcc	%o4, 8, %o4\n"
"	bg 1b\n"
"	std	%o2, [%o4 + %o1]\n"
"\n"
"	mov	%o0, %o4\n"
"1:\n"
"	subcc	%o4, 8, %o4\n"
"	bg 1b\n"
"	ldd	[%o4 + %o1], %g0\n"
"\n"
"	mov	%o0, %o4\n"
"	subcc	%o4, 4, %o4\n"
"	ld	[%o4 + %o1], %o3\n"
"2:\n"
"	subcc	%o3, %o2, %g0\n"
"	bne 	6f\n"
"	subcc	%o4, 4, %o4\n"
"	bg,a 2b\n"
"	ld	[%o4 + %o1], %o3\n"

"	retl\n"
"	mov	0, %o0\n"
"\n"
"6: 	mov	1, %o0\n"

	);
}

/* test dcache data ram */
ddramtest2(int bytes, int *buf, int init)
{
	asm (

"	mov	%o0, %o4\n"
"	mov	%o2, %o3\n"
"1:\n"
"	subcc	%o4, 8, %o4\n"
"	bg 1b\n"
"	std	%o2, [%o4 + %o1]\n"
"\n"
"	mov	%o0, %o4\n"
"	subcc	%o4, 4, %o4\n"
"	ld	[%o4 + %o1], %o3\n"
"2:\n"
"	subcc	%o3, %o2, %g0\n"
"	bne 	6f\n"
"	subcc	%o4, 4, %o4\n"
"	bg,a 2b\n"
"	ld	[%o4 + %o1], %o3\n"
"\n"
"\n"
"	retl\n"
"	mov	0, %o0\n"
"\n"
"6: 	mov	1, %o0\n"

	);

}

/* test icache data ram */
idramtest(int bytes, int init)
{
	asm (

"	mov	%o0, %o4	! init data ram\n"
"	subcc	%o4, 4, %o4\n"
"	sta	%o1, [%o4] 0xd\n"
"1:\n"
"	lda	[%o4] 0xd, %o5\n"
"	subcc	%o5, %o1, %g0\n"
"	bne 	6f\n"
"	subcc	%o4, 4, %o4\n"
"	bg 1b\n"
"	sta	%o1, [%o4] 0xd\n"
"\n"
"	retl\n"
"	mov	0, %o0\n"
"\n"
"6: 	mov	1, %o0\n"
	);

}


itramtest(int bytes, int mask, int linesz, int init)
{
	asm (

"	mov	%o0, %o5	! init data ram\n"
"	subcc	%o5, %o2, %o5\n"
"	sta	%o3, [%o5] 0xc\n"
"1:\n"
"	lda	[%o5] 0xc, %o4\n"
"	xor  	%o4, %o3, %o4\n"
"	andcc	%o4, %o1, %o4\n"
"	bne 	6f\n"
"	subcc	%o5, %o2, %o5\n"
"	bg 1b\n"
"	sta	%o3, [%o5] 0xc\n"
"\n"
"	retl\n"
"	mov	0, %o0\n"
"\n"
"6: 	mov	1, %o0\n"
	);
}

dtramtest(int bytes, int addr, int mask, int linesz, int init)
{
	asm (

"	mov	%o0, %o5	! init data ram\n"
"	subcc	%o5, %o3, %o5\n"
"	sta	%o4, [%o1 + %o5] 0xe\n"
"\n"
"1:\n"
"	lda	[%o1 + %o5] 0xe, %g2\n"
"	xor  	%g2, %o4, %g2\n"
"	andcc	%g2, %o2, %g2\n"
"	bne 	6f\n"
"	subcc	%o5, %o3, %o5\n"
"	bg 1b\n"
"	sta	%o4, [%o1 + %o5] 0xe\n"
"\n"
"	retl\n"
"	mov	0, %o0\n"
"\n"
"6: 	mov	1, %o0\n"
	);
}



asm (

"	.text\n"
"	.align 4\n"
"	.global _dpramtest, dpramtest, regbuf\n"
"	.proc	04\n"
"\n"
"_dpramtest:\n"
"dpramtest:\n"
"\n"
"\n"
"	save %sp, -96, %sp\n"
"\n"
"	set 0x80000000, %l0\n"
"	ld [%l0 + 0x08], %g2	! get configuration status\n"
"	srl %g2, 30, %g3\n"
"	andcc %g3, 3, %g3\n"
"	be 1f\n"
"	nop\n"
"	mov	%asr16, %g3	! skip tests if err-injection is on\n"
"	andcc %g4, 0xe00, %g3\n"
"	bne testok\n"
"	nop\n"
"	mov 	%g0, %asr16	! clear error counters\n"
"\n"
"	! flush all register windows (maximum 32)\n"
"1:\n"
"	set	0x80000024, %l2\n"
"	ld	[%l2], %l2\n"
"	srl	%l2, 20, %g5\n"
"	and	%g5, 0x1f, %g5\n"
"	mov	%g5, %g6\n"
"2:\n"
"	save %sp, -96, %sp\n"
"	subcc	%g5, 1, %g5\n"
"	bge	2b\n"
"	nop\n"
"3:\n"
"	restore\n"
"	subcc	%g6, 1, %g6\n"
"	bge	3b\n"
"	nop\n"
"\n"
"	! save global and input registers\n"
"\n"
"	nop; nop\n"
"	set	regbuf, %l0\n"
"	mov	%psr, %l1\n"
"	st	%l1, [%l0]\n"
"	st	%g1, [%l0+4]\n"
"	std	%g2, [%l0+8]\n"
"	std	%g4, [%l0+16]\n"
"	std	%g6, [%l0+24]\n"
"	std	%i0, [%l0+32]\n"
"	std	%i2, [%l0+40]\n"
"	std	%i4, [%l0+48]\n"
"	std	%i6, [%l0+56]\n"
"	mov	%wim, %l2\n"
"	st	%l2, [%l0+64]\n"
"\n"
"	mov	%g0, %wim\n"
"	andn	%l1, 0x1f, %l2\n"
"	mov	%l2, %psr\n"
"	nop; nop; nop\n"
"	\n"
"	! test gloabal registers\n"
"	! note that test have to be done twice, once for each dpram-half\n"
"\n"
"	set	0x55555555, %g1	! test 0x55555555 in global regs, dpram1\n"
"7:\n"
"	mov	%g1, %g2\n"
"	mov	%g1, %g3\n"
"	mov	%g1, %g4\n"
"	mov	%g1, %g5\n"
"	mov	%g1, %g6\n"
"	mov	%g1, %g7\n"
"\n"
"	subcc	%g1, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%g1, %g3, %g0\n"
"	bne	fail\n"
"	subcc	%g1, %g4, %g0\n"
"	bne	fail\n"
"	subcc	%g1, %g5, %g0\n"
"	bne	fail\n"
"	subcc	%g1, %g6, %g0\n"
"	bne	fail\n"
"	subcc	%g1, %g7, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %g1, %g0\n"
"	bne	fail\n"
"	subcc	%g3, %g1, %g0\n"
"	bne	fail\n"
"	subcc	%g4, %g1, %g0\n"
"	bne	fail\n"
"	subcc	%g5, %g1, %g0\n"
"	bne	fail\n"
"	subcc	%g6, %g1, %g0\n"
"	bne	fail\n"
"	subcc	%g7, %g1, %g0\n"
"	bne	fail\n"
"\n"
"	andcc	%g1, 0x0ff, %g7\n"
"	xorcc	%g7, 0x55, %g0\n"
"	be	7b\n"
"	xor	%g1, -1, %g1\n"
"\n"
"	! fill all windowed registers with 0x55555555/0xaaaaaaaa\n"
"	set	0x55555555, %g2\n"
"	mov	%g0, %g3\n"
"	set	0x80000024, %g3\n"
"	ld	[%g3], %g3\n"
"	srl	%g3, 20, %g4\n"
"	and	%g4, 0x1f, %g4\n"
"	mov	%g4, %g3\n"
"4:\n"
"	mov	%g2, %l0\n"
"	mov	%g2, %l1\n"
"	mov	%g2, %l2\n"
"	mov	%g2, %l3\n"
"	mov	%g2, %l4\n"
"	mov	%g2, %l5\n"
"	mov	%g2, %l6\n"
"	mov	%g2, %l7\n"
"	mov	%g2, %o0\n"
"	mov	%g2, %o1\n"
"	mov	%g2, %o2\n"
"	mov	%g2, %o3\n"
"	mov	%g2, %o4\n"
"	mov	%g2, %o5\n"
"	mov	%g2, %o6\n"
"	mov	%g2, %o7\n"
"	save\n"
"	subcc	%g3, 1, %g3\n"
"	bge	4b\n"
"	nop\n"
"	\n"
"	! check values\n"
"\n"
"	mov	%g4, %g3\n"
"	mov	%g0, %g1\n"
"5:\n"
"	subcc	%l0, %g2, %g0	! dpram1\n"
"	bne	fail\n"
"	subcc	%l1, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%l2, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%l3, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%l4, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%l5, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%l6, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%l7, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%o0, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%o1, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%o2, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%o3, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%o4, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%o5, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%o6, %g2, %g0\n"
"	bne	fail\n"
"	subcc	%o7, %g2, %g0\n"
"	bne	fail\n"
"	nop\n"
"	\n"
"	subcc	%g2, %l0, %g0	! dpram2\n"
"	bne	fail\n"
"	subcc	%g2, %l1, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %l2, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %l3, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %l4, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %l5, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %l6, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %l7, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %o0, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %o1, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %o2, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %o3, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %o4, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %o5, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %o6, %g0\n"
"	bne	fail\n"
"	subcc	%g2, %o7, %g0\n"
"	bne	fail\n"
"\n"
"	save\n"
"	subcc	%g3, 1, %g3\n"
"	bge	5b\n"
"	nop\n"
"	\n"
"	and	%g2, 0x0ff, %g6	! repeat test with 0xaaaaaaaa\n"
"	subcc	%g6, 0xaa, %g0\n"
"	be	cbtest\n"
"	add	%g2, %g2, %g2\n"
"	ba	4b\n"
"	mov	%g4, %g3\n"
"\n"
"! the above test all set the edac checkbits to 0x0c, redo the\n"
"! test with cb = 0x73 to check for both stuck-1 and stuck-0 errors\n"
"\n"
"cbtest:\n"
"	set 0x80000000, %l0\n"
"	ld [%l0 + 0x08], %g2	! get configuration status\n"
"	srl %g2, 30, %g3\n"
"	andcc %g3, 3, %g3\n"
"	be testok\n"
"	nop\n"
"	mov	%asr16, %g3	! skip cb tests if errors have been seen\n"
"	andcc %g4, 0xe00, %g3\n"
"	bne fail\n"
"\n"
"	! fill all windowed registers with 0x80000090 (cb = 0x73)\n"
"	mov %g0, %asr16		! clear error counters\n"
"	set	0x80000090, %g2\n"
"	mov	%g4, %g3\n"
"4:\n"
"	mov	%g2, %l0\n"
"	mov	%g2, %l1\n"
"	mov	%g2, %l2\n"
"	mov	%g2, %l3\n"
"	mov	%g2, %l4\n"
"	mov	%g2, %l5\n"
"	mov	%g2, %l6\n"
"	mov	%g2, %l7\n"
"	mov	%g2, %o0\n"
"	mov	%g2, %o1\n"
"	mov	%g2, %o2\n"
"	mov	%g2, %o3\n"
"	mov	%g2, %o4\n"
"	mov	%g2, %o5\n"
"	mov	%g2, %o6\n"
"	mov	%g2, %o7\n"
"	save\n"
"	subcc	%g3, 1, %g3\n"
"	bge	4b\n"
"	nop\n"
"	\n"
"	! check values\n"
"\n"
"	mov	%g4, %g3\n"
"5:\n"
"	addcc	%l0, %l0, %g1	! dpram1 & dpram2\n"
"	addxcc	%l1, %l1, %g1\n"
"	addxcc	%l2, %l2, %g1\n"
"	addxcc	%l3, %l3, %g1\n"
"	addxcc	%l4, %l4, %g1\n"
"	addxcc	%l5, %l5, %g1\n"
"	addxcc	%l6, %l6, %g1\n"
"	addxcc	%l7, %l7, %g1\n"
"	addxcc	%o0, %o0, %g1\n"
"	addxcc	%o1, %o1, %g1\n"
"	addxcc	%o2, %o2, %g1\n"
"	addxcc	%o3, %o3, %g1\n"
"	addxcc	%o4, %o4, %g1\n"
"	addxcc	%o5, %o5, %g1\n"
"	addxcc	%o6, %o6, %g1\n"
"	addxcc	%o7, %o7, %g1\n"
"	save\n"
"	subcc	%g3, 1, %g3\n"
"	bge	5b\n"
"	nop\n"
"\n"
"	mov %asr16, %l2\n"
"	andcc %l2, 0xe00, %l2\n"
"	bne	fail		! fail if correction was found\n"
"\n"
"testok:\n"
"	set	regbuf, %l0\n"
"	mov	1, %o0\n"
"	ba	exit\n"
"	st	%o0, [%l0+32]\n"
"fail:\n"
"	set	regbuf, %l0\n"
"	ba	exit\n"
"	st	%g0, [%l0+32]\n"
"\n"
"exit:\n"
"\n"
"	! restore state\n"
"\n"
"	set	regbuf, %g1\n"
"	ld	[%g1], %g2\n"
"	mov	%g2, %psr\n"
"	nop; nop; nop\n"
"	ldd	[%g1+8], %g2\n"
"	ldd	[%g1+16], %g4\n"
"	ldd	[%g1+24], %g6\n"
"	ldd	[%g1+32], %i0\n"
"	ldd	[%g1+40], %i2\n"
"	ldd	[%g1+48], %i4\n"
"	ldd	[%g1+56], %i6\n"
"	ld	[%g1+64], %l2\n"
"	ld	[%g1+4], %g1\n"
"	mov	%l2, %wim\n"
"	nop; nop; nop\n"
"\n"
"last:\n"
"\n"
"	ret\n"
"	restore\n"
);
