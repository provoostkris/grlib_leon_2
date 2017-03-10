#include "leon.h"
#include "test.h" 
#include "mmu.h" 

#define NODO_CLEAR
#define TLBNUM 8

typedef unsigned long traptableentry[4] ;
extern traptableentry patch_dtrap, patch_itrap, patch_dtrap_replace, patch_itrap_replace;

extern unsigned long ctx;
extern unsigned long pg0,pm0,pt0,page0,page1,page2,pth_addr,pth_addr1;
extern unsigned long ctx1_pg0,ctx1_pm0,ctx1_pt0,ctx1_page0;
typedef void (*functype)(void);

volatile struct lregs *leonctlr = (struct lregs *) PREGS;

void mmu_func1();
mmu_test_real(void);

mmu_test()
{
  unsigned long dtrap[4];
  unsigned long itrap[4];
  dtrap[0] = patch_dtrap[0];
  dtrap[1] = patch_dtrap[1];
  dtrap[2] = patch_dtrap[2];
  dtrap[3] = patch_dtrap[3];
  itrap[0] = patch_itrap[0];
  itrap[1] = patch_itrap[1];
  itrap[2] = patch_itrap[2];
  itrap[3] = patch_itrap[3];
  patch_dtrap[0] = patch_dtrap_replace[0];
  patch_dtrap[1] = patch_dtrap_replace[1];
  patch_dtrap[2] = patch_dtrap_replace[2];
  patch_dtrap[3] = patch_dtrap_replace[3];
  patch_itrap[0] = patch_itrap_replace[0];
  patch_itrap[1] = patch_itrap_replace[1];
  patch_itrap[2] = patch_itrap_replace[2];
  patch_itrap[3] = patch_itrap_replace[3];
  asm(" sta	%g0, [%g0] 0x5 "); //iflush 
  mmu_test_real();
  patch_dtrap[0] = dtrap[0];
  patch_dtrap[1] = dtrap[1];
  patch_dtrap[2] = dtrap[2];
  patch_dtrap[3] = dtrap[3];
  patch_itrap[0] = itrap[0];
  patch_itrap[1] = itrap[1];
  patch_itrap[2] = itrap[2];
  patch_itrap[3] = itrap[3];
  asm(" sta	%g0, [%g0] 0x5 "); //iflush 
}

mmu_test_real(void)
{
  ctxd_t *c0 = (ctxd_t *)&ctx;
  pgd_t *g0 = (pgd_t *)&pg0;
  pmd_t *m0 = (pmd_t *)&pm0; 
  pte_t *p0 = (pte_t *)&pt0; 
  pgd_t *ctx1_g0 = (pgd_t *)&ctx1_pg0;
  pmd_t *ctx1_m0 = (pmd_t *)&ctx1_pm0; 
  pte_t *ctx1_p0 = (pte_t *)&ctx1_pt0; 
  unsigned long pteval,j;
  unsigned long paddr, vaddr, val;
  unsigned long *pthaddr = &pth_addr1;
  functype func = mmu_func1;
  int i=0;

  leonctlr->cachectrl |= 0x1000f;

__asm__(
	".section .data\n\t"
	".align %0\n\t"
	"ctx: .skip %1\n\t"
	".align %1\n\t"
	"pg0: .skip %1\n\t"
	".align %2\n\t"
	"pm0: .skip %2\n\t"
	".align %3\n\t"
	"pt0: .skip %3\n\t"
	".align %0\n\t"
	"page0: .skip %0\n\t"
	"page1: .skip %0\n\t"
	"page2: .skip %4\n\t"
        
	".align %1\n\t"
	"ctx1_pg0: .skip %1\n\t"
	".align %2\n\t"
	"ctx1_pm0: .skip %2\n\t"
	".align %3\n\t"
	"ctx1_pt0: .skip %3\n\t"
	".align %0\n\t"
	"ctx1_page0: .skip %0\n\t"
	".text\n"
	: : "i" (PAGE_SIZE), 
	"i"(SRMMU_PGD_TABLE_SIZE) , 
	"i"(SRMMU_PMD_TABLE_SIZE) ,
	"i"(SRMMU_PTE_TABLE_SIZE) ,
      "i"((3)*PAGE_SIZE) );
   
	if (!((lr->leonconf >> MMU_CONF_BIT) & 1))
			return(0);	


  report(MMU_TEST);

 /* Prepare Page Table Hirarchy */
 #ifndef NODO_CLEAR
 /* use ram vhl model that clear mem at startup to suppress this loop */ 
 for (i = 0;i<SRMMU_PTRS_PER_CTX;i++) {
   srmmu_ctxd_set(c0+i,(pgd_t *)0);
 }
 #endif /*DO_CLEAR*/
 
 /* one-on-one mapping for context 0+1 */
 paddr = 0;
 srmmu_ctxd_set(c0+0,(pgd_t *)g0);
 srmmu_ctxd_set(c0+1,(pgd_t *)ctx1_g0);
 pteval = ((0 >> 4) | SRMMU_ET_PTE | SRMMU_PRIV);           /*0 - 1000000: ROM */
 srmmu_set_pte(g0+0, pteval);
 srmmu_set_pte(ctx1_g0+0, pteval);
 pteval = ((0x20000000 >> 4) | SRMMU_ET_PTE | SRMMU_PRIV);  /*20000000 - 21000000: IOAREA */
 srmmu_set_pte(g0+32, pteval);
 srmmu_set_pte(ctx1_g0+32, pteval);
 pteval = ((0x40000000 >> 4) | SRMMU_ET_PTE | SRMMU_PRIV | SRMMU_CACHE);  /*40000000 - 41000000: CRAM */
 srmmu_set_pte(g0+64, pteval);

 pteval = ((0x40000000 >> 4) | SRMMU_ET_PTE | SRMMU_EXEC | SRMMU_WRITE | SRMMU_CACHE);  /*40000000 - 41000000: CRAM */
 srmmu_set_pte(ctx1_g0+64, pteval);
 
 /* testarea: 
  *  map 0x40000000  at f0080000 [vaddr:(0) (240)(2)(-)] as pmd 
  *  map page0       at f0041000 [vaddr:(0) (240)(1)(1)] as page SRMMU_PRIV_RDONLY
  *  map mmu_func1() at f0042000 [vaddr:(0) (240)(1)(2)] as page
  *  map f0043000 - f007f000 [vaddr:(0) (240)(1)(3)] - [vaddr:(0) (240)(1)(63)] as page
  *  map mmu_func1() at f0043000 [vaddr:(1) (240)(1)(3)] as page
  *  map ctx1_page0  at f0044000 [vaddr:(1) (240)(1)(4)] as page
  * page fault test: 
  *  missing pgd at f1000000 [vaddr:(0) (241)(-)(-)]
  *  write protection fault at f0041000 [vaddr:(0) (240)(1)(1)]
  *  instruction page fault at f0042000 [vaddr:(0) (240)(1)(2)]
  *  instruction priv fault at f0043000 [vaddr:(1) (240)(1)(3)]
  */
 srmmu_pgd_set(g0+240,m0);
 srmmu_pgd_set(ctx1_g0+240,ctx1_m0);
 pteval = ((((unsigned long)0x40000000) >> 4) | SRMMU_ET_PTE | SRMMU_PRIV); 
 srmmu_set_pte(m0+2, pteval);
 srmmu_pmd_set(m0+1,p0);
 srmmu_pmd_set(ctx1_m0+1,ctx1_p0);
 srmmu_set_pte(p0+2, 0); /* for: instruction page fault at f0042000 [vaddr:(0) (240)(1)(2)] */
 pteval = ((((unsigned long)&page0) >> 4) | SRMMU_ET_PTE | SRMMU_PRIV_RDONLY); 
 srmmu_set_pte(p0+1, pteval); /* for: write protection fault at f0041000 [vaddr:(0) (240)(1)(1)] */
 pteval = ((((unsigned long)&ctx1_page0) >> 4) | SRMMU_ET_PTE | SRMMU_PRIV); 
 srmmu_set_pte(ctx1_p0+1, pteval);
 pteval = ((((unsigned long)func) >> 4) | SRMMU_ET_PTE | SRMMU_PRIV);  
 srmmu_set_pte(ctx1_p0+3, pteval); /* for: priviledge exc fault at f0043000 [vaddr:(1) (240)(1)(3)] */
 pteval = ((((unsigned long)&ctx1_page0) >> 4) | SRMMU_ET_PTE | SRMMU_PRIV);  
 srmmu_set_pte(ctx1_p0+4, pteval); /* for: priviledge data fault at f0044000 [vaddr:(1) (240)(1)(4)] */
 
 ((unsigned long *)&page0)[0] = 0;
 ((unsigned long *)&page0)[1] = 0x12345678;
 for (i = 3;i<TLBNUM+3;i++) {
       pteval = (((((unsigned long)&page2)+(((i-3)%3)*PAGE_SIZE)) >> 4) | SRMMU_ET_PTE | SRMMU_PRIV | SRMMU_CACHE); 
       srmmu_set_pte(p0+i, pteval);
 }

 *((unsigned long **)&pth_addr) =  pthaddr;
 /* repair info for fault (0xf1000000)*/
 pthaddr[0] = (unsigned long) (g0+241);
 pthaddr[1] = ((0x40000000 >> 4) | SRMMU_ET_PTE | SRMMU_PRIV);  
 pthaddr[2] = 0xf1000000;
 /* repair info for write protection fault (0xf0041000) */
 pthaddr[3] = (unsigned long) (p0+1);
 pthaddr[4] = ((((unsigned long)&page0) >> 4) | SRMMU_ET_PTE | SRMMU_PRIV);
 pthaddr[5] = 0xf0041000;
 /* repair info for instruction page fault (0xf0042000) */
 pthaddr[6] = (unsigned long) (p0+2);
 pthaddr[7] = ((((unsigned long)func) >> 4) | SRMMU_ET_PTE | SRMMU_PRIV);
 pthaddr[8] = 0xf0042000;
 /* repair info for privilege exec fault (0xf0043000) */
 pthaddr[9] = (unsigned long) (ctx1_p0+3);
 pthaddr[10] = ((((unsigned long)func) >> 4) | SRMMU_ET_PTE | SRMMU_EXEC | SRMMU_WRITE);
 pthaddr[11] = 0xf0042000;
 /* repair info for privilege data fault (0xf0044000) */
 pthaddr[12] = (unsigned long) (ctx1_p0+4);
 pthaddr[13] = ((((unsigned long)&ctx1_page0) >> 4) | SRMMU_ET_PTE | SRMMU_EXEC | SRMMU_WRITE);
 pthaddr[14] = 0xf0044000;
 
 srmmu_set_ctable_ptr((unsigned long)c0);
 srmmu_set_context(0);
 /* close your eyes and pray ... */
 srmmu_set_mmureg(0x00000001);

 __asm__ __volatile__("set 0xf0043000,%%g1\n\t"\
                      "set 0xf0044000,%%g2\n\t"\
                      "set 0xabcdef01,%%g3\n\t"\
                      "ld  [%%g1],%%g4\n\t"\
		      "st  %%g3, [%%g1]\n\t"\
                      "st  %%g3, [%%g2]\n\t"\
		      "ld  [%%g2],%%g4\n\t" : : :
                      "g1","g2","g3","g4");
 
 
 asm(" sta	%g0, [%g0] 0x5 "); //iflush 
 asm(" sta	%g0, [%g0] 0x6 "); //dflush
 
 /* do tests*/
 if ( (*((unsigned long *)0xf0041000)) != 0 ||
      (*((unsigned long *)0xf0041004)) != 0x12345678 ) { fail(1); }
 if ( (*((unsigned long *)0xf0080000)) != (*((unsigned long *)0x40000000))) { fail(2); }
 
 /* page faults tests*/
 val = * ((volatile unsigned long *)0xf1000000);
 /* write protection fault */
 * ((volatile unsigned long *)0xf0041004) = 0x87654321;
 if ( (*((volatile unsigned long *)0xf0041004)) != 0x87654321 ) { fail(3); }
 /* doubleword write */
 __asm__ __volatile__("set 0xf0041000,%%g1\n\t"\
                      "set 0x12345678,%%g2\n\t"\
                      "set 0xabcdef01,%%g3\n\t"\
                      "std %%g2, [%%g1]\n\t"\
                      "std %%g2, [%%g1]\n\t": : :
                      "g1","g2","g3");
 if ( (*((volatile unsigned long *)0xf0041000)) != 0x12345678 ||
      (*((volatile unsigned long *)0xf0041004)) != 0xabcdef01) { fail(4); }
 
 for (j=0xf0043000,i = 3;i<TLBNUM+3;i++,j+=0x1000) {
       *((unsigned long *)j) = j;
       asm(" sta	%g0, [%g0] 0x6 "); //dflush
       if ( *((unsigned long*) (((unsigned long)&page2)+(((i-3)%3)*PAGE_SIZE))) != j ) { fail(5); }
 }
 asm(" sta	%g0, [%g0] 0x6 "); //dflush
 for (j=0,i = 3;i<TLBNUM+3;i++) {
       pteval = (((((unsigned long)&page2)+(((i-3)%3)*PAGE_SIZE)) >> 4) | SRMMU_ET_PTE | SRMMU_PRIV | SRMMU_CACHE); 
       if ((*(p0+i)) & (SRMMU_DIRTY | SRMMU_REF)) j++;
       if (((*(p0+i)) & ~(SRMMU_DIRTY | SRMMU_REF))  != (pteval& ~(SRMMU_DIRTY | SRMMU_REF))) { fail(6); }
 }
 //at least one entry has to have been flushed
 if (j == 0) { fail(7);}
 
 /* instruction page fault */
 func = (functype)0xf0042000;
 func();
 
 /* flush */
 srmmu_flush_whole_tlb();
 asm(" sta	%g0, [%g0] 0x6 "); //dflush
       
 for (j=0,i = 3;i<TLBNUM+3;i++) {
       if ((*(p0+i)) & (SRMMU_DIRTY | SRMMU_REF)) j++;
 }
 if (j != TLBNUM) { fail(8);}
 
 /* check modified & ref bit */
 if (!srmmu_pte_dirty(p0[1]) || !srmmu_pte_young(p0[1])) { fail(9); };
 if (!srmmu_pte_young(m0[2])) { fail(10); };
 if (!srmmu_pte_young(p0[2])) { fail(11); };

 /*check precise tlb flushing */
 srmmu_flush_whole_tlb();
 asm(" sta	%g0, [%g0] 0x6 "); //dflush
 /* fill tlb */
 for (j=0xf0043000,i = 3;i<TLBNUM+3;i++,j+=0x1000) {
      unsigned int v = *((unsigned long *)j);
 }
 srmmu_flush_pte_tlb(0xf004a000); /* (check in modelsim) */ 
 /* check precise tlb flushing */
 
 
 /* check user/supervisor mode */
 srmmu_set_context(1);
 __asm__ __volatile__("ta 9\n\t"); /* enter user mode */
 /* instruction provilege exec fault */
 func = (functype)0xf0043000;
 func();
 /* data provilege access fault */
 __asm__ __volatile__("set 0xf0044000,%%g1\n\t"\
                      "set 0x12345678,%%g2\n\t"\
                      "set 0xabcdef01,%%g3\n\t"\
                      "std %%g2, [%%g1]\n\t"\
                      "std %%g2, [%%g1]\n\t": : :
                      "g1","g2","g3");
 if ( (*((volatile unsigned long *)0xf0044000)) != 0x12345678 ||
      (*((volatile unsigned long *)0xf0044004)) != 0xabcdef01) { fail(12); }
 
 __asm__ __volatile__("ta 8\n\t"); /* enter supervisor mode again*/
 srmmu_set_context(0);
 
 srmmu_set_mmureg(0x00000000);
 
}


