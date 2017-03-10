
`define HEADER_VENDOR_ID    16'h0000
`define HEADER_DEVICE_ID    16'h0000
`define HEADER_REVISION_ID  8'h00

`define WB_RAM_DONT_SHARE
`define PCI_RAM_DONT_SHARE
`define WBW_ADDR_LENGTH 8
`define WBR_ADDR_LENGTH 8
`define PCIW_ADDR_LENGTH 8
`define PCIR_ADDR_LENGTH 8
`define PCI_FIFO_RAM_ADDR_LENGTH 8 
`define WB_FIFO_RAM_ADDR_LENGTH 8    

`define ETH_WISHBONE_B3

`define ETH_TX_FIFO_CNT_WIDTH  4
`define ETH_TX_FIFO_DEPTH      8

`define ETH_RX_FIFO_CNT_WIDTH  4
`define ETH_RX_FIFO_DEPTH      8

`define ETH_BURST_CNT_WIDTH    3
`define ETH_BURST_LENGTH       4
