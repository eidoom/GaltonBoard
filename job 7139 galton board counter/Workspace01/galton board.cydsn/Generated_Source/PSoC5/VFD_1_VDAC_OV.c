/*******************************************************************************
* File Name: VFD_1_VDAC_OV.c  
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
#include "VFD_1_VDAC_OV.h"

#if (CY_PSOC5A)
#include <CyLib.h>
#endif /* CY_PSOC5A */

uint8 VFD_1_VDAC_OV_initVar = 0u;

#if (CY_PSOC5A)
    static uint8 VFD_1_VDAC_OV_restoreVal = 0u;
#endif /* CY_PSOC5A */

#if (CY_PSOC5A)
    static VFD_1_VDAC_OV_backupStruct VFD_1_VDAC_OV_backup;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_Init
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
void VFD_1_VDAC_OV_Init(void) 
{
    VFD_1_VDAC_OV_CR0 = (VFD_1_VDAC_OV_MODE_V );

    /* Set default data source */
    #if(VFD_1_VDAC_OV_DEFAULT_DATA_SRC != 0 )
        VFD_1_VDAC_OV_CR1 = (VFD_1_VDAC_OV_DEFAULT_CNTL | VFD_1_VDAC_OV_DACBUS_ENABLE) ;
    #else
        VFD_1_VDAC_OV_CR1 = (VFD_1_VDAC_OV_DEFAULT_CNTL | VFD_1_VDAC_OV_DACBUS_DISABLE) ;
    #endif /* (VFD_1_VDAC_OV_DEFAULT_DATA_SRC != 0 ) */

    /* Set default strobe mode */
    #if(VFD_1_VDAC_OV_DEFAULT_STRB != 0)
        VFD_1_VDAC_OV_Strobe |= VFD_1_VDAC_OV_STRB_EN ;
    #endif/* (VFD_1_VDAC_OV_DEFAULT_STRB != 0) */

    /* Set default range */
    VFD_1_VDAC_OV_SetRange(VFD_1_VDAC_OV_DEFAULT_RANGE); 

    /* Set default speed */
    VFD_1_VDAC_OV_SetSpeed(VFD_1_VDAC_OV_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_Enable
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
void VFD_1_VDAC_OV_Enable(void) 
{
    VFD_1_VDAC_OV_PWRMGR |= VFD_1_VDAC_OV_ACT_PWR_EN;
    VFD_1_VDAC_OV_STBY_PWRMGR |= VFD_1_VDAC_OV_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */
    #if (CY_PSOC5A)
        if(VFD_1_VDAC_OV_restoreVal == 1u) 
        {
             VFD_1_VDAC_OV_CR0 = VFD_1_VDAC_OV_backup.data_value;
             VFD_1_VDAC_OV_restoreVal = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_Start
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
*  VFD_1_VDAC_OV_initVar: Is modified when this function is called for the 
*  first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void VFD_1_VDAC_OV_Start(void)  
{
    /* Hardware initiazation only needs to occure the first time */
    if(VFD_1_VDAC_OV_initVar == 0u)
    { 
        VFD_1_VDAC_OV_Init();
        VFD_1_VDAC_OV_initVar = 1u;
    }

    /* Enable power to DAC */
    VFD_1_VDAC_OV_Enable();

    /* Set default value */
    VFD_1_VDAC_OV_SetValue(VFD_1_VDAC_OV_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_Stop
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
void VFD_1_VDAC_OV_Stop(void) 
{
    /* Disble power to DAC */
    VFD_1_VDAC_OV_PWRMGR &= (uint8)(~VFD_1_VDAC_OV_ACT_PWR_EN);
    VFD_1_VDAC_OV_STBY_PWRMGR &= (uint8)(~VFD_1_VDAC_OV_STBY_PWR_EN);

    /* This is a work around for PSoC5A  ,
    this sets VDAC to current mode with output off */
    #if (CY_PSOC5A)
        VFD_1_VDAC_OV_backup.data_value = VFD_1_VDAC_OV_CR0;
        VFD_1_VDAC_OV_CR0 = VFD_1_VDAC_OV_CUR_MODE_OUT_OFF;
        VFD_1_VDAC_OV_restoreVal = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_SetSpeed
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
void VFD_1_VDAC_OV_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    VFD_1_VDAC_OV_CR0 &= (uint8)(~VFD_1_VDAC_OV_HS_MASK);
    VFD_1_VDAC_OV_CR0 |=  (speed & VFD_1_VDAC_OV_HS_MASK);
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_SetRange
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
void VFD_1_VDAC_OV_SetRange(uint8 range) 
{
    VFD_1_VDAC_OV_CR0 &= (uint8)(~VFD_1_VDAC_OV_RANGE_MASK);      /* Clear existing mode */
    VFD_1_VDAC_OV_CR0 |= (range & VFD_1_VDAC_OV_RANGE_MASK);      /*  Set Range  */
    VFD_1_VDAC_OV_DacTrim();
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_SetValue
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
void VFD_1_VDAC_OV_SetValue(uint8 value) 
{
    #if (CY_PSOC5A)
        uint8 VFD_1_VDAC_OV_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    VFD_1_VDAC_OV_Data = value;                /*  Set Value  */

    /* PSOC5A requires a double write */
    /* Exit Critical Section */
    #if (CY_PSOC5A)
        VFD_1_VDAC_OV_Data = value;
        CyExitCriticalSection(VFD_1_VDAC_OV_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_DacTrim
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
void VFD_1_VDAC_OV_DacTrim(void) 
{
    uint8 mode;

    mode = (uint8)((VFD_1_VDAC_OV_CR0 & VFD_1_VDAC_OV_RANGE_MASK) >> 2) + VFD_1_VDAC_OV_TRIM_M7_1V_RNG_OFFSET;
    VFD_1_VDAC_OV_TR = CY_GET_XTND_REG8((uint8 *)(VFD_1_VDAC_OV_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
