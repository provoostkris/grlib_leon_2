
----------------------------------------------------------------------------
--  This file is a part of the LEON VHDL model
--  Copyright (C) 1999  European Space Agency (ESA)
--
--  This library is free software; you can redistribute it and/or
--  modify it under the terms of the GNU Lesser General Public
--  License as published by the Free Software Foundation; either
--  version 2 of the License, or (at your option) any later version.
--
--  See the file COPYING.LGPL for the full details of the license.


-----------------------------------------------------------------------------
-- Entity: 	device
-- File:	device.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	package to select current device configuration
------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use work.target.all;

package device is

-----------------------------------------------------------------------------
-- Automatically generated by tkonfig/mkdevice
-----------------------------------------------------------------------------

  constant syn_config : syn_config_type := (  
    targettech => gen , infer_pads => false, infer_pci => false,
    infer_ram => false, infer_regf => false, infer_rom => false,
    infer_mult => true, rftype => 2, targetclk => gen,
    clk_mul => 1, clk_div => 1, pci_dll => false, pci_sysclk => false );

  constant iu_config : iu_config_type := (
    nwindows => 8, multiplier => m16x16, mulpipe => true, 
    divider => radix2, mac => false, fpuen => 0, cpen => false, 
    fastjump => true, icchold => true, lddelay => 1, fastdecode => true, 
    rflowpow => false, watchpoints => 2);

  constant fpu_config : fpu_config_type := 
    (core => meiko, interface => none, fregs => 0, version => 0);

  constant cache_config : cache_config_type := (
    isets => 1, isetsize => 8, ilinesize => 8, ireplace => rnd, ilock => 0,
    ilram => false, ilramsize => 1, ilramaddr => 16#8E#,
    dsets => 1, dsetsize => 8, dlinesize => 8, dreplace => rnd, dlock => 0,
    dsnoop => none, drfast => false, dwfast => false, dlram => false,
    dlramsize => 1, dlramaddr => 16#8F#);

  constant mmu_config : mmu_config_type := (
    enable => 0, itlbnum => 8, dtlbnum => 8, tlb_type => combinedtlb, 
    tlb_rep => replruarray, tlb_diag => false );

  constant ahbrange_config  : ahbslv_addr_type := 
        (0,0,0,0,0,0,0,0,1,2,7,7,7,7,7,7);

  constant ahb_config : ahb_config_type := ( masters => 2, defmst => 0,
    split => false, testmod => false);

  constant mctrl_config : mctrl_config_type := (
    bus8en => true, bus16en => false, wendfb => false, ramsel5 => false,
    sdramen => false, sdinvclk => false, sdsepbus => false, 
    sd64 => false);

  constant peri_config : peri_config_type := (
    cfgreg => true, ahbstat => false, wprot => false, wdog => false, 
    irq2en => false, ahbram => false, ahbrambits => 11, ethen => false );

  constant debug_config : debug_config_type := ( enable => true, uart => false, 
    iureg => false, fpureg => false, nohalt => false, pclow => 2,
    dsuenable => true, dsutrace => true, dsumixed => true,
    dsudpram => false, tracelines => 512);

  constant boot_config : boot_config_type := (boot => memory, ramrws => 0,
    ramwws => 0, sysclk => 25000000, baud => 19200, extbaud => false,
    pabits => 11);

  constant pci_config : pci_config_type := (
    pcicore => none , ahbmasters => 0, fifodepth => 8,
    arbiter => false, fixpri => false, prilevels => 4, pcimasters => 4,
    vendorid => 16#0000#, deviceid => 16#0000#, subsysid => 16#0000#,
    revisionid => 16#00#, classcode =>16#000000#, pmepads => false,
    p66pad => false, pcirstall => false, trace => false, tracedepth => 256);

  constant irq2cfg : irq2type := irq2none;


-----------------------------------------------------------------------------
-- end of automatic configuration
-----------------------------------------------------------------------------

end;
