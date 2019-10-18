/*******************************************************************************
* File Name: Counter_5_PM.c  
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

#include "Counter_5.h"

static Counter_5_backupStruct Counter_5_backup;


/*******************************************************************************
* Function Name: Counter_5_SaveConfig
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
*  Counter_5_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_5_SaveConfig(void) 
{
    #if (!Counter_5_UsingFixedFunction)

        Counter_5_backup.CounterUdb = Counter_5_ReadCounter();

        #if(!Counter_5_ControlRegRemoved)
            Counter_5_backup.CounterControlRegister = Counter_5_ReadControlRegister();
        #endif /* (!Counter_5_ControlRegRemoved) */

    #endif /* (!Counter_5_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_5_RestoreConfig
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
*  Counter_5_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_5_RestoreConfig(void) 
{      
    #if (!Counter_5_UsingFixedFunction)

       Counter_5_WriteCounter(Counter_5_backup.CounterUdb);

        #if(!Counter_5_ControlRegRemoved)
            Counter_5_WriteControlRegister(Counter_5_backup.CounterControlRegister);
        #endif /* (!Counter_5_ControlRegRemoved) */

    #endif /* (!Counter_5_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_5_Sleep
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
*  Counter_5_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_5_Sleep(void) 
{
    #if(!Counter_5_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_5_CTRL_ENABLE == (Counter_5_CONTROL & Counter_5_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_5_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_5_backup.CounterEnableState = 0u;
        }
    #else
        Counter_5_backup.CounterEnableState = 1u;
        if(Counter_5_backup.CounterEnableState != 0u)
        {
            Counter_5_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_5_ControlRegRemoved) */
    
    Counter_5_Stop();
    Counter_5_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_5_Wakeup
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
*  Counter_5_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_5_Wakeup(void) 
{
    Counter_5_RestoreConfig();
    #if(!Counter_5_ControlRegRemoved)
        if(Counter_5_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_5_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_5_ControlRegRemoved) */
    
}


/* [] END OF FILE */
