/*******************************************************************************
* File Name: Counter_8_PM.c  
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

#include "Counter_8.h"

static Counter_8_backupStruct Counter_8_backup;


/*******************************************************************************
* Function Name: Counter_8_SaveConfig
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
*  Counter_8_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_8_SaveConfig(void) 
{
    #if (!Counter_8_UsingFixedFunction)

        Counter_8_backup.CounterUdb = Counter_8_ReadCounter();

        #if(!Counter_8_ControlRegRemoved)
            Counter_8_backup.CounterControlRegister = Counter_8_ReadControlRegister();
        #endif /* (!Counter_8_ControlRegRemoved) */

    #endif /* (!Counter_8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_8_RestoreConfig
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
*  Counter_8_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_8_RestoreConfig(void) 
{      
    #if (!Counter_8_UsingFixedFunction)

       Counter_8_WriteCounter(Counter_8_backup.CounterUdb);

        #if(!Counter_8_ControlRegRemoved)
            Counter_8_WriteControlRegister(Counter_8_backup.CounterControlRegister);
        #endif /* (!Counter_8_ControlRegRemoved) */

    #endif /* (!Counter_8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_8_Sleep
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
*  Counter_8_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_8_Sleep(void) 
{
    #if(!Counter_8_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_8_CTRL_ENABLE == (Counter_8_CONTROL & Counter_8_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_8_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_8_backup.CounterEnableState = 0u;
        }
    #else
        Counter_8_backup.CounterEnableState = 1u;
        if(Counter_8_backup.CounterEnableState != 0u)
        {
            Counter_8_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_8_ControlRegRemoved) */
    
    Counter_8_Stop();
    Counter_8_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_8_Wakeup
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
*  Counter_8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_8_Wakeup(void) 
{
    Counter_8_RestoreConfig();
    #if(!Counter_8_ControlRegRemoved)
        if(Counter_8_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_8_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_8_ControlRegRemoved) */
    
}


/* [] END OF FILE */
