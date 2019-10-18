/*******************************************************************************
* File Name: Counter_4_PM.c  
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

#include "Counter_4.h"

static Counter_4_backupStruct Counter_4_backup;


/*******************************************************************************
* Function Name: Counter_4_SaveConfig
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
*  Counter_4_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_4_SaveConfig(void) 
{
    #if (!Counter_4_UsingFixedFunction)

        Counter_4_backup.CounterUdb = Counter_4_ReadCounter();

        #if(!Counter_4_ControlRegRemoved)
            Counter_4_backup.CounterControlRegister = Counter_4_ReadControlRegister();
        #endif /* (!Counter_4_ControlRegRemoved) */

    #endif /* (!Counter_4_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_4_RestoreConfig
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
*  Counter_4_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_4_RestoreConfig(void) 
{      
    #if (!Counter_4_UsingFixedFunction)

       Counter_4_WriteCounter(Counter_4_backup.CounterUdb);

        #if(!Counter_4_ControlRegRemoved)
            Counter_4_WriteControlRegister(Counter_4_backup.CounterControlRegister);
        #endif /* (!Counter_4_ControlRegRemoved) */

    #endif /* (!Counter_4_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_4_Sleep
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
*  Counter_4_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_4_Sleep(void) 
{
    #if(!Counter_4_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_4_CTRL_ENABLE == (Counter_4_CONTROL & Counter_4_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_4_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_4_backup.CounterEnableState = 0u;
        }
    #else
        Counter_4_backup.CounterEnableState = 1u;
        if(Counter_4_backup.CounterEnableState != 0u)
        {
            Counter_4_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_4_ControlRegRemoved) */
    
    Counter_4_Stop();
    Counter_4_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_4_Wakeup
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
*  Counter_4_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_4_Wakeup(void) 
{
    Counter_4_RestoreConfig();
    #if(!Counter_4_ControlRegRemoved)
        if(Counter_4_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_4_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_4_ControlRegRemoved) */
    
}


/* [] END OF FILE */
