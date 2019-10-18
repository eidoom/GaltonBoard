/*******************************************************************************
* File Name: Counter_10_PM.c  
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

#include "Counter_10.h"

static Counter_10_backupStruct Counter_10_backup;


/*******************************************************************************
* Function Name: Counter_10_SaveConfig
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
*  Counter_10_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_10_SaveConfig(void) 
{
    #if (!Counter_10_UsingFixedFunction)

        Counter_10_backup.CounterUdb = Counter_10_ReadCounter();

        #if(!Counter_10_ControlRegRemoved)
            Counter_10_backup.CounterControlRegister = Counter_10_ReadControlRegister();
        #endif /* (!Counter_10_ControlRegRemoved) */

    #endif /* (!Counter_10_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_10_RestoreConfig
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
*  Counter_10_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_10_RestoreConfig(void) 
{      
    #if (!Counter_10_UsingFixedFunction)

       Counter_10_WriteCounter(Counter_10_backup.CounterUdb);

        #if(!Counter_10_ControlRegRemoved)
            Counter_10_WriteControlRegister(Counter_10_backup.CounterControlRegister);
        #endif /* (!Counter_10_ControlRegRemoved) */

    #endif /* (!Counter_10_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_10_Sleep
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
*  Counter_10_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_10_Sleep(void) 
{
    #if(!Counter_10_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_10_CTRL_ENABLE == (Counter_10_CONTROL & Counter_10_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_10_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_10_backup.CounterEnableState = 0u;
        }
    #else
        Counter_10_backup.CounterEnableState = 1u;
        if(Counter_10_backup.CounterEnableState != 0u)
        {
            Counter_10_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_10_ControlRegRemoved) */
    
    Counter_10_Stop();
    Counter_10_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_10_Wakeup
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
*  Counter_10_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_10_Wakeup(void) 
{
    Counter_10_RestoreConfig();
    #if(!Counter_10_ControlRegRemoved)
        if(Counter_10_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_10_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_10_ControlRegRemoved) */
    
}


/* [] END OF FILE */
