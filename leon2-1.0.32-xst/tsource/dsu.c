#include "leon.h"
#include "test.h" 

struct dsuregs {
	volatile unsigned int dsuctrl;
	volatile unsigned int tracectrl;
	volatile unsigned int timetag;
	volatile unsigned int ahbbreak1;
	volatile unsigned int ahbmask1;
	volatile unsigned int ahbbreak2;
	volatile unsigned int ahbmask2;
};

struct tracebuf {
	volatile unsigned int buf[8192];
};

dsu_ram_test()
{
	unsigned int ntrace, dsuen;
	int i;

	struct dsuregs *dsu = (struct dsuregs *) 0x90000000;
	struct tracebuf *tbuf = (struct tracebuf *) 0x90010000;

	if (!(lr->leonconf & 0x40000000)) return;
	dsu->dsuctrl = 0xfff00008;
	dsuen = dsu->dsuctrl & 8;
	ntrace = (dsu->dsuctrl >> 20 ) + 1;
	if (!dsuen) return;


	if (!ntrace) return;

	report(DSU_TEST);

	/* test trace buffer memory*/


	buframfill(ntrace<<2, 0x90010000, 0x55555555, 0xaaaaaaaa);
	if (buframtest(ntrace<<2, 0x90010000, 0x55555555, 0xaaaaaaaa))
		fail(1);
	buframfill(ntrace<<2, 0x90010000, 0xaaaaaaaa, 0x55555555);
	if (buframtest(ntrace<<2, 0x90010000, 0xaaaaaaaa, 0x55555555))
		fail(2);

}

buframtest(int len, int buf, int patt1, int patt2)
{
asm (
"	mov	%g0, %o5\n"
"1:\n"
"	sub	%o0, 4, %o0\n"
"	ld	[%o1+%o0], %o4\n"
"	subcc	%o0, 4, %o0\n"
"	xor	%o4, %o2, %o4\n"
"	or	%o4, %o5, %o5\n"
"	ld	[%o1+%o0], %o4\n"
"	xor	%o4, %o3, %o4\n"
"	bg	1b\n"
"	or	%o4, %o5, %o5\n"
"	mov	%o5, %o0\n"
	);
}
	
buframfill(int len, int buf, int patt1, int patt2)
{
asm (
"1:\n"
"	sub	%o0, 4, %o0\n"
"	st	%o2, [%o1+%o0]\n"
"	subcc	%o0, 4, %o0\n"
"	bg	1b\n"
"	st	%o3, [%o1+%o0]\n"
	);
	
}
	
