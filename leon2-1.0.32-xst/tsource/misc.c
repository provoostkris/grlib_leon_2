#include "leon.h"
struct lregs *lr = (struct lregs *) PREGS;
unsigned char *msg = (unsigned char *) IOAREA; 
//unsigned char *msg = (unsigned char *) CRAM; 
unsigned short *msgh = (unsigned short *) IOAREA; 
unsigned int *msgw = (unsigned int *) IOAREA; 
unsigned long long *msgd = (unsigned long long *) IOAREA; 
int test ;
int dummy[4] = {0,0,0,0};

fail(err) int err; { msg[test] = err; }
report(test_case) int test_case; { test = test_case; msg[test] = 0; }

int getpsr() { asm(" mov %psr, %o0 "); }

setpsr(psr) int psr; { asm(" mov %o0, %psr;nop;nop;nop "); }

unsigned char inb(a) int a; { return(msg[a]); }
outb(a,d) int a; char d; { msg[a] = d; }
unsigned short inh(a) int a; { return(msgh[a]); }
outh(a,d) int a; short d; { msgh[a] = d; }
unsigned int inw(a) int a; { return(msgw[a]); }
outw(a,d) int a; short d; { msgw[a] = d; }
unsigned long long ind(a) int a; { return(msgd[a]); }
outd(a,d) int a; short d; { msgd[a] = d; }

asm("	.global _get_fsr, _set_fsr\n"
"	.global get_fsr, set_fsr\n"
"	.data\n"
"fsrtmp:	.word 0\n"
"	.text\n"
"_get_fsr:\n"
"get_fsr:\n"
"	set	fsrtmp, %o0\n"
"	st	%fsr, [%o0]\n"
"	retl\n"
"	ld	[%o0], %o0\n"
"_set_fsr:\n"
"set_fsr:\n"
"	set	fsrtmp, %o1\n"
"	st	%o0, [%o1]\n"
"	retl\n"
"	ld	[%o1], %fsr\n"
"	.global get_asr24\n"
"get_asr24:\n"
"	retl; mov	%asr24, %o0;\n"
"	.global get_asr25\n"
"get_asr25:\n"
"	retl; mov	%asr25, %o0;\n"
"	.global get_asr26\n"
"get_asr26:\n"
"	retl; mov	%asr26, %o0;\n"
"	.global get_asr27\n"
"get_asr27:\n"
"	retl; mov	%asr27, %o0;\n"
"	.global get_asr28\n"
"get_asr28:\n"
"	retl; mov	%asr28, %o0;\n"
"	.global get_asr29\n"
"get_asr29:\n"
"	retl; mov	%asr29, %o0;\n"
"	.global get_asr30\n"
"get_asr30:\n"
"	retl; mov	%asr30, %o0;\n"
"	.global get_asr31\n"
"get_asr31:\n"
"	retl; mov	%asr31, %o0;\n"
"	.global set_asr24\n"
"set_asr24:\n"
"	retl; mov	%o0, %asr24;\n"
"	.global set_asr25\n"
"set_asr25:\n"
"	retl; mov	%o0, %asr25;\n"
"	.global set_asr26\n"
"set_asr26:\n"
"	retl; mov	%o0, %asr26;\n"
"	.global set_asr27\n"
"set_asr27:\n"
"	retl; mov	%o0, %asr27;\n"
"	.global set_asr28\n"
"set_asr28:\n"
"	retl; mov	%o0, %asr28;\n"
"	.global set_asr29\n"
"set_asr29:\n"
"	retl; mov	%o0, %asr29;\n"
"	.global set_asr30\n"
"set_asr30:\n"
"	retl; mov	%o0, %asr30;\n"
"	.global set_asr31\n"
"set_asr31:\n"
"	retl; mov	%o0, %asr31;\n"
);
