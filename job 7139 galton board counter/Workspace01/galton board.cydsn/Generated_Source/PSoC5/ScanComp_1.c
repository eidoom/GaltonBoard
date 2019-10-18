/*******************************************************************************
* File Name: ScanComp_1.c
* Version 1.10
*
* Description:
*  This file provides the source code to the API for the Scanning Comparator
*  component
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ScanComp_1.h"

#if (ScanComp_1_INTERNAL_REF_IS_USED)

    static uint8  ScanComp_1_compDACRange;
    
    #if (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED)
        
        /* Variable declarations for DMA */
        static uint8 ScanComp_1_DMA_VDACUpdate_Chan;
        static uint8 ScanComp_1_DMA_VDACUpdate_TD = CY_DMA_INVALID_TD;
        
        static void ScanComp_1_InitDma(void)  ;

        /* Array from customizer */
        static uint8 ScanComp_1_DacVoltages[ScanComp_1_NUM_OF_CHANNELS] = {
    0xFAu, 
};

    #else
        static uint8  ScanComp_1_compDACVoltage;
    #endif /* ScanComp_1_VDAC_PER_CHANNEL_MODE_USED */
#endif /* ScanComp_1_INTERNAL_REF_IS_USED */

#if (ScanComp_1_NUM_OF_CHANNELS >= 8u)
    static uint8 storeStatusReg;
#endif /* ScanComp_1_NUM_OF_CHANNELS >= 8u */

uint8  ScanComp_1_initVar = 0u;

static reg8 * const CYCODE interruptStatusMaskRegs[ScanComp_1_NUM_OF_INT_REGISTERS] =
{
    ScanComp_1_STATUS_INT0_MASK_PTR,

    #if(ScanComp_1_NUM_OF_CHANNELS > 8u)
        ScanComp_1_STATUS_INT1_MASK_PTR,
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 8u */

    #if(ScanComp_1_NUM_OF_CHANNELS > 16u)
        ScanComp_1_STATUS_INT2_MASK_PTR,
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 16u */

    #if(ScanComp_1_NUM_OF_CHANNELS > 24u)
        ScanComp_1_STATUS_INT3_MASK_PTR,
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 24u */

    #if(ScanComp_1_NUM_OF_CHANNELS > 32u)
        ScanComp_1_STATUS_INT4_MASK_PTR,
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 32u */

    #if(ScanComp_1_NUM_OF_CHANNELS > 40u)
        ScanComp_1_STATUS_INT5_MASK_PTR,
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 40u */

    #if(ScanComp_1_NUM_OF_CHANNELS > 48u)
        ScanComp_1_STATUS_INT6_MASK_PTR,
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 48u */

    #if(ScanComp_1_NUM_OF_CHANNELS > 56u)
        ScanComp_1_STATUS_INT7_MASK_PTR,
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 56u */
};


/*******************************************************************************
* Function Name: ScanComp_1_Start
********************************************************************************
*
* Summary:
*  Performs all of the required initialization for the ScanComp component
*  and enables power to the block.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ScanComp_1_Start(void) 
{
    if(0u == ScanComp_1_initVar)
    {
        ScanComp_1_Init();
        ScanComp_1_initVar = 1u;
    }
    ScanComp_1_Enable();
}


/*******************************************************************************
* Function Name: ScanComp_1_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the ScanComp component according to
*  the customizer settings.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ScanComp_1_Init(void) 
{
    #if (ScanComp_1_NUM_OF_CHANNELS >= 8u)
        storeStatusReg = 0u;
    #endif /* ScanComp_1_NUM_OF_CHANNELS >= 8u */

    ScanComp_1_Comp_Init();    
    
    #if (ScanComp_1_INTERNAL_REF_IS_USED)
        ScanComp_1_VDAC_Init();
        ScanComp_1_compDACRange = ScanComp_1_DEFAULT_RANGE;

        #if (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED)
            ScanComp_1_VDAC_SetValue(0x0u);
            
            if(CY_DMA_INVALID_TD == ScanComp_1_DMA_VDACUpdate_TD)
            {
                ScanComp_1_InitDma();
            }            
        #else
            ScanComp_1_SetDACVoltage(ScanComp_1_DEFAULT_VOLTAGE);
        #endif /* ScanComp_1_VDAC_PER_CHANNEL_MODE_USED */
    #endif /* ScanComp_1_INTERNAL_REF_IS_USED */
}


/*******************************************************************************
* Function Name: ScanComp_1_Stop
********************************************************************************
*
* Summary:
*  Turns off the Scanning Comparator by turning off the comparator itself and 
*  halting the muxing of inputs, and turning off the DAC if it is used.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ScanComp_1_Stop(void) 
{
    ScanComp_1_CONTROL_REG = ScanComp_1_CONTROL_DISABLE;
    
    ScanComp_1_Comp_Stop();    
    
    #if (ScanComp_1_INTERNAL_REF_IS_USED)
        ScanComp_1_VDAC_Stop();

        #if (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED)
            (void) CyDmaChDisable(ScanComp_1_DMA_VDACUpdate_Chan);
        #endif /* ScanComp_1_VDAC_PER_CHANNEL_MODE_USED */
    #endif /* ScanComp_1_INTERNAL_REF_IS_USED */
}


/*******************************************************************************
* Function Name: ScanComp_1_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ScanComp_1_Enable(void) 
{
    uint8 interruptState;

    ScanComp_1_Comp_Enable();
    
    ScanComp_1_COUNTER_COUNT_REG  = ScanComp_1_DEFAULT_CHANNELS_VAL;

    #if (ScanComp_1_INTERNAL_REF_IS_USED)
        ScanComp_1_VDAC_Enable();
        #if (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED)        
            (void)CyDmaClearPendingDrq(ScanComp_1_DMA_VDACUpdate_Chan);
            (void)CyDmaChEnable(ScanComp_1_DMA_VDACUpdate_Chan, 1u);
        #endif /* (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED) */    
    #endif /* ScanComp_1_INTERNAL_REF_IS_USED */    

    /* Enable Status Int */
    interruptState = CyEnterCriticalSection();

    #if (ScanComp_1_NUM_OF_CHANNELS < 8u)
        ScanComp_1_STATUS_INT0_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS < 8u */

    #if (ScanComp_1_NUM_OF_CHANNELS >= 8u)
        ScanComp_1_STATUS_INT8_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS >= 8u */

    #if (ScanComp_1_NUM_OF_CHANNELS >= 9u)
        ScanComp_1_STATUS_INT1_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS >= 9u */

    #if (ScanComp_1_NUM_OF_CHANNELS > 16u)
        ScanComp_1_STATUS_INT2_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 16u */

    #if (ScanComp_1_NUM_OF_CHANNELS > 24u)
        ScanComp_1_STATUS_INT3_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 24u */

    #if (ScanComp_1_NUM_OF_CHANNELS > 32u)
        ScanComp_1_STATUS_INT4_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 32u */

    #if (ScanComp_1_NUM_OF_CHANNELS > 40u)
        ScanComp_1_STATUS_INT5_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 40u */

    #if (ScanComp_1_NUM_OF_CHANNELS > 48u)
        ScanComp_1_STATUS_INT6_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 48u */

    #if (ScanComp_1_NUM_OF_CHANNELS > 56u)
        ScanComp_1_STATUS_INT7_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 56u */

    #if (ScanComp_1_NUM_OF_CHANNELS == 64)
        ScanComp_1_STATUS_INT9_AUX_CONTROL |= ScanComp_1_STATUS_INTR_ENBL;
    #endif /* ScanComp_1_NUM_OF_CHANNELS == 64u */

    ScanComp_1_COUNTER_AUX_CONTROL_REG |= ScanComp_1_COUNTER_ENABLE;

    CyExitCriticalSection(interruptState);

    ScanComp_1_CONTROL_REG = ScanComp_1_CONTROL_ENABLE ;
}


/*******************************************************************************
* Function Name: ScanComp_1_SetSpeed
********************************************************************************
*
* Summary:
*  Sets the drive power and speed to one of three settings.
*
* Parameters:
*  speed: enumerated speed mode value.
*   ScanComp_1_SLOWSPEED  - Slow speed / Ultra low power
*   ScanComp_1_MEDSPEED   - Medium speed / Medium power
*   ScanComp_1_HIGHSPEED  - High speed / High power
*
* Return:
*  None
*
*******************************************************************************/
void ScanComp_1_SetSpeed(uint8 speed) 
{
    #if (CY_PSOC4)
        ScanComp_1_Comp_SetSpeed((uint32)speed);
    #else
        ScanComp_1_Comp_SetSpeed(speed);
    #endif /* CY_PSOC4 */
}


/*******************************************************************************
* Function Name: ScanComp_1_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the pending interrupt requests from the selected block.
*  Even masked interrupts are returned.
*
* Parameters:
*  InputBlock: Specifies the block of 8 channels whose interrupt requests
*  should be returned.
*
* Return:
*  Bit field of interrupt sources from the selected block of 8 inputs.
*
*******************************************************************************/
uint8 ScanComp_1_GetInterruptSource(uint8 inputBlock) 
{
    uint8 intSrc = 0u;

    #if (ScanComp_1_NUM_OF_CHANNELS >= 8u)
        uint8 intMsb = 0u;
    #endif /* ScanComp_1_NUM_OF_CHANNELS >= 8u */

    static reg8 * const CYCODE interruptStatusRegs[ScanComp_1_NUM_OF_INT_REGISTERS] =
    {
        ScanComp_1_STATUS_INT0_PTR,

        #if(ScanComp_1_NUM_OF_CHANNELS > 8u)
            ScanComp_1_STATUS_INT1_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 8u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 16u)
            ScanComp_1_STATUS_INT2_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 16u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 24u)
            ScanComp_1_STATUS_INT3_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 24u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 32u)
            ScanComp_1_STATUS_INT4_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 32u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 40u)
            ScanComp_1_STATUS_INT5_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 40u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 48u)
            ScanComp_1_STATUS_INT6_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 48u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 56u)
            ScanComp_1_STATUS_INT7_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 56u */
    };    
    
    if (inputBlock < ScanComp_1_NUM_OF_INT_REGISTERS)
    {
        intSrc = ((uint8) (* interruptStatusRegs[inputBlock])) & ScanComp_1_INT_REG_MASK;
    }
    
    #if (ScanComp_1_NUM_OF_CHANNELS > 63u)
        if (inputBlock == 7u)
        {
            intMsb = (uint8)(ScanComp_1_STATUS_INT9_REG  << ScanComp_1_7BIT_SHIFT);
        }
        else
        {
            storeStatusReg |= ScanComp_1_STATUS_INT8_REG;
            intMsb = (uint8)((storeStatusReg >> inputBlock) << ScanComp_1_7BIT_SHIFT);
            storeStatusReg = (storeStatusReg & (uint8)(~(uint8)(1u << inputBlock)));
        }
        intSrc |= intMsb;
    #elif (ScanComp_1_NUM_OF_CHANNELS >= 8u)
        if (inputBlock != 7u)
        {
            storeStatusReg |= ScanComp_1_STATUS_INT8_REG;
            intMsb = (uint8)((storeStatusReg >> inputBlock) << ScanComp_1_7BIT_SHIFT);
            storeStatusReg = (storeStatusReg & (uint8)(~(uint8)(1u << inputBlock)));
        }
        intSrc |= intMsb;
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 64u */

    return (intSrc);
}


/*******************************************************************************
* Function Name: ScanComp_1_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the pending interrupt requests from the selected block.
*  Masked interrupts are not returned.
*
* Parameters:
*  InputBlock: Specifies the block of 8 channels whose interrupt requests
*  should be returned.
*
* Return:
*  Bit field of interrupt sources from the selected block of 8 inputs.
*
*******************************************************************************/
uint8 ScanComp_1_GetInterruptSourceMasked(uint8 inputBlock)   \
                                                
{
    return (ScanComp_1_GetInterruptSource(inputBlock) & (uint8)~ScanComp_1_GetInterruptMask (inputBlock));
}


/*******************************************************************************
* Function Name: ScanComp_1_GetInterruptMask
********************************************************************************
*
* Summary:
*  Gets the current interrupt mask for the selected block.
*
* Parameters:
*  InputBlock: Specifies the block of 8 channels whose interrupt requests
*  should be returned.
*
* Return:
*  The interrupt mask for the given block of channels.
*  A '1' indicates that the interrupt is enabled, a '0' indicates that it 
*  is masked off.
*
*******************************************************************************/
uint8 ScanComp_1_GetInterruptMask(uint8 inputBlock) 
{
    #if (ScanComp_1_NUM_OF_CHANNELS >= 8u)
        uint8 maskMsb = 0u;
    #endif /* ScanComp_1_NUM_OF_CHANNELS >= 8u */

    uint8 intSrc = 0u;

    if (inputBlock < ScanComp_1_NUM_OF_INT_REGISTERS)
    {
        intSrc = (uint8) (* interruptStatusMaskRegs[inputBlock]);
    }    

    #if (ScanComp_1_NUM_OF_CHANNELS > 63u)
        if (inputBlock == 7u)
        {
            maskMsb = (uint8)(ScanComp_1_STATUS_INT9_MASK  << ScanComp_1_7BIT_SHIFT);
        }
        else
        {
            maskMsb = (uint8)((ScanComp_1_STATUS_INT8_MASK >> inputBlock) << ScanComp_1_7BIT_SHIFT);
        }
        intSrc |= maskMsb;
    #elif (ScanComp_1_NUM_OF_CHANNELS >= 8u)
        if (inputBlock != 7u)
        {
            maskMsb = (uint8)((ScanComp_1_STATUS_INT8_MASK >> inputBlock) << ScanComp_1_7BIT_SHIFT);
        }
        intSrc |= maskMsb;
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 63u */

    return ((uint8)intSrc);
}


/*******************************************************************************
* Function Name: ScanComp_1_SetInterruptMask
********************************************************************************
*
* Summary:
*  Sets the interrupt masks for the set block of 8 channels.
*
* Parameters:
*  InputBlock: specifies the block of 8 channels whose interrupt requests
*  should be returned.
*  mask: Interrupt mask value for the specified block of 8 or less channels. 
*  A '1' indicates that the interrupt is enabled, a '0' indicates that it 
*  is masked off.
*
* Return:
*  None
*
*******************************************************************************/
void ScanComp_1_SetInterruptMask(uint8 inputBlock, uint8 mask)    \
                                       
{
    uint8 maskLsb = mask & ScanComp_1_7BIT_MASK;

    #if (ScanComp_1_NUM_OF_CHANNELS >= 8u)
        uint8 maskMsb = (uint8)((mask >> ScanComp_1_7BIT_SHIFT) << inputBlock);
    #endif /* ScanComp_1_NUM_OF_CHANNELS >= 8u */
    
    if (inputBlock < ScanComp_1_NUM_OF_INT_REGISTERS)
    {
        (* interruptStatusMaskRegs[inputBlock]) = maskLsb;
    }    

    #if (ScanComp_1_NUM_OF_CHANNELS > 63u)
        if (inputBlock == 7u)
        {
            ScanComp_1_STATUS_INT9_MASK = maskMsb;
        }
        else
        {
            ScanComp_1_STATUS_INT8_MASK = maskMsb;
        }

    #elif (ScanComp_1_NUM_OF_CHANNELS >= 8u)
        if (inputBlock != 7u)
        {
            ScanComp_1_STATUS_INT8_MASK = maskMsb;
        }
    #endif /* ScanComp_1_NUM_OF_CHANNELS > 63u */
}


/*******************************************************************************
* Function Name: ScanComp_1_GetCompare
********************************************************************************
*
* Summary:
*  Returns a nonzero value when the voltage connected to the positive input
*  is greater than the negative input voltage.
*
* Parameters:
*  channel: channel whose compare output is to be read.
*
* Return:
*  Comparator output state
*
*******************************************************************************/
uint8 ScanComp_1_GetCompare(uint8 channel) 
{
    uint8 compVal;
    uint8 chanPos;
    uint8 blockNum;

    static reg8 * const CYCODE outputStatusRegs[ScanComp_1_NUM_OF_OUT_REGISTERS] =
    {
        ScanComp_1_STATUS_OUT0_PTR,

        #if(ScanComp_1_NUM_OF_CHANNELS > 8u)
            ScanComp_1_STATUS_OUT1_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 8u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 16u)
            ScanComp_1_STATUS_OUT2_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 16u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 24u)
            ScanComp_1_STATUS_OUT3_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 24u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 32u)
            ScanComp_1_STATUS_OUT4_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 32u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 40u)
            ScanComp_1_STATUS_OUT5_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 40u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 48u)
            ScanComp_1_STATUS_OUT6_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 48u */

        #if(ScanComp_1_NUM_OF_CHANNELS > 56u)
            ScanComp_1_STATUS_OUT7_PTR,
        #endif /* ScanComp_1_NUM_OF_CHANNELS > 56u */
    };

    /* Identify block number by dividing by 8 */
    blockNum = (channel >> 3u);

    /* Identify maximum available channels for block */
    chanPos = (uint8)((blockNum + 1u) << 3u);
    
    /* Identify channel position in the register */
    chanPos = ScanComp_1_MSB_REG_BIT_HIGH >> ((chanPos - 1u) - channel);
    
    /* Input block int register & MSB bit shift */
    compVal = ((uint8) * outputStatusRegs[blockNum]) & chanPos;

    return (compVal);
}

#if (ScanComp_1_INTERNAL_REF_IS_USED)
    /*******************************************************************************
    * Function Name: ScanComp_1_SetDACRange
    ********************************************************************************
    *
    * Summary:
    *  Sets the DAC to a new range.
    *
    * Parameters:
    *  uint8 DACRange: new range to be written to the DAC:
    *   ScanComp_1_SCANCOMP_DACRANGE_1V
    *   ScanComp_1_SCANCOMP_DACRANGE_4V
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void ScanComp_1_SetDACRange(uint8 DACRange) 
    {
        ScanComp_1_VDAC_SetRange(DACRange);
        ScanComp_1_compDACRange = DACRange;
    }


    /*******************************************************************************
    * Function Name: ScanComp_1_GetDACRange
    ********************************************************************************
    *
    * Summary:
    *  Gets the DAC range setting..
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8 DACrange:
    *   ScanComp_1_SCANCOMP_DACRANGE_1V
    *   ScanComp_1_SCANCOMP_DACRANGE_4V
    *
    *******************************************************************************/

    uint8 ScanComp_1_GetDACRange(void) 
    {
        return (ScanComp_1_compDACRange);
    }


    #if (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED)

        /*******************************************************************************
        * Function Name: ScanComp_1_SetChannelDACVoltage
        ********************************************************************************
        *
        * Summary:
        *  Sets the DAC output for a specific channel to a new voltage.
        *
        * Parameters:
        *  Channel: Specifies the channel whose DAC voltage should be changed. 0
        *           indexed.
        *  DACVoltage: Voltage to be written to the DAC, in units depending on the
        *              selected range (4 or 16 mV per bit in 1 or 4V range,
        *              respectively). Should only be used when the DAC is enabled and
        *              voltage is per channel.
        *
        * Return:
        *  void
        *
        *******************************************************************************/
        void ScanComp_1_SetChannelDACVoltage(uint8 channel, uint8 DACVoltage) \
                                                   
        {
            if (channel < ScanComp_1_NUM_OF_CHANNELS)
            {
                ScanComp_1_DacVoltages[(ScanComp_1_DEFAULT_CHANNELS_VAL - channel)] = DACVoltage;
            }
        }


        /*******************************************************************************
        * Function Name: ScanComp_1_GetChannelDACVoltage
        ********************************************************************************
        *
        * Summary:
        *  Gets the DAC output voltage for a specific channel.
        *
        * Parameters:
        *  Channel: Specifies the channel whose DAC voltage should be returned.
        *
        * Return:
        *  Voltage the DAC is configured for, for the given channel, in units
        *  depending on the selected range (4 or 16 mV per bit in 1 or 4V range,
        *  respectively).
        *
        *******************************************************************************/
        uint8 ScanComp_1_GetChannelDACVoltage(uint8 channel)  \
                                                    
        {
            uint8 DACVoltage = 0u;

            if (channel < ScanComp_1_NUM_OF_CHANNELS)
            {
                DACVoltage = ScanComp_1_DacVoltages[(ScanComp_1_DEFAULT_CHANNELS_VAL - channel)];
            }

            return (DACVoltage);
        }
        
        
        /*******************************************************************************
        * Function Name: ScanComp_1_InitDma
        ********************************************************************************
        *
        * Summary:
        *  Configures a DMA transfer of channel values from memory to VDAC.
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        static void ScanComp_1_InitDma(void)  
        {
            /***************************************************************************
            * Transfers channel values from SRAM to VDAC register. One byte transfer,
            * each burst requires a request.
            ***************************************************************************/
            ScanComp_1_DMA_VDACUpdate_Chan =  ScanComp_1_DMA_DmaInitialize(
                                                    ScanComp_1_DMA_BYTES_PER_BURST,
                                                    ScanComp_1_DMA_REQUEST_PER_BURST,
                                                    HI16(ScanComp_1_DMA_SRC_BASE),
                                                    HI16(ScanComp_1_DMA_DST_BASE));

            ScanComp_1_DMA_VDACUpdate_TD = CyDmaTdAllocate();


            /***************************************************************************
            * One TD looping on itself, increment the source address, but not the
            * destination address.
            ***************************************************************************/
            (void) CyDmaTdSetConfiguration( ScanComp_1_DMA_VDACUpdate_TD,
                                            ScanComp_1_NUM_OF_CHANNELS,
                                            ScanComp_1_DMA_VDACUpdate_TD,
                                            TD_INC_SRC_ADR);

            /* Transfers the value for each channel from memory to VDAC */
            (void) CyDmaTdSetAddress(   ScanComp_1_DMA_VDACUpdate_TD,
                                        LO16((uint32)ScanComp_1_DacVoltages),
                                        LO16((uint32)ScanComp_1_VDAC_Data_PTR));

            (void) CyDmaChSetInitialTd(ScanComp_1_DMA_VDACUpdate_Chan, ScanComp_1_DMA_VDACUpdate_TD);
        }

    #else
        
        /*******************************************************************************
        * Function Name: ScanComp_1_SetDACVoltage
        ********************************************************************************
        *
        * Summary:
        *  Sets the DAC output to a new voltage.
        *
        * Parameters:
        *  DACVoltage: Voltage to be written to the DAC, in units depending
        *  on the selected range
        *
        * Return:
        *  void
        *
        *******************************************************************************/
        void ScanComp_1_SetDACVoltage(uint8 DACVoltage) 
        {
            ScanComp_1_VDAC_SetValue(DACVoltage);
            ScanComp_1_compDACVoltage = DACVoltage;
        }


        /*******************************************************************************
        * Function Name: ScanComp_1_GetDACVoltage
        ********************************************************************************
        *
        * Summary:
        *  Gets the DAC output to a new voltage.
        *
        * Parameters:
        *  None
        *
        * Return:
        *  Voltage the DAC is configured for, in units depending on the selected range
        *  (4 or 16 mV per bit in 1 or 4V range, respectively).
        *
        *******************************************************************************/
        uint8 ScanComp_1_GetDACVoltage(void) 
        {
            return (ScanComp_1_compDACVoltage);
        }

    #endif /* ScanComp_1_VDAC_PER_CHANNEL_MODE_USED */
#endif /* ScanComp_1_INTERNAL_REF_IS_USED */


/* [] END OF FILE */
