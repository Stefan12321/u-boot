/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 *
 * Configuration settings for the Embedded Artists i.MX7 ULP uCOM Board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX7ULPEA_UCOM_H
#define __MX7ULPEA_UCOM_H

#include <linux/sizes.h>
#include <asm/arch/imx-regs.h>
#include "configs/imx_env.h"

#ifdef CONFIG_SPL_BUILD

#define CONFIG_SERIAL_LPUART_BASE      0x402D0000

#endif /* CONFIG_SPL_BUILD */

#ifdef CONFIG_SECURE_BOOT
#ifndef CONFIG_CSF_SIZE
#define CONFIG_CSF_SIZE			0x4000
#endif
#endif

#define CONFIG_BOARD_POSTCLK_INIT
#define CONFIG_SYS_BOOTM_LEN		0x1000000

#define SRC_BASE_ADDR			CMC1_RBASE
#define IRAM_BASE_ADDR			OCRAM_0_BASE
#define IOMUXC_BASE_ADDR		IOMUXC1_RBASE

/* Fuses */
#define CONFIG_CMD_FUSE
#define CONFIG_MXC_OCOTP

#define CONFIG_SERIAL_TAG

#define CONFIG_BOUNCE_BUFFER
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */

#define CONFIG_SYS_FSL_USDHC_NUM        1

#define CONFIG_SYS_FSL_ESDHC_ADDR       0
#define CONFIG_SYS_MMC_ENV_DEV          0	/* USDHC1 */
#define CONFIG_SYS_MMC_ENV_PART		1       /* 0=user area, 1=1st MMC boot part., 2=2nd MMC boot part. */
#define CONFIG_MMCROOT                  "/dev/mmcblk0p2"  /* USDHC1 */
#define CONFIG_SYS_MMC_IMG_LOAD_PART    1

#define CONFIG_ENV_OFFSET		(SZ_2M - CONFIG_ENV_SIZE)
#define CONFIG_ENV_SIZE			SZ_8K

/* EA TODO: ---> */
#define CONFIG_SYS_I2C_SPEED              100000
/* EA TODO: <--- */


/* Using ULP WDOG for reset */
#define WDOG_BASE_ADDR			WDG1_RBASE

#define CONFIG_SYS_ARCH_TIMER
#define CONFIG_SYS_HZ_CLOCK		1000000 /* Fixed at 1Mhz from TSTMR */

#define CONFIG_INITRD_TAG
#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
/*#define CONFIG_REVISION_TAG*/

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(8 * SZ_1M)

/* UART */
#define LPUART_BASE			LPUART4_RBASE

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200

#define CONFIG_SYS_CACHELINE_SIZE      64

/* Miscellaneous configurable options */
#define CONFIG_SYS_PROMPT		"=> "
#define CONFIG_SYS_CBSIZE		512

#define CONFIG_SYS_MAXARGS		256

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1

#define PHYS_SDRAM			0x60000000ul
#define PHYS_SDRAM_SIZE			SZ_1G
#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM

#define CONFIG_LOADADDR             0x60800000

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_END      (CONFIG_SYS_MEMTEST_START + (PHYS_SDRAM_SIZE >> 1))

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT  \
	"initrd_addr=0x63800000\0" \
	"initrd_high=0xffffffff\0" \
	"emmc_dev=0\0" \
	"sd_dev=0\0"

#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	TEE_ENV \
	"script=boot.scr\0" \
	"image=zImage\0" \
	"console=ttyLP0\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"fdt_file=" CONFIG_DEFAULT_FDT_FILE ".dtb\0" \
	"fdt_addr=0x63000000\0" \
	"tee_addr=0x64000000\0" \
	"tee_file=uTee-7ulp\0" \
	"boot_fdt=try\0" \
	"earlycon=lpuart32,0x402D0010\0" \
	"ip_dyn=yes\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcautodetect=yes\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=${mmcroot} ${args_from_script}\0" \
	"loadbootscript=" \
		"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"loadtee=fatload mmc ${mmcdev}:${mmcpart} ${tee_addr} ${tee_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"if test ${tee} = yes; then " \
			"run loadfdt; run loadtee; bootm ${tee_addr} - ${fdt_addr}; " \
		"else " \
			"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
				"if run loadfdt; then " \
					"bootz ${loadaddr} - ${fdt_addr}; " \
				"else " \
					"if test ${boot_fdt} = try; then " \
						"bootz; " \
					"else " \
						"echo WARN: Cannot load the DT; " \
					"fi; " \
				"fi; " \
			"else " \
				"bootz; " \
			"fi; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console},${baudrate} " \
		"root=/dev/nfs " \
		"ip=:::::eth0:dhcp nfsroot=${serverip}:${nfsroot},v3,tcp " \
		"${args_from_script}\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"usb start; "\
		"${get_cmd} ${image}; " \
		"if test ${tee} = yes; then " \
			"${get_cmd} ${tee_addr} ${tee_file}; " \
			"${get_cmd} ${fdt_addr} ${fdt_file}; " \
			"bootm ${tee_addr} - ${fdt_addr}; " \
		"else " \
			"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
				"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
					"bootz ${loadaddr} - ${fdt_addr}; " \
				"else " \
					"if test ${boot_fdt} = try; then " \
						"bootz; " \
					"else " \
						"echo WARN: Cannot load the DT; " \
					"fi; " \
				"fi; " \
			"else " \
				"bootz; " \
			"fi; " \
		"fi;\0" \

#define CONFIG_BOOTCOMMAND \
	   "mmc dev ${mmcdev}; if mmc rescan; then " \
		   "if run loadbootscript; then " \
			   "run bootscript; " \
		   "else " \
			   "if run loadimage; then " \
				   "run mmcboot; " \
			   "else run netboot; " \
			   "fi; " \
		   "fi; " \
	   "fi"

#define CONFIG_SYS_HZ			1000
#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	SZ_256K

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

/* QSPI configs */
#ifdef CONFIG_FSL_QSPI
#define CONFIG_SYS_FSL_QSPI_AHB
#define CONFIG_SF_DEFAULT_BUS           0
#define CONFIG_SF_DEFAULT_CS            0
#define CONFIG_SF_DEFAULT_SPEED         40000000
#define CONFIG_SF_DEFAULT_MODE          SPI_MODE_0
#define FSL_QSPI_FLASH_NUM              1
#define FSL_QSPI_FLASH_SIZE             SZ_8M
#define QSPI0_BASE_ADDR                 0x410A5000
#define QSPI0_AMBA_BASE                 0xC0000000
#endif

/* USB Configs */
#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)

#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_MXS
#define CONFIG_VIDEO_LOGO
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_VIDEO_SKIP

#define CONFIG_HX8363
#endif

#define CONFIG_OF_SYSTEM_SETUP

#if defined(CONFIG_ANDROID_SUPPORT)
#include "mx7ulp_evk_android.h"
#endif

#ifdef CONFIG_IMX_OPTEE
#define TEE_ENV "tee=yes\0"
#else
#define TEE_ENV "tee=no\0"
#endif
#endif	/* __CONFIG_H */
