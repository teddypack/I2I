/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2021
 * Generated linker script file for LPC11U68
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v11.3.0 [Build 5222] [2021-01-11] on Mar 15, 2021 9:58:58 AM
 */

MEMORY
{
  /* Define each memory region */
  MFlash256 (rx) : ORIGIN = 0x0, LENGTH = 0x40000 /* 256K bytes (alias Flash) */  
  Ram0_32 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x8000 /* 32K bytes (alias RAM) */  
  Ram1_2 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x800 /* 2K bytes (alias RAM2) */  
  Ram2USB_2 (rwx) : ORIGIN = 0x20004000, LENGTH = 0x800 /* 2K bytes (alias RAM3) */  
}

  /* Define a symbol for the top of each memory region */
  __base_MFlash256 = 0x0  ; /* MFlash256 */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_MFlash256 = 0x0 + 0x40000 ; /* 256K bytes */  
  __top_Flash = 0x0 + 0x40000 ; /* 256K bytes */  
  __base_Ram0_32 = 0x10000000  ; /* Ram0_32 */  
  __base_RAM = 0x10000000 ; /* RAM */  
  __top_Ram0_32 = 0x10000000 + 0x8000 ; /* 32K bytes */  
  __top_RAM = 0x10000000 + 0x8000 ; /* 32K bytes */  
  __base_Ram1_2 = 0x20000000  ; /* Ram1_2 */  
  __base_RAM2 = 0x20000000 ; /* RAM2 */  
  __top_Ram1_2 = 0x20000000 + 0x800 ; /* 2K bytes */  
  __top_RAM2 = 0x20000000 + 0x800 ; /* 2K bytes */  
  __base_Ram2USB_2 = 0x20004000  ; /* Ram2USB_2 */  
  __base_RAM3 = 0x20004000 ; /* RAM3 */  
  __top_Ram2USB_2 = 0x20004000 + 0x800 ; /* 2K bytes */  
  __top_RAM3 = 0x20004000 + 0x800 ; /* 2K bytes */  
