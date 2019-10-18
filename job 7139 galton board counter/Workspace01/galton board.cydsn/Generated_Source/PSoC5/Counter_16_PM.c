/*******************************************************************************
* File Name: Counter_16_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Counter_16.h"

static Counter_16_backupStruct Counter_16_backup;


/*******************************************************************************
* Function Name: Counter_16_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_16_SaveConfig(void) 
{
    #if (!Counter_16_UsingFixedFunction)

        Counter_16_backup.CounterUdb = Counter_16_ReadCounter();

        #if(!Counter_16_ControlRegRemoved)
            Counter_16_backup.CounterControlRegister = Counter_16_ReadControlRegister();
        #endif /* (!Counter_16_ControlRegRemoved) */

    #endif /* (!Counter_16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_16_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_16_RestoreConfig(void) 
{      
    #if (!Counter_16_UsingFixedFunction)

       Counter_16_WriteCounter(Counter_16_backup.CounterUdb);

        #if(!Counter_16_ControlRegRemoved)
            Counter_16_WriteControlRegister(Counter_16_backup.CounterControlRegister);
        #endif /* (!Counter_16_ControlRegRemoved) */

    #endif /* (!Counter_16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_16_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_16_Sleep(void) 
{
    #if(!Counter_16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_16_CTRL_ENABLE == (Counter_16_CONTROL & Counter_16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_16_backup.CounterEnableState = 0u;
        }
    #else
        Counter_16_backup.CounterEnableState = 1u;
        if(Counter_16_backup.CounterEnableState != 0u)
        {
            Counter_16_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_16_ControlRegRemoved) */
    
    Counter_16_Stop();
    Counter_16_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_16_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_16_Wakeup(void) 
{
    Counter_16_RestoreConfig();
    #if(!Counter_16_ControlRegRemoved)
        if(Counter_16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_16_ControlRegRemoved) */
    
}


/* [] END OF FILE */
