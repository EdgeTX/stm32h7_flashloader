/*********************************************************************
*               (c) SEGGER Microcontroller GmbH & Co. KG             *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************
*/

#include <stdint.h>
#include <stddef.h>

typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t U8;

struct SEGGER_OPEN_CMD_INFO {
    uint32_t cmdParam0;
    uint32_t u1;
    uint32_t baseAddr;
    uint32_t offset;
    uint32_t numBytes;
    uint32_t cmdParam1;
    uint32_t cmdParam2;
    uint32_t cmdParam3;
    uint32_t result;
    uint32_t nextCmdRes2;
    uint32_t u2;
    uint32_t cmd;
};

typedef struct  {
  // Optional functions may be NULL
  //
  void (*pfFeedWatchdog)   (void);                                            // Optional
  int  (*pfInit)           (U32 Addr, U32 Freq, U32 Func);                    // Mandatory
  int  (*pfUnInit)         (U32 Func);                                        // Mandatory
  int  (*pfEraseSector)    (U32 Addr);                                        // Mandatory
  int  (*pfProgramPage)    (U32 Addr, U32 NumBytes, U8 *pSrcBuff);            // Mandatory
  int  (*pfBlankCheck)     (U32 Addr, U32 NumBytes, U8 BlankData);            // Optional
  int  (*pfEraseChip)      (void);                                            // Optional
  U32  (*pfVerify)         (U32 Addr, U32 NumBytes, U8 *pSrcBuff);            // Optional
  U32  (*pfSEGGERCalcCRC)  (U32 CRC, U32 Addr, U32 NumBytes, U32 Polynom);    // Optional
  int  (*pfSEGGERRead)     (U32 Addr, U32 NumBytes, U8 *pDestBuff);           // Optional
  int  (*pfSEGGERProgram)  (U32 DestAddr, U32 NumBytes, U8 *pSrcBuff);        // Optional
  int  (*pfSEGGERErase)    (U32 SectorAddr, U32 SectorIndex, U32 NumSectors); // Optional
  void (*pfSEGGERStart)    (volatile struct SEGGER_OPEN_CMD_INFO* pInfo);     // Optional
} SEGGER_OFL_API;


typedef enum SEGGER_CMD {
    SEGGER_CMD_IDLE=0,
    SEGGER_CMD_BLANCKCHECK=2,
    SEGGER_CMD_CRC=3,
    SEGGER_CMD_READ=5,
    SEGGER_CMD_PROGRAM=6,
    SEGGER_CMD_ERASECHIP=7,
    SEGGER_CMD_INIT=8,
    SEGGER_CMD_UNINIT=9,
    SEGGER_CMD_VERIFY=11
} SEGGER_CMD;


#define ONCHIP     (1)             // On-chip Flash Memory

#define MAX_NUM_SECTORS (512)      // Max. number of sectors, must not be modified.
#define ALGO_VERSION    (0x0101)   // Algo version, must not be modified.

struct SECTOR_INFO  {
  U32 SectorSize;       // Sector Size in bytes
  U32 SectorStartAddr;  // Start address of the sector area (relative to the "BaseAddr" of the flash)
};

struct FlashDevice  {
   U16 AlgoVer;       // Algo version number
   U8  Name[128];     // Flash device name
   U16 Type;          // Flash device type
   U32 BaseAddr;      // Flash base address
   U32 TotalSize;     // Total flash device size in Bytes (256 KB)
   U32 PageSize;      // Page Size (number of bytes that will be passed to ProgramPage(). MinAlig is 8 byte
   U32 Reserved;      // Reserved, should be 0
   U8  ErasedVal;     // Flash erased value
   U32 TimeoutProg;   // Program page timeout in ms
   U32 TimeoutErase;  // Erase sector timeout in ms
   struct SECTOR_INFO SectorInfo[MAX_NUM_SECTORS]; // Flash sector layout definition
};

__attribute__ ((noinline)) int  Init        (U32 Addr, U32 Freq, U32 Func);                    // Mandatory
__attribute__ ((noinline)) int  UnInit      (U32 Func);                                        // Mandatory
__attribute__ ((noinline)) int  EraseSector (U32 Addr);                                        // Mandatory
__attribute__ ((noinline)) int  ProgramPage (U32 Addr, U32 NumBytes, U8 *pSrcBuff);            // Mandatory
__attribute__ ((noinline)) int  BlankCheck  (U32 Addr, U32 NumBytes, U8 BlankData);            // Optional
__attribute__ ((noinline)) int  EraseChip   (void);                                            // Optional
__attribute__ ((noinline)) U32  Verify      (U32 Addr, U32 NumBytes, U8 *pSrcBuff);            // Optional
//
// SEGGER extensions
//
__attribute__ ((noinline)) U32  SEGGER_OPEN_CalcCRC  (U32 CRC, U32 Addr, U32 NumBytes, U32 Polynom);    // Optional
__attribute__ ((noinline)) int  SEGGER_OPEN_Read     (U32 Addr, U32 NumBytes, U8 *pDestBuff);           // Optional
__attribute__ ((noinline)) int  SEGGER_OPEN_Program  (U32 DestAddr, U32 NumBytes, U8 *pSrcBuff);        // Optional
__attribute__ ((noinline)) int  SEGGER_OPEN_Erase    (U32 SectorAddr, U32 SectorIndex, U32 NumSectors); // Optional
__attribute__ ((noinline)) void SEGGER_OPEN_Start    (volatile struct SEGGER_OPEN_CMD_INFO* pInfo);     // Optional

__attribute__ ((noinline)) void SEGGER_OFL_Lib_StartTurbo(const SEGGER_OFL_API *pAPI, volatile struct SEGGER_OPEN_CMD_INFO *pInfo);
__attribute__ ((noinline)) uint32_t SEGGER_OFL_Lib_CalcCRC(const SEGGER_OFL_API *pAPI,U32 CRC,U32 Addr,U32 NumBytes,U32 Polynom);
