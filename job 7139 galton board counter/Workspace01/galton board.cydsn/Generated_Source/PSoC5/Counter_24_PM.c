/*******************************************************************************
* File Name: Counter_24_PM.c  
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

#include "Counter_24.h"

static Counter_24_backupStruct Counter_24_backup;


/*******************************************************************************
* Function Name: Counter_24_SaveConfig
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
*  Counter_24_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_24_SaveConfig(void) 
{
    #if (!Counter_24_UsingFixedFunction)

        Counter_24_backup.CounterUdb = Counter_24_ReadCounter();

        #if(!Counter_24_ControlRegRemoved)
            Counter_24_backup.CounterControlRegister = Counter_24_ReadControlRegister();
        #endif /* (!Counter_24_ControlRegRemoved) */

    #endif /* (!Counter_24_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_24_RestoreConfig
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
*  Counter_24_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_24_RestoreConfig(void) 
{      
    #if (!Counter_24_UsingFixedFunction)

       Counter_24_WriteCounter(Counter_24_backup.CounterUdb);

        #if(!Counter_24_ControlRegRemoved)
            Counter_24_WriteControlRegister(Counter_24_backup.CounterControlRegister);
        #endif /* (!Counter_24_ControlRegRemoved) */

    #endif /* (!Counter_24_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_24_Sleep
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
*  Counter_24_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_24_Sleep(void) 
{
    #if(!Counter_24_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_24_CTRL_ENABLE == (Counter_24_CONTROL & Counter_24_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_24_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_24_backup.CounterEnableState = 0u;
        }
    #else
        Counter_24_backup.CounterEnableState = 1u;
        if(Counter_24_backup.CounterEnableState != 0u)
        {
            Counter_24_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_24_ControlRegRemoved) */
    
    Counter_24_Stop();
    Counter_24_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_24_Wakeup
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
*  Counter_24_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_24_Wakeup(void) 
{
    Counter_24_RestoreConfig();
    #if(!Counter_24_ControlRegRemoved)
        if(Counter_24_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_24_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_24_ControlRegRemoved) */
    
}


/* [] END OF FILE */
