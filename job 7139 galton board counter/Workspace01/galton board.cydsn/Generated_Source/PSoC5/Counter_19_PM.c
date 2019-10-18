/*******************************************************************************
* File Name: Counter_19_PM.c  
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

#include "Counter_19.h"

static Counter_19_backupStruct Counter_19_backup;


/*******************************************************************************
* Function Name: Counter_19_SaveConfig
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
*  Counter_19_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_19_SaveConfig(void) 
{
    #if (!Counter_19_UsingFixedFunction)

        Counter_19_backup.CounterUdb = Counter_19_ReadCounter();

        #if(!Counter_19_ControlRegRemoved)
            Counter_19_backup.CounterControlRegister = Counter_19_ReadControlRegister();
        #endif /* (!Counter_19_ControlRegRemoved) */

    #endif /* (!Counter_19_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_19_RestoreConfig
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
*  Counter_19_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_19_RestoreConfig(void) 
{      
    #if (!Counter_19_UsingFixedFunction)

       Counter_19_WriteCounter(Counter_19_backup.CounterUdb);

        #if(!Counter_19_ControlRegRemoved)
            Counter_19_WriteControlRegister(Counter_19_backup.CounterControlRegister);
        #endif /* (!Counter_19_ControlRegRemoved) */

    #endif /* (!Counter_19_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_19_Sleep
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
*  Counter_19_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_19_Sleep(void) 
{
    #if(!Counter_19_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_19_CTRL_ENABLE == (Counter_19_CONTROL & Counter_19_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_19_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_19_backup.CounterEnableState = 0u;
        }
    #else
        Counter_19_backup.CounterEnableState = 1u;
        if(Counter_19_backup.CounterEnableState != 0u)
        {
            Counter_19_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_19_ControlRegRemoved) */
    
    Counter_19_Stop();
    Counter_19_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_19_Wakeup
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
*  Counter_19_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_19_Wakeup(void) 
{
    Counter_19_RestoreConfig();
    #if(!Counter_19_ControlRegRemoved)
        if(Counter_19_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_19_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_19_ControlRegRemoved) */
    
}


/* [] END OF FILE */
