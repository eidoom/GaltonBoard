/*******************************************************************************
* File Name: Counter_20_PM.c  
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

#include "Counter_20.h"

static Counter_20_backupStruct Counter_20_backup;


/*******************************************************************************
* Function Name: Counter_20_SaveConfig
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
*  Counter_20_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_20_SaveConfig(void) 
{
    #if (!Counter_20_UsingFixedFunction)

        Counter_20_backup.CounterUdb = Counter_20_ReadCounter();

        #if(!Counter_20_ControlRegRemoved)
            Counter_20_backup.CounterControlRegister = Counter_20_ReadControlRegister();
        #endif /* (!Counter_20_ControlRegRemoved) */

    #endif /* (!Counter_20_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_20_RestoreConfig
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
*  Counter_20_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_20_RestoreConfig(void) 
{      
    #if (!Counter_20_UsingFixedFunction)

       Counter_20_WriteCounter(Counter_20_backup.CounterUdb);

        #if(!Counter_20_ControlRegRemoved)
            Counter_20_WriteControlRegister(Counter_20_backup.CounterControlRegister);
        #endif /* (!Counter_20_ControlRegRemoved) */

    #endif /* (!Counter_20_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_20_Sleep
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
*  Counter_20_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_20_Sleep(void) 
{
    #if(!Counter_20_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_20_CTRL_ENABLE == (Counter_20_CONTROL & Counter_20_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_20_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_20_backup.CounterEnableState = 0u;
        }
    #else
        Counter_20_backup.CounterEnableState = 1u;
        if(Counter_20_backup.CounterEnableState != 0u)
        {
            Counter_20_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_20_ControlRegRemoved) */
    
    Counter_20_Stop();
    Counter_20_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_20_Wakeup
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
*  Counter_20_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_20_Wakeup(void) 
{
    Counter_20_RestoreConfig();
    #if(!Counter_20_ControlRegRemoved)
        if(Counter_20_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_20_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_20_ControlRegRemoved) */
    
}


/* [] END OF FILE */
