/*******************************************************************************
* File Name: ScanComp_1_VDAC.c  
* Version 1.90
*
* Description:
*  This file provides the source code to the API for the 8-bit Voltage DAC 
*  (VDAC8) User Module.
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "ScanComp_1_VDAC.h"

#if (CY_PSOC5A)
#include <CyLib.h>
#endif /* CY_PSOC5A */

uint8 ScanComp_1_VDAC_initVar = 0u;

#if (CY_PSOC5A)
    static uint8 ScanComp_1_VDAC_restoreVal = 0u;
#endif /* CY_PSOC5A */

#if (CY_PSOC5A)
    static ScanComp_1_VDAC_backupStruct ScanComp_1_VDAC_backup;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
* 
* Parameters:
*  void:
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void ScanComp_1_VDAC_Init(void) 
{
    ScanComp_1_VDAC_CR0 = (ScanComp_1_VDAC_MODE_V );

    /* Set default data source */
    #if(ScanComp_1_VDAC_DEFAULT_DATA_SRC != 0 )
        ScanComp_1_VDAC_CR1 = (ScanComp_1_VDAC_DEFAULT_CNTL | ScanComp_1_VDAC_DACBUS_ENABLE) ;
    #else
        ScanComp_1_VDAC_CR1 = (ScanComp_1_VDAC_DEFAULT_CNTL | ScanComp_1_VDAC_DACBUS_DISABLE) ;
    #endif /* (ScanComp_1_VDAC_DEFAULT_DATA_SRC != 0 ) */

    /* Set default strobe mode */
    #if(ScanComp_1_VDAC_DEFAULT_STRB != 0)
        ScanComp_1_VDAC_Strobe |= ScanComp_1_VDAC_STRB_EN ;
    #endif/* (ScanComp_1_VDAC_DEFAULT_STRB != 0) */

    /* Set default range */
    ScanComp_1_VDAC_SetRange(ScanComp_1_VDAC_DEFAULT_RANGE); 

    /* Set default speed */
    ScanComp_1_VDAC_SetSpeed(ScanComp_1_VDAC_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_Enable
********************************************************************************
* Summary:
*  Enable the VDAC8
* 
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void ScanComp_1_VDAC_Enable(void) 
{
    ScanComp_1_VDAC_PWRMGR |= ScanComp_1_VDAC_ACT_PWR_EN;
    ScanComp_1_VDAC_STBY_PWRMGR |= ScanComp_1_VDAC_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */
    #if (CY_PSOC5A)
        if(ScanComp_1_VDAC_restoreVal == 1u) 
        {
             ScanComp_1_VDAC_CR0 = ScanComp_1_VDAC_backup.data_value;
             ScanComp_1_VDAC_restoreVal = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as
*  executing the stop function.
*
* Parameters:
*  Power: Sets power level between off (0) and (3) high power
*
* Return:
*  void 
*
* Global variables:
*  ScanComp_1_VDAC_initVar: Is modified when this function is called for the 
*  first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void ScanComp_1_VDAC_Start(void)  
{
    /* Hardware initiazation only needs to occure the first time */
    if(ScanComp_1_VDAC_initVar == 0u)
    { 
        ScanComp_1_VDAC_Init();
        ScanComp_1_VDAC_initVar = 1u;
    }

    /* Enable power to DAC */
    ScanComp_1_VDAC_Enable();

    /* Set default value */
    ScanComp_1_VDAC_SetValue(ScanComp_1_VDAC_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_Stop
********************************************************************************
*
* Summary:
*  Powers down DAC to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void ScanComp_1_VDAC_Stop(void) 
{
    /* Disble power to DAC */
    ScanComp_1_VDAC_PWRMGR &= (uint8)(~ScanComp_1_VDAC_ACT_PWR_EN);
    ScanComp_1_VDAC_STBY_PWRMGR &= (uint8)(~ScanComp_1_VDAC_STBY_PWR_EN);

    /* This is a work around for PSoC5A  ,
    this sets VDAC to current mode with output off */
    #if (CY_PSOC5A)
        ScanComp_1_VDAC_backup.data_value = ScanComp_1_VDAC_CR0;
        ScanComp_1_VDAC_CR0 = ScanComp_1_VDAC_CUR_MODE_OUT_OFF;
        ScanComp_1_VDAC_restoreVal = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_SetSpeed
********************************************************************************
*
* Summary:
*  Set DAC speed
*
* Parameters:
*  power: Sets speed value
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void ScanComp_1_VDAC_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    ScanComp_1_VDAC_CR0 &= (uint8)(~ScanComp_1_VDAC_HS_MASK);
    ScanComp_1_VDAC_CR0 |=  (speed & ScanComp_1_VDAC_HS_MASK);
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_SetRange
********************************************************************************
*
* Summary:
*  Set one of three current ranges.
*
* Parameters:
*  Range: Sets one of Three valid ranges.
*
* Return:
*  void 
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void ScanComp_1_VDAC_SetRange(uint8 range) 
{
    ScanComp_1_VDAC_CR0 &= (uint8)(~ScanComp_1_VDAC_RANGE_MASK);      /* Clear existing mode */
    ScanComp_1_VDAC_CR0 |= (range & ScanComp_1_VDAC_RANGE_MASK);      /*  Set Range  */
    ScanComp_1_VDAC_DacTrim();
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_SetValue
********************************************************************************
*
* Summary:
*  Set 8-bit DAC value
*
* Parameters:  
*  value:  Sets DAC value between 0 and 255.
*
* Return: 
*  void 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void ScanComp_1_VDAC_SetValue(uint8 value) 
{
    #if (CY_PSOC5A)
        uint8 ScanComp_1_VDAC_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    ScanComp_1_VDAC_Data = value;                /*  Set Value  */

    /* PSOC5A requires a double write */
    /* Exit Critical Section */
    #if (CY_PSOC5A)
        ScanComp_1_VDAC_Data = value;
        CyExitCriticalSection(ScanComp_1_VDAC_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_DacTrim
********************************************************************************
*
* Summary:
*  Set the trim value for the given range.
*
* Parameters:
*  range:  1V or 4V range.  See constants.
*
* Return:
*  void
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void ScanComp_1_VDAC_DacTrim(void) 
{
    uint8 mode;

    mode = (uint8)((ScanComp_1_VDAC_CR0 & ScanComp_1_VDAC_RANGE_MASK) >> 2) + ScanComp_1_VDAC_TRIM_M7_1V_RNG_OFFSET;
    ScanComp_1_VDAC_TR = CY_GET_XTND_REG8((uint8 *)(ScanComp_1_VDAC_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
