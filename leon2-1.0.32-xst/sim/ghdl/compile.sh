#
set -x
mkdir leon/work
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/amba.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/target.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/device.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/config.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmuconfig.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/sparcv8.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/iface.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/macro.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/debug.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/ambacomp.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/multlib.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_generic.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_proasic.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_axcel.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_atc18.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_tsmc25.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_atc35.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_atc25.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_atc18.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_umc18.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_fs90.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/bprom.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_virtex.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_virtex2.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/tech_map.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/fpulib.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/meiko.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/fpu_lth.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/fpu_core.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/grfpc.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/fp1eu.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmu_icache.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmu_dcache.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmu_acache.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmutlbcam.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmulrue.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmulru.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmutlb.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmutw.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmu.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mmu_cache.vhd

ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mul.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/div.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/rstgen.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/iu.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/icache.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/dcache.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/cachemem.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/acache.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/cache.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/proc.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/irqctrl2.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/apbmst.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/ahbarb.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/ahbram.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/lconf.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/wprot.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/ahbtest.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/ahbstat.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/timers.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/irqctrl.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/uart.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/ioport.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/sdmctrl.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mctrl.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/ahbmst.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/dcom_uart.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/dcom.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/dma.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/dsu.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/dsu_mem.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/pci_arb.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/pci_gr.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/pci.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/eth_oc.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/mcore.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/leon.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/leon_pci.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/leon_eth_pci.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 leon/leon_eth.vhd 

ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/leonlib.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/iram.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/mt48lc16m16a2.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/testmod.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/mspram.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/bprom.vhd
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/tbgen.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/tblib.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/tbdef.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/tbleon.vhd 
ghdl -a --workdir=leon/work --ieee=synopsys --std=87 tbench/tb_msp.vhd

ghdl -e  --workdir=leon/work --ieee=synopsys --std=87  tb_func32
