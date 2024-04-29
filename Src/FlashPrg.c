/*********************************************************************
*               (c) SEGGER Microcontroller GmbH & Co. KG             *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************
----------------------------------------------------------------------
File    : FlashPrg.c
Purpose : Implementation of RAMCode template
--------  END-OF-HEADER  ---------------------------------------------
*/
#include "FlashOS.h"
#include "stm32h7_regs.h"
#include "qspi.h"
#include "gpio.h"

void clock_setup(void);
void qspi_init(void);

static void _FeedWatchdog(void);


// segger_flash_loader compilation options
//
// Only compile in functions that make sense to keep RAMCode as small as possible
//
// Non-memory mapped flashes only. Flash cannot be read memory-mapped
// If enabled, the J-Link will make use of the Verify() in order to verify flash contents.
// Otherwise, the built-in memory-mapped readback function will be used.
#define SUPPORT_NATIVE_VERIFY                   (0)
// Non-memory mapped flashes only. Flash cannot be read memory-mapped
// If enabled, the J-Link will make use of the SEGGER_OPEN_Read() in order to read a specified number
// of bytes from flash. Otherwise, the built-in memory-mapped readback function will be used.
#define SUPPORT_NATIVE_READ_FUNCTION            (0)
// To potentially speed up production programming: Erases whole flash bank / chip with special command
#define SUPPORT_ERASE_CHIP                      (0)
// Currently available for Cortex-M only
#define SUPPORT_TURBO_MODE                      (0)
// Flashes with uniform sectors only. Speed up erase because 1 OFL call may erase multiple sectors
#define SUPPORT_SEGGER_OPEN_ERASE               (0)
// If disabled, the J-Link software will assume that erased state of a sector can be determined via
// normal memory-mapped readback of sector.
#define SUPPORT_BLANK_CHECK                     (0)



typedef struct {
        U32 AddVariablesHere;
} RESTORE_INFO;
static RESTORE_INFO _RestoreInfo;


/*
 * Mark start of <PrgData> segment. Non-static to make sure linker can keep this symbol.
 * Dummy needed to make sure that <PrgData> section in resulting ELF file is present.
 * Needed by open flash loader logic on PC side
 */
volatile int PRGDATA_StartMarker __attribute__((section ("PrgData")));


#if (SUPPORT_ERASE_CHIP == 0)
#       define EraseChip NULL
#endif
#if (SUPPORT_NATIVE_VERIFY == 0)
#       define Verify NULL
#endif
#if (SUPPORT_NATIVE_READ_FUNCTION == 0)
#       define SEGGER_OPEN_Read NULL
#endif
#if (SUPPORT_SEGGER_OPEN_ERASE == 0)
#       define SEGGER_OPEN_Erase NULL
#endif
#if (SUPPORT_SEGGER_OPEN_Program == 0)
#       define SEGGER_OPEN_Program NULL
#endif
#if (SUPPORT_TURBO_MODE == 0)
#       define SEGGER_OPEN_Start NULL
#endif
#if (SUPPORT_BLANK_CHECK == 0)
#       define BlankCheck NULL
#endif

// Mark start of <PrgCode> segment. Non-static to make sure linker can keep this symbol.
const SEGGER_OFL_API SEGGER_OFL_Api/* __attribute__((section ("PrgCode")))*/ =
{
        _FeedWatchdog,
        Init,
        UnInit,
        EraseSector,
        ProgramPage,
        BlankCheck,
        EraseChip,
        Verify,
        SEGGER_OPEN_CalcCRC,
        SEGGER_OPEN_Read,
        SEGGER_OPEN_Program,
        SEGGER_OPEN_Erase,
        SEGGER_OPEN_Start
};



/*********************************************************************
*
*       Defines (configurable)
*
**********************************************************************
*/
#define PAGE_SIZE_SHIFT          (3)      // The smallest program unit (one page) is 8 uint8_t in size
//
// Some flash types require a native verify function as the memory is not memory mapped available (e.g. eMMC flashes).
// If the verify function is implemented in the algorithm, it will be used by the J-Link DLL during compare / verify
// independent of what verify type is configured in the J-Link DLL.
// Please note, that SEGGER does not recommend to use this function if the flash can be memory mapped read
// as this may can slow-down the compare / verify step.
//
#define SUPPORT_NATIVE_VERIFY    (0)
#define SUPPORT_NATIVE_READ_BACK (0)
#define SUPPORT_BLANK_CHECK      (0)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// We use this dummy variable to make sure that the PrgData
// section is present in the output elf-file as this section
// is mandatory in current versions of the J-Link DLL 
//
//static volatile int _Dummy;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _FeedWatchdog
*
*  Function description
*    Feeds the watchdog. Needs to be called during RAMCode execution
*    in case of an watchdog is active.
*/
static void _FeedWatchdog(void) {
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       Init
*
*  Function description
*    Handles the initialization of the flash module.
*
*  Parameters
*    Addr: Flash base address
*    Freq: Clock frequency in Hz
*    Func: Caller type (e.g.: 1 - Erase, 2 - Program, 3 - Verify)
*
*  Return value 
*    0 O.K.
*    1 Error
*/
int Init(U32 Addr, U32 Freq, U32 Func) {
  (void)Addr;
  (void)Freq;
  (void)Func;
  //
  // Init code
  //
  clock_setup();

  gpio_set_qspi(GPIOA_BASE,'B',2,GPIOx_PUPDR_NOPULL, 0x9);
  gpio_set_qspi(GPIOA_BASE,'B',6,GPIOx_PUPDR_NOPULL, 0xA);
  gpio_set_qspi(GPIOA_BASE,'D',11,GPIOx_PUPDR_NOPULL, 0x9);
  gpio_set_qspi(GPIOA_BASE,'D',12,GPIOx_PUPDR_NOPULL, 0x9);
  gpio_set_qspi(GPIOA_BASE,'D',13,GPIOx_PUPDR_NOPULL, 0x9);
  gpio_set_qspi(GPIOA_BASE,'E',2,GPIOx_PUPDR_NOPULL, 0x9);

  quadspi_init(0, (void *)QUADSPI_BASE);

  if(Func != 1 )
    quadspi_mmap();

  return 0;
}

/*********************************************************************
*
*       UnInit
*
*  Function description
*    Handles the de-initialization of the flash module.
*
*  Parameters
*    Func: Caller type (e.g.: 1 - Erase, 2 - Program, 3 - Verify)
*
*  Return value 
*    0 O.K.
*    1 Error
*/
int UnInit(U32 Func) {
  (void)Func;
  //
  // Uninit code
  //
  quadspi_init(0, (void *)QUADSPI_BASE);

  quadspi_mmap();

  return 0;
}

/*********************************************************************
*
*       EraseSector
*
*  Function description
*    Erases one flash sector.
*
*  Parameters
*    Addr: Address of the sector to be erased
*
*  Return value 
*    0 O.K.
*    1 Error
*/
int EraseSector(U32 SectorAddr) {

  SectorAddr -= 0x90000000;
  quadspi_erase_sector(SectorAddr);
  //_FeedWatchdog();
  return 0;
}

/*********************************************************************
*
*       ProgramPage
*
*  Function description
*    Programs one flash page.
*
*  Parameters
*    DestAddr: Destination address
*    NumBytes: Number of bytes to be programmed (always a multiple of program page size, defined in FlashDev.c)
*    pSrcBuff: Point to the source buffer
*
*  Return value 
*    0 O.K.
*    1 Error
*/
int ProgramPage(U32 DestAddr, U32 NumBytes, U8 *pSrcBuff) {
  

  DestAddr -= 0x90000000;
  quadspi_write(DestAddr,pSrcBuff,NumBytes);
  return 0;
}

/*********************************************************************
*
*       Verify
*
*  Function description
*    Compares a specified number of bytes of a provided data
*    buffer with the content of the device
*
*  Parameters
*    Addr: Start address in memory which should be compared
*    NumBytes: Number of bytes to be compared
*    pBuff: Pointer to the data to be compared
*
*  Return value 
*    == (Addr + NumBytes): O.K.
*    != (Addr + NumBytes): *not* O.K.
*
*/
#if SUPPORT_NATIVE_VERIFY
U32 Verify(U32 Addr, U32 NumBytes, U8 *pBuff) {
  unsigned char *pFlash;
  unsigned long r;

  pFlash = (unsigned char *)Addr;
  r = Addr + NumBytes;
  do {
      if (*pFlash != *pBuff) {
        r = (unsigned long)pFlash;
        break;
      }
      pFlash++;
      pBuff++;
  } while (--NumBytes);
  return r;
}
#endif

/*********************************************************************
*
*       BlankCheck
*
*  Function description
*    Checks if a memory region is blank
*
*  Parameters
*    Addr: Blank check start address
*    NumBytes: Number of bytes to be checked
*    BlankData: Pointer to the destination data
*
*  Return value 
*    0 O.K., blank
*    1 O.K., *not* blank
*    <  0 Error
*
*/
#if SUPPORT_BLANK_CHECK
int BlankCheck(U32 Addr, U32 NumBytes, U8 BlankData) {
  U8* pData;
  
  pData = (U8 *)Addr;
  do {
    if (*pData++ != BlankData) {
      return 1;
    }
  } while (--NumBytes);
  return 0;
}
#endif

/*********************************************************************
*
*       SEGGER_OPEN_Read
*
*  Function description
*    Reads a specified number of bytes into the provided buffer
*
*  Parameters
*    Addr: Start read address
*    NumBytes: Number of bytes to be read
*    pBuff: Pointer to the destination data
*
*  Return value 
*    >= 0 O.K., NumBytes read
*    <  0 Error
*
*/
#if SUPPORT_NATIVE_READ_BACK
int SEGGER_OPEN_Read(U32 Addr, U32 NumBytes, U8 *pDestBuff) {
  //
  // Read function
  // Add your code here...
  //
  return NumBytes;
}
#endif


/*********************************************************************
*
*       SEGGER_OPEN_Erase
*
*  Function description
*    Erases one or more flash sectors.
*    The implementation from this template only works on flashes that have uniform sectors.
*
*  Notes
*    (1) This function can rely on that at least one sector will be passed
*    (2) This function must be able to handle multiple sectors at once
*    (3) This function can rely on that only multiple sectors of the same sector
*        size will be passed. (e.g. if the device has two sectors with different
*        sizes, the DLL will call this function two times with NumSectors = 1)
*
*  Parameters
*    SectorAddr:  Address of the start sector to be erased
*    SectorIndex: Index of the start sector to be erased (1st sector handled by this flash bank:
*                 SectorIndex == 0)
*    NumSectors:  Number of sectors to be erased. Min. 1
*
*  Return value
*    == 0  O.K.
*    == 1  Error
*
*  Notes
*    (1) This function is optional. If not present, the J-Link software will use EraseSector()
*    (2) Use "noinline" attribute to make sure that function is never inlined and label not
*        accidentally removed by linker from ELF file.
*/
#if SUPPORT_SEGGER_OPEN_ERASE
__attribute__ ((noinline)) int SEGGER_OPEN_Erase(U32 SectorAddr, U32 SectorIndex, U32 NumSectors)
{
        int r;
        (void)SectorIndex;
        _FeedWatchdog();
        r = 0;
        do {
                r = EraseSector(SectorAddr);
                if (r) {
                        break;
                }
                SectorAddr += (1 << SECTOR_SIZE_SHIFT);
        } while (--NumSectors);
        return r;
}
#endif /* SUPPORT_SEGGER_OPEN_ERASE */

/*********************************************************************
*
*       SEGGER_OPEN_Start
*
*  Function description
*    Starts the turbo mode of flash algo.
*    Currently only available for Cortex-M based targets.
*/
#if SUPPORT_TURBO_MODE
__attribute__ ((noinline)) void SEGGER_OPEN_Start(volatile struct SEGGER_OPEN_CMD_INFO* pInfo)
{
  SEGGER_OFL_Lib_StartTurbo(&SEGGER_OFL_Api, pInfo);
}

#endif /* SUPPORT_TURBO_MODE */


/*********************************************************************
*
*       SEGGER_OPEN_CalcCRC
*
*  Function description
*    Calculates the CRC over a specified number of bytes
*    Even more optimized version of Verify() as this avoids downloading the compare data into the
*    RAMCode for comparison. Heavily reduces traffic between J-Link software and target and therefore
*    speeds up verification process significantly.
*
*  Parameters
*    CRC       CRC start value
*    Addr      Address where to start calculating CRC from
*    NumBytes  Number of bytes to calculate CRC on
*    Polynom   Polynom to be used for CRC calculation
*
*  Return value
*    CRC
*
*  Notes
*    (1) This function is optional
*    (2) Use "noinline" attribute to make sure that function is never inlined and label not
*        accidentally removed by linker from ELF file.
*/
__attribute__ ((noinline)) U32 SEGGER_OPEN_CalcCRC(U32 CRC, U32 Addr, U32 NumBytes, U32 Polynom)
{
        // Use lib function from SEGGER by default. Pass API pointer to it because it may need to
        // call the read function (non-memory mapped flashes)
        CRC = SEGGER_OFL_Lib_CalcCRC(&SEGGER_OFL_Api, CRC, Addr, NumBytes, Polynom);

        return CRC;
}
#if 0
/* WARNING: Function: __gnu_thumb1_case_uqi replaced with injection: switch8_r0 */
/* WARNING (jumptable): Removing unreachable block (ram,0x000100dc) */
/* WARNING: Removing unreachable block (ram,0x000100dc) */
#endif
#if SUPPORT_TURBO_MODE
void SEGGER_OFL_Lib_StartTurbo(const SEGGER_OFL_API *pAPI, volatile struct SEGGER_OPEN_CMD_INFO *pInfo)
{
  SEGGER_CMD command;
  uint32_t tmp;
  int result;
  int *resPtr;
  volatile struct SEGGER_OPEN_CMD_INFO *nextCmd;
  U32 Addr;
  
  do {
    command = pInfo->cmd;
    nextCmd = pInfo;
    if (command == SEGGER_CMD_IDLE)
    {
      pInfo = nextCmd;
      if (pAPI->pfFeedWatchdog != 0x0) {
        (*pAPI->pfFeedWatchdog)();
      }
    }
    resPtr = (int *)pInfo->result;
    nextCmd = (struct SEGGER_OPEN_CMD_INFO *)pInfo->nextCmdRes2;
    *resPtr = 0x7fffffff;
    pInfo->cmd = 0;
    Addr = pInfo->baseAddr + pInfo->offset;
    result = -1;
    switch(command) {
    case SEGGER_CMD_BLANCKCHECK:
      if (pAPI->pfBlankCheck != 0x0) {
        result = (*pAPI->pfBlankCheck)(Addr,pInfo->numBytes,(U8)pInfo->cmdParam1);
      }
      break;
    case SEGGER_CMD_CRC:
      tmp = SEGGER_OFL_Lib_CalcCRC(pAPI,pInfo->cmdParam1,Addr,pInfo->numBytes,pInfo->cmdParam2);
      pInfo->nextCmdRes2 = tmp;
      result = 0;
      break;
    case SEGGER_CMD_READ:
      if (pAPI->pfSEGGERRead != 0x0) {
        result = pAPI->pfSEGGERRead(Addr,pInfo->numBytes,(U8*)pInfo->cmdParam0);
      }
      break;
    case SEGGER_CMD_PROGRAM:
      if (pAPI->pfSEGGERProgram != 0x0) {
        result = pAPI->pfSEGGERProgram(Addr,pInfo->numBytes,(U8*)pInfo->cmdParam0);
      }
      break;
    case SEGGER_CMD_ERASECHIP:
      if (pInfo->numBytes == 0) {
        if (pAPI->pfEraseChip != 0x0) {
          result = pAPI->pfEraseChip();
        }
        break;
      }
      if (pAPI->pfSEGGERErase == 0x0) {
        if (pInfo->numBytes == 1) {
          result = pAPI->pfEraseSector(Addr);
        }
        break;
      }
      result = pAPI->pfSEGGERErase(Addr,pInfo->cmdParam1,pInfo->cmdParam0);
      break;
    case SEGGER_CMD_INIT:
      result = Init(Addr,pInfo->cmdParam2,pInfo->cmdParam1);
      break;
    case SEGGER_CMD_UNINIT:
      result = UnInit(pInfo->cmdParam1);
      break;
    case SEGGER_CMD_VERIFY:
      if (pAPI->pfVerify != 0x0) {
        result = pAPI->pfVerify(Addr,pInfo->numBytes,(U8*)pInfo->cmdParam0);
      }
      break;
    }
    *resPtr = result;
    pInfo = nextCmd;
    if (pAPI->pfFeedWatchdog != 0x0) {
      (*pAPI->pfFeedWatchdog)();
    }
  } while( 1 );
}
#endif
uint8_t crcTable82F63B78[] = { 0x00, 0x00, 0x00, 0x00, 0x6f, 0xc7, 0x5e, 0x10, 0xde, 0x8e, 0xbd, 0x20, 0xb1, 0x49, 0xe3, 0x30, 0xbc, 0x1d, 0x7b, 0x41, 0xd3, 0xda, 0x25, 0x51, 0x62, 0x93, 0xc6, 0x61, 0x0d, 0x54, 0x98, 0x71, 0x78, 0x3b, 0xf6, 0x82, 0x17, 0xfc, 0xa8, 0x92, 0xa6, 0xb5, 0x4b, 0xa2, 0xc9, 0x72, 0x15, 0xb2, 0xc4, 0x26, 0x8d, 0xc3, 0xab, 0xe1, 0xd3, 0xd3, 0x1a, 0xa8, 0x30, 0xe3, 0x75, 0x6f, 0x6e, 0xf3 };
uint8_t crcTableEDB88320[] = { 0x00, 0x00, 0x00, 0x00, 0x64, 0x10, 0xb7, 0x1d, 0xc8, 0x20, 0x6e, 0x3b, 0xac, 0x30, 0xd9, 0x26, 0x90, 0x41, 0xdc, 0x76, 0xf4, 0x51, 0x6b, 0x6b, 0x58, 0x61, 0xb2, 0x4d, 0x3c, 0x71, 0x05, 0x50, 0x20, 0x83, 0xb8, 0xed, 0x44, 0x93, 0x0f, 0xf0, 0xe8, 0xa3, 0xd6, 0xd6, 0x8c, 0xb3, 0x61, 0xcb, 0xb0, 0xc2, 0x64, 0x9b, 0xd4, 0xd2, 0xd3, 0x86, 0x78, 0xe2, 0x0a, 0xa0, 0x1c, 0xf2, 0xbd, 0xbd };


uint32_t SEGGER_OFL_Lib_CalcCRC(const SEGGER_OFL_API *pAPI,U32 CRC,U32 Addr,U32 NumBytes,U32 Polynom)
{
  int i;
  uint8_t *data;
  uint32_t count;
  uint8_t *dataEnd;
  uint8_t *polyTable;
  uint8_t *readAddr;
  uint8_t buf [20];
  uint8_t tmp;
  
  polyTable = crcTableEDB88320;
  readAddr = (uint8_t *)Addr;
  if ((Polynom !=0xedb88320) && (polyTable = crcTable82F63B78, Polynom !=0x82f63b78)) {
    polyTable = (uint8_t *)0x0;
  }
  do {
    count = NumBytes;
    if (0x10 < NumBytes) {
      count =0x10;
    }
    NumBytes = NumBytes - count;
    data = readAddr;
    if (pAPI->pfSEGGERRead != 0x0) {
      (*pAPI->pfSEGGERRead)((U32)readAddr,count,buf);
      data = buf;
    }
    readAddr = readAddr + count;
    dataEnd = data + count;
    if (polyTable == (uint8_t *)0x0) {
      do {
        CRC = CRC ^ *data;
        i =0x8;
        do {
          count = CRC &0x1;
          CRC = CRC >>0x1;
          if (count !=0x0) {
            CRC = CRC ^ Polynom;
          }
          i = i + -1;
        } while (i !=0x0);
        data = data +0x1;
      } while (dataEnd != data);
    }
    else {
      do {
        tmp = *data;
        data = data +0x1;
        count = (CRC ^ tmp) >>0x4 ^ *(uint32_t *)(polyTable + ((CRC ^ tmp) &0xf) *0x4);
        CRC = *(uint32_t *)(polyTable + (count &0xf) *0x4) ^ count >>0x4;
      } while (dataEnd != data);
    }
  } while (NumBytes !=0x0);
  return CRC;
}








