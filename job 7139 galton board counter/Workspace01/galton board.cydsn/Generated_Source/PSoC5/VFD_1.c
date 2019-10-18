/*******************************************************************************
* File Name: VFD_1.c
* Version 3.0
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the Voltage Fault Detector Component.
*
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VFD_1.h"
#include "VFD_1_MuxDMA_dma.h"
#include "VFD_1_FiltRdDMA_dma.h"
#include "VFD_1_FiltWrDMA_dma.h"

#if (VFD_1_ENABLE_OV_DETECT)
#include "VFD_1_OV.h"
#endif /* VFD_1_ENABLE_OV_DETECT */
#if (VFD_1_ENABLE_UV_DETECT)
#include "VFD_1_UV.h"
#endif /* VFD_1_ENABLE_UV_DETECT */


/***************************************
*   Internal definitions
***************************************/

#define AMUX_DMA_DATA_SIZE      1u
#define AMUX_DMA_DATA_NR        1u

/* Glitch filter DMA data element size depends on the Compare type parameter.
* When the parameter is set to UV, the filtering is only applied to PGOOD.
* Thus the data element size is 1 byte.
* When OV is enabled, a fault type is also distinguished. This requires
* a filtering to be additionally applied to OV fault condition. Both filters
* are combined to use 16-bit interface for DMA access. 
*/
#define FILT_DMA_DATA_SIZE      (VFD_1_ENABLE_OV_DETECT ? 2u : 1u)

/* Compiler endiannes compatibility defines */
#if (CY_PSOC3)
    #define STS_B1      3
    #define STS_B2      2
    #define STS_B3      1
    #define STS_B4      0
#else
    #define STS_B1      0
    #define STS_B2      1
    #define STS_B3      2
    #define STS_B4      3
#endif

#define NUM_STS_BYTES   4


/***************************************
*   Internal type definitions
***************************************/

typedef union
{
	uint32	word;
	uint8	bytes[NUM_STS_BYTES];
} FAULT_STS;


/***************************************
*   Internal component variables
***************************************/

static  uint8   amuxDmaCh;
static  uint8   amuxDmaTd;

static  uint8   filtRdDmaCh;
static  uint8   filtWrDmaCh;
static  uint16  filtCount[VFD_1_NUMBER_OF_VOLTAGES];


/***************************************
*    Internal component functions
***************************************/

static void VFD_1_AMuxDmaInit(void) ;
static void VFD_1_GlitchFilterDmaInit(void) ;


/***************************************
*   External variables
***************************************/

uint8 VFD_1_initVar = 0u;
const uint16 CYCODE VFD_1_VoltageScale[] = {
    1000u
};


/*******************************************************************************
* Function Name: VFD_1_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the default VFD configuration provided with the
*  customizer. Initializes internal DMA channels. It is not necessary to call
*  VFD_Init() because the VFD_Start() routine calls this function, which is the
*  preferred method to begin the component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Disables all interrupts for the moment the counter enabling bits are set.
*  Configures DMA transfer the first time this function is called. Does not
*  reinitialize DMA on subsequent calls.
*  Use VFD_1_Stop() to reset DMA to the initial state.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void VFD_1_Init(void) 
{
    uint8 interruptState;
    static uint8 dmaInitDone = 0u;

    /* DMA initialization is done the first time VFD_1_Init() is
    * called. Otherwise it will consume all available TD resources. */
    if(dmaInitDone == 0u)
    {
        VFD_1_AMuxDmaInit();
        VFD_1_GlitchFilterDmaInit();
        dmaInitDone = 1u;
    }

    #if (VFD_1_ENABLE_OV_DETECT)
        VFD_1_OVInit();
    #endif /* VFD_1_ENABLE_OV_DETECT */

    #if (VFD_1_ENABLE_UV_DETECT)
        VFD_1_UVInit();
    #endif /* VFD_1_ENABLE_UV_DETECT */

    /* Counters are hardware controlled. Set firmware enable bits at init stage */
    interruptState = CyEnterCriticalSection();
    #if (VFD_1_VIN_SETTLING_DELAY > VFD_1_VIN_SETTLING_DELAY_MIN)
        VFD_1_VIN_DELAY_CNT_ACTL_REG |= VFD_1_CNT_START;
    #endif /* VFD_1_VIN_SETTLING_DELAY > VFD_1_VIN_SETTLING_DELAY_MIN */
    VFD_1_VIN_INDEX_CNT_ACTL_REG |= VFD_1_CNT_START;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: VFD_1_Enable
********************************************************************************
*
* Summary:
*  Enables hardware blocks, the DMA channels and the control state machine.
*  It is not necessary to call VFD_1_Init() because the VFD_1_Start()
*  routine calls this function, which is the preferred method to begin the component
*  operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Sets/restores the initial TD for the AMux DMA channel, resets glitch filter
*  counters and the current voltage index. This function should not be called
*  when the component is started.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void VFD_1_Enable(void) 
{
    /* AMux control uses TD chain. Set initial TD in chain. */
    (void)CyDmaChSetInitialTd(amuxDmaCh, amuxDmaTd);
    (void)CyDmaChEnable(amuxDmaCh, 1u);
    (void)CyDmaChEnable(filtRdDmaCh, 1u);
    (void)CyDmaChEnable(filtWrDmaCh, 1u);
       
    #if (VFD_1_ENABLE_OV_DETECT)
        VFD_1_OVEnable();
    #endif /* VFD_1_ENABLE_OV_DETECT */

    #if (VFD_1_ENABLE_UV_DETECT)
        VFD_1_UVEnable();
    #endif /* VFD_1_ENABLE_UV_DETECT */

    /* Reset glitch filter counters */
    (void)memset((void *)filtCount, 0, sizeof(filtCount));

    /* Reset voltage index  */
    VFD_1_VIN_INDEX_CNT_COUNT_REG = VFD_1_NUMBER_OF_VOLTAGES - 1u;

    /* Enable the control state machine */
    VFD_1_CONTROL_REG = VFD_1_EN;
}


/*******************************************************************************
* Function Name: VFD_1_Start
********************************************************************************
*
* Summary:
*  Calls the Init() API if the component has not been initialized before.
*  Runs a calibration routine for comparators and then calls Enable() to
*  begin the component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  VFD_1_initVar - Used to check the initial configuration, modified
*  on a first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void VFD_1_Start(void) 
{
    if(VFD_1_initVar == 0u)
    {
        VFD_1_Init();
        VFD_1_initVar = 1u;
    }

    VFD_1_ComparatorCal(VFD_1_OV);
    VFD_1_ComparatorCal(VFD_1_UV);
    VFD_1_Enable();
}


/*******************************************************************************
* Function Name: VFD_1_Stop
********************************************************************************
*
* Summary:
*  Disables the VFD component. Stops DMA controller and resets TDs. Disconnects
*  AMux channels.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  pgood and pgood[x] outputs are de-asserted.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void VFD_1_Stop(void) 
{
    uint8 indx;
    uint8 interruptState;

    /* Disable control state machine */
    VFD_1_CONTROL_REG = 0u;
    
    /* Terminate all transfers and disable DMA channels */
    (void)CyDmaChSetRequest(amuxDmaCh, CY_DMA_CPU_TERM_CHAIN);
    (void)CyDmaChSetRequest(filtRdDmaCh, CY_DMA_CPU_TERM_CHAIN);
    (void)CyDmaChSetRequest(filtWrDmaCh, CY_DMA_CPU_TERM_CHAIN);
        
    /* Clear potential DRQs that may occur during termination period */    
    (void)CyDmaClearPendingDrq(amuxDmaCh);
    (void)CyDmaClearPendingDrq(filtRdDmaCh);
    (void)CyDmaClearPendingDrq(filtWrDmaCh);

    #if (VFD_1_ENABLE_OV_DETECT)
        VFD_1_OVStop();
    #endif /* VFD_1_ENABLE_OV_DETECT */

    #if (VFD_1_ENABLE_UV_DETECT)
        VFD_1_UVStop();
    #endif /* VFD_1_ENABLE_UV_DETECT */

    /* Disconnect AMux channels */
    for(indx = 0u; indx < VFD_1_NUMBER_OF_VOLTAGES; ++indx)
    {
        *VFD_1_AMux__addrTable[indx] = 0u;
    }
    /* Reset glitch filter FIFOs */
    interruptState = CyEnterCriticalSection();
    VFD_1_PG_FILTER_FIFO_ACTL_REG |= VFD_1_FIFO_CLR;
    VFD_1_PG_FILTER_FIFO_ACTL_REG &= (uint8)~VFD_1_FIFO_CLR;
    #if (VFD_1_ENABLE_OV_DETECT)
        VFD_1_OV_FILTER_FIFO_ACTL_REG |= VFD_1_FIFO_CLR;
        VFD_1_OV_FILTER_FIFO_ACTL_REG &= (uint8)~VFD_1_FIFO_CLR;
    #endif /* VFD_1_ENABLE_OV_DETECT */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: VFD_1_GetOVUVFaultStatus
********************************************************************************
*
* Summary:
*  Assigns an over/under voltage fault status of each voltage input to its
*  parameters. Bits are sticky and cleared by calling this API. Applicable only
*  if Compare Type is set to "OV/UV".
*
* Parameters:
*  ovStatus: OV status for all voltages.
*  uvStatus: UV status for all voltages.
*
* Return:
*  None.
*
* Side Effects:
*  Calling this API clears the fault condition source sticky bits. If the
*  condition still persists, then the bit will be set again after the next scan.
*
* Reentrant:
*  No.
*
*******************************************************************************/
#if (VFD_1_COMP_TYPE == VFD_1_OV_UV)
void VFD_1_GetOVUVFaultStatus(uint32 * ovStatus, uint32 * uvStatus) 
{
    FAULT_STS ovLocal;
    FAULT_STS uvLocal;

    #ifdef __CC_ARM
        /* ARMCC compiler issues C4017W warnings about the use of uninitialized
        * variables. The compiler cannot reliably identify faulty code and any
        * C4017W warnings are intended only as an indication of possible problems.
        */
        ovLocal.word = 0u;
        uvLocal.word = 0u;
    #endif /* __CC_ARM */

    #ifdef VFD_1_PG_STS8_STATUS_REG
        ovLocal.bytes[STS_B1] = VFD_1_OV_STS8_STATUS_REG;
        uvLocal.bytes[STS_B1] = (uint8)~(VFD_1_PG_STS8_STATUS_REG | ovLocal.bytes[STS_B1]);
    #endif /* VFD_1_PG_STS8_STATUS_REG */
    #ifdef VFD_1_PG_STS16_STATUS_REG
        ovLocal.bytes[STS_B2] = VFD_1_OV_STS16_STATUS_REG;
        uvLocal.bytes[STS_B2] = (uint8)~(VFD_1_PG_STS16_STATUS_REG | ovLocal.bytes[STS_B2]);
    #else
        ovLocal.bytes[STS_B2] = 0u;
        uvLocal.bytes[STS_B2] = 0u;
    #endif /* VFD_1_PG_STS16_STATUS_REG */
    #ifdef VFD_1_PG_STS24_STATUS_REG
        ovLocal.bytes[STS_B3] = VFD_1_OV_STS24_STATUS_REG;
        uvLocal.bytes[STS_B3] = (uint8)~(VFD_1_PG_STS24_STATUS_REG | ovLocal.bytes[STS_B3]);
    #else
        ovLocal.bytes[STS_B3] = 0u;
        uvLocal.bytes[STS_B3] = 0u;
    #endif /* VFD_1_PG_STS24_STATUS_REG */
    #ifdef VFD_1_PG_STS32_STATUS_REG
        ovLocal.bytes[STS_B4] = VFD_1_OV_STS32_STATUS_REG;
        uvLocal.bytes[STS_B4] = (uint8)~(VFD_1_PG_STS32_STATUS_REG | ovLocal.bytes[STS_B4]);
    #else
        ovLocal.bytes[STS_B4] = 0u;
        uvLocal.bytes[STS_B4] = 0u;
    #endif /* VFD_1_PG_STS32_STATUS_REG */

    *ovStatus = ovLocal.word;
    *uvStatus = uvLocal.word;
}

#elif (VFD_1_COMP_TYPE == VFD_1_OV)

/*******************************************************************************
* Function Name: VFD_1_GetOVFaultStatus
********************************************************************************
*
* Summary:
*  Returns the OV status detected for all voltages.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void VFD_1_GetOVFaultStatus(uint32 * status) 
{
    FAULT_STS CYDATA stsLocal;

    #ifdef __CC_ARM
        /* ARMCC compiler issues C4017W warnings about the use of uninitialized
        * variables. The compiler cannot reliably identify faulty code and any
        * C4017W warnings are intended only as an indication of possible problems.
        */
        stsLocal.word = 0u;
    #endif /* __CC_ARM */

    #ifdef VFD_1_OV_STS8_STATUS_REG
        stsLocal.bytes[STS_B1] = VFD_1_OV_STS8_STATUS_REG;
    #endif /* VFD_1_OV_STS8_STATUS_REG */
    #ifdef VFD_1_OV_STS16_STATUS_REG
        stsLocal.bytes[STS_B2] = VFD_1_OV_STS16_STATUS_REG;
    #else
        stsLocal.bytes[STS_B2] = 0u;
    #endif /* VFD_1_OV_STS16_STATUS_REG */
    #ifdef VFD_1_OV_STS24_STATUS_REG
        stsLocal.bytes[STS_B3] = VFD_1_OV_STS24_STATUS_REG;
    #else
        stsLocal.bytes[STS_B3] = 0u;
    #endif /* VFD_1_OV_STS24_STATUS_REG */
    #ifdef VFD_1_OV_STS32_STATUS_REG
        stsLocal.bytes[STS_B4] = VFD_1_OV_STS32_STATUS_REG;
    #else
        stsLocal.bytes[STS_B4] = 0u;
    #endif /* VFD_1_OV_STS32_STATUS_REG */

    *status = stsLocal.word;
}

#else 

/*******************************************************************************
* Function Name: VFD_1_GetUVFaultStatus
********************************************************************************
*
* Summary:
*  Returns the UV status detected for all voltages.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void VFD_1_GetUVFaultStatus(uint32 * status) 
{
    FAULT_STS CYDATA stsLocal;

    #ifdef __CC_ARM
        /* ARMCC compiler issues C4017W warnings about the use of uninitialized
        * variables. The compiler cannot reliably identify faulty code and any
        * C4017W warnings are intended only as an indication of possible problems.
        */
        stsLocal.word = 0u;
    #endif /* __CC_ARM */

    #ifdef VFD_1_PG_STS8_STATUS_REG
        stsLocal.bytes[STS_B1] = (uint8)~VFD_1_PG_STS8_STATUS_REG;
    #endif /* VFD_1_PG_STS8_STATUS_REG */
    #ifdef VFD_1_PG_STS16_STATUS_REG
        stsLocal.bytes[STS_B2] = (uint8)~VFD_1_PG_STS16_STATUS_REG;
    #else
        stsLocal.bytes[STS_B2] = 0u;
    #endif /* VFD_1_PG_STS16_STATUS_REG */
    #ifdef VFD_1_PG_STS24_STATUS_REG
        stsLocal.bytes[STS_B3] = (uint8)~VFD_1_PG_STS24_STATUS_REG;
    #else
        stsLocal.bytes[STS_B3] = 0u;
    #endif /* VFD_1_PG_STS24_STATUS_REG */
    #ifdef VFD_1_PG_STS32_STATUS_REG
        stsLocal.bytes[STS_B4] = (uint8)~VFD_1_PG_STS32_STATUS_REG;
    #else
        stsLocal.bytes[STS_B4] = 0u;
    #endif /* VFD_1_PG_STS32_STATUS_REG */

    *status = stsLocal.word;
}

#endif /* (VFD_1_COMP_TYPE == VFD_1_OV_UV) */


/*******************************************************************************
* Function Name: VFD_1_ComparatorCal
********************************************************************************
*
* Summary:
*  Runs a calibration routine that measures the selected comparator's offset 
*  voltage by shorting its inputs together and corrects for it by writing to the
*  CMP block's trim register.
*
* Parameters:
*  enum compType
*    Valid values: VFD_1_OV, VFD_1_UV.
*
* Return:
*  None.
*
* Side Effects:
*  Calling this API may cause the comparator to trigger a fault condition. To 
*  prevent this, call the VFD_Pause() API prior to calling this API.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void VFD_1_ComparatorCal(uint8 compType) 
{    
    #if (VFD_1_ENABLE_OV_DETECT)
        if(compType == VFD_1_OV)
        {
            (void) VFD_1_Comp_OV_ZeroCal();
        }
    #endif /*VFD_1_ENABLE_OV_DETECT */

    #if (VFD_1_ENABLE_UV_DETECT)
        if(compType == VFD_1_UV)
        {
            (void) VFD_1_Comp_UV_ZeroCal();
        }
    #endif /* VFD_1_ENABLE_UV_DETECT */
}


/*******************************************************************************
* Function Name: VFD_1_SetSpeed
********************************************************************************
*
* Summary:
*  Sets the speed mode for the Voltage DACs and Comparators.
*
* Parameters:
*  speedMode: specifies the speed mode. Valid values are VFD_1_LOW_SPEED
*             and VFD_1_HIGH_SPEED.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void VFD_1_SetSpeed(uint8 speedMode) 
{
    if(speedMode == VFD_1_LOW_SPEED)
    {
        #if (VFD_1_ENABLE_OV_DETECT)
            VFD_1_SetOVSpeed(LOWSPEED);
        #endif /* VFD_1_ENABLE_OV_DETECT */

        #if (VFD_1_ENABLE_UV_DETECT)
            VFD_1_SetUVSpeed(LOWSPEED);
        #endif /* VFD_1_ENABLE_UV_DETECT */
    }
    else
    {
        #if (VFD_1_ENABLE_OV_DETECT)
            VFD_1_SetOVSpeed(HIGHSPEED);
        #endif /* VFD_1_ENABLE_OV_DETECT */

        #if (VFD_1_ENABLE_UV_DETECT)
            VFD_1_SetUVSpeed(HIGHSPEED);
        #endif /* VFD_1_ENABLE_UV_DETECT */
    }
}

    
/*******************************************************************************
* Function Name: VFD_1_AMuxDmaInit
********************************************************************************
*
* Summary:
*  Initializes the DMA controlled analog multiplexer. On receiving a trigger,
*  the AMUX DMA channel connects the next analog input voltage to the window
*  comparator. The DMA controller disconnects the previous input before
*  connecting the next input to ensure a break-before-make connection.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
static void VFD_1_AMuxDmaInit(void) 
{      
    /* Chained TD pair is used to break previous AMux connection before
    * making new connection, i.e. td0(break)->td1(make).
    */
    uint8 td0;
    uint8 td1;
    uint8 td;

    /* 0x00 is written into AMux port to break connection */
    static uint8 amuxClear = 0u;

    /* Copy AMux mask table to SRAM to improve DMA transfer performance */
    static uint8 amuxMaskTable[VFD_1_NUMBER_OF_VOLTAGES];
    (void)memcpy((void *)amuxMaskTable, (const void *)VFD_1_AMux__maskTable, VFD_1_NUMBER_OF_VOLTAGES);

    amuxDmaCh = VFD_1_DMA_CFG(MuxDMA, AMUX_DMA_DATA_SIZE, HI16(CYDEV_SRAM_BASE), HI16(CYDEV_PERIPH_BASE));

    td1 = CY_DMA_INVALID_TD;
    for(td = 0u; td < VFD_1_NUMBER_OF_VOLTAGES; ++td)
    {
        /* TD0: Move 0 into channel being disconnected (break) */
        td0 = CyDmaTdAllocate();
        (void)CyDmaTdSetAddress(td0, LO16((uint32)&amuxClear), LO16((uint32)VFD_1_AMux__addrTable[td]));
        (void)CyDmaTdSetConfiguration(td0,  AMUX_DMA_DATA_NR, td1, CY_DMA_TD_AUTO_EXEC_NEXT);
        
        /* TD1: Move proper AMux mask into channel being connected (make) */
        td1 = CyDmaTdAllocate();
        (void)CyDmaTdSetAddress(td1, LO16((uint32)&amuxMaskTable[td]), LO16((uint32)VFD_1_AMux__addrTable[td]));
        (void)CyDmaTdSetConfiguration(td1, AMUX_DMA_DATA_NR, td0, VFD_1_MuxDMA__TD_TERMOUT_EN);
        
        /* Store initial TD in chain */
        if(td == 0u)
        {
            amuxDmaTd = td0;
        }
    }

    /* The initial TD is configured to join this TD chain into a ring.
    * The TD execution order is as follows.
    * break[0]->set[N-1]->break[N-1]->set[N-2]->...->break[1]->set[0]
    * This matches the control logic that uses a down counter to track the
    * voltage number being scanned. 
    */
    (void)CyDmaTdSetConfiguration(amuxDmaTd,  AMUX_DMA_DATA_NR, td1, CY_DMA_TD_AUTO_EXEC_NEXT);
}


/***************************************************************************
* Function Name: VFD_1_GlitchFilterDmaInit
****************************************************************************
*
* Summary:
*  Initializes the glitch filter. The glitch filter outputs a 1 only when the 
*  current and previous N samples are 1, and a 0 only when the current and
*  previous N samples are 0. Otherwise the output is unchanged from its
*  current value. The glitch filtering process uses a DMA controller to read
*  the previous filter count for the current voltage from SRAM and write the
*  updated filter count back to SRAM after the voltage is processed.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
***************************************************************************/
static void VFD_1_GlitchFilterDmaInit(void) 
{
    uint8 td;

    /* Reads previous glitch filter count for current input from SRAM */
    filtRdDmaCh = VFD_1_DMA_CFG(FiltRdDMA, FILT_DMA_DATA_SIZE, HI16(CYDEV_SRAM_BASE), HI16(CYDEV_PERIPH_BASE));
    td = CyDmaTdAllocate();
    (void)CyDmaTdSetConfiguration(td, sizeof(filtCount), td, CY_DMA_TD_INC_SRC_ADR);
    (void)CyDmaTdSetAddress(td, LO16((uint32)filtCount), LO16((uint32)VFD_1_FILTER_IN_FIFO_PTR));
    (void)CyDmaChSetInitialTd(filtRdDmaCh, td);

    /* Moves updated glitch filter result back to SRAM */ 
    filtWrDmaCh = VFD_1_DMA_CFG(FiltWrDMA, FILT_DMA_DATA_SIZE, HI16(CYDEV_PERIPH_BASE), HI16(CYDEV_SRAM_BASE));
    td = CyDmaTdAllocate();
    (void)CyDmaTdSetConfiguration(td, sizeof(filtCount), td, CY_DMA_TD_INC_DST_ADR);
    (void)CyDmaTdSetAddress(td, LO16((uint32)VFD_1_FILTER_OUT_FIFO_PTR), LO16((uint32)filtCount));
    (void)CyDmaChSetInitialTd(filtWrDmaCh, td);
}

/* [] END OF FILE */
