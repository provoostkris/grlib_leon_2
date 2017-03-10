#include "leon.h"
#include "test.h" 
#include "mmu.h" 
 
	.seg	"text"
	
/* SRMMU_TFAULT() SRMMU_DFAULT()
 * %l0 : %psr
 * %l1 = pc            :set by trap
 * %l2 = npc           :set by trap
 * %l7 : 1 on inst, 0 on data
 */
	.global srmmu_fault, _mmu_switch
_mmu_switch:    
srmmu_fault:
	jmp	%l1				! reexecute command
	rett	%l2

	.align 8 
	.global pth_addr, pth_addr1
pth_addr:	.word 0
	.align 8 
pth_addr1:	.word 0,0,0,0,0,0,0,0,0
		
	