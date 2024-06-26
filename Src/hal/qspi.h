#ifndef _QSPI_H
#define _QSPI_H

#include <stdint.h>

/* QUADSPI_CR */
#define QUADSPI_CR_EN				(1 << 0)
#define QUADSPI_CR_TCEN				(1 << 3)
#define QUADSPI_CR_SSHIFT			(1 << 4)
#define QUADSPI_CR_DFM				(1 << 6)
#define QUADSPI_CR_FSEL				(1 << 7)
#define QUADSPI_CR_FTHRES(x)		((x) << 8)
#define QUADSPI_CR_TOIE				(1 << 20)
#define QUADSPI_CR_AMPS				(1 << 22)
#define QUADSPI_CR_PRESCALER(x)		((x) << 24)

#define QUADSPI_CR_PRESCALER_MASK	QUADSPI_CR_PRESCALER(0xff)

/* QUADSPI_DCR */
#define QUADSPI_DCR_CSHT(x)			((x) << 8)
#define QUADSPI_DCR_FSIZE(x)		((x) << 16)

#define QUADSPI_DCR_CSHT_MASK		QUADSPI_DCR_CSHT(0x7)
#define QUADSPI_DCR_FSIZE_MASK		QUADSPI_DCR_FSIZE(0x1f)
#define QUADSPI_DCR_FSIZE_8MB		QUADSPI_DCR_FSIZE(22)
#define QUADSPI_DCR_FSIZE_16MB		QUADSPI_DCR_FSIZE(23)
#define QUADSPI_DCR_FSIZE_32MB		QUADSPI_DCR_FSIZE(24)
#define QUADSPI_DCR_FSIZE_64MB		QUADSPI_DCR_FSIZE(25)
#define QUADSPI_DCR_FSIZE_128MB		QUADSPI_DCR_FSIZE(26)

/* QUADSPI_SR */
#define QUADSPI_SR_TCF				(1 << 1)
#define QUADSPI_SR_FTF				(1 << 2)
#define QUADSPI_SR_SMF				(1 << 3)
#define QUADSPI_SR_BUSY				(1 << 5)

/* QUADSPI_FCR */
#define QUADSPI_FCR_CTCF			(1 << 1)
#define QUADSPI_FCR_CSMF			(1 << 3)

/* QUADSPI_CCR */
#define QUADSPI_CCR_IDMODE(x)		((x) << 8)
#define QUADSPI_CCR_ADMODE(x)		((x) << 10)
#define QUADSPI_CCR_ADSIZE(x)		((x) << 12)
#define QUADSPI_CCR_ABMODE(x)		((x) << 14)
#define QUADSPI_CCR_ABSIZE(x)		((x) << 16)
#define QUADSPI_CCR_DCYC(x)			((x) << 18)
#define QUADSPI_CCR_DMODE(x)		((x) << 24)
#define QUADSPI_CCR_FMODE(x)		((x) << 26)
#define QUADSPI_CCR_SIOMODE(x)          ((x) << 28)

#define QUADSPI_CCR_IDMOD_1_LINE	QUADSPI_CCR_IDMODE(1)
#define QUADSPI_CCR_ADMOD_1_LINE	QUADSPI_CCR_ADMODE(1)
#define QUADSPI_CCR_ADMOD_4_LINE	QUADSPI_CCR_ADMODE(3)
#define QUADSPI_CCR_ADSIZE_24BITS	QUADSPI_CCR_ADSIZE(2)
#define QUADSPI_CCR_ADSIZE_32BITS	QUADSPI_CCR_ADSIZE(3)
#define QUADSPI_CCR_ABMOD_4_LINE	QUADSPI_CCR_ABMODE(3)
#define QUADSPI_CCR_ABSIZE_8BITS	QUADSPI_CCR_ABSIZE(0)
#define QUADSPI_CCR_DMODE_1_LINE	QUADSPI_CCR_DMODE(1)
#define QUADSPI_CCR_DMODE_4_LINES	QUADSPI_CCR_DMODE(3)
#define QUADSPI_CCR_FMODE_IND_WR	QUADSPI_CCR_FMODE(0)
#define QUADSPI_CCR_FMODE_IND_RD	QUADSPI_CCR_FMODE(1)
#define QUADSPI_CCR_FMODE_AUTO_POLL	QUADSPI_CCR_FMODE(2)
#define QUADSPI_CCR_FMODE_MEMMAP	QUADSPI_CCR_FMODE(3)
#define QUADSPI_CCR_SIOMODE_ALWAYS	QUADSPI_CCR_SIOMODE(0)
#define QUADSPI_CCR_SIOMODE_ONCE	QUADSPI_CCR_SIOMODE(1)



/*  QSPI Comands */
#define READ_STATUS_REG_CMD			0x05
#define WRITE_ENABLE_CMD			0x06
#define RESET_ENABLE_CMD			0x66
#define QUAD_OUTPUT_FAST_READ_CMD	0x6b
#define QUAD_IO_FAST_READ_CMD	0xeb
#define WRITE_VOL_CFG_REG_CMD		0x81
#define READ_VOL_CFG_REG_CMD		0x85
#define RESET_MEMORY_CMD			0x99
#define ENTER_4_BYTE_ADDR_MODE_CMD	0xb7


#define QUADSPI_CR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x00))
#define QUADSPI_DCR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x04))
#define QUADSPI_SR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x08))
#define QUADSPI_FCR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x0c))
#define QUADSPI_DLR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x10))
#define QUADSPI_CCR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x14))
#define QUADSPI_AR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x18))
#define QUADSPI_ABR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x1C))
#define QUADSPI_DR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x20))
#define QUADSPI_PSMKR    (*(volatile unsigned long *)(QUADSPI_BASE + 0x24))
#define QUADSPI_PSMAR    (*(volatile unsigned long *)(QUADSPI_BASE + 0x28))
#define QUADSPI_PIR	 (*(volatile unsigned long *)(QUADSPI_BASE + 0x2c))

/* N25Q512A Registers*/

/* N25Q512A_SR */
#define N25Q512A_SR_WIP				(1 << 0)
#define N25Q512A_SR_WREN			(1 << 1)

struct qspi_params {
	uint32_t address_size;
	uint32_t fifo_threshold;
	uint32_t prescaler;
	uint32_t sshift;
	uint32_t fsel;
	uint32_t dfm;
	uint32_t dummy_cycle;
	uint32_t fsize;
};

void quadspi_init(struct qspi_params *params, void *base);
void quadspi_erase_sector(uint32_t sector);
void quadspi_write(uint32_t address,uint8_t *data,int len);
void quadspi_mmap(void);

#endif /* _QSPI_H */
