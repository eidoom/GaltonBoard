/*******************************************************************************
* File Name: Counter_3_PM.c  
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

#include "Counter_3.h"

static Counter_3_backupStruct Counter_3_backup;


/*******************************************************************************
* Function Name: Counter_3_SaveConfig
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
*  Counter_3_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_3_SaveConfig(void) 
{
    #if (!Counter_3_UsingFixedFunction)

        Counter_3_backup.CounterUdb = Counter_3_ReadCounter();

        #if(!Counter_3_ControlRegRemoved)
            Counter_3_backup.CounterControlRegister = Counter_3_ReadControlRegister();
        #endif /* (!Counter_3_ControlRegRemoved) */

    #endif /* (!Counter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_3_RestoreConfig
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
*  Counter_3_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_3_RestoreConfig(void) 
{      
    #if (!Counter_3_UsingFixedFunction)

       Counter_3_WriteCounter(Counter_3_backup.CounterUdb);

        #if(!Counter_3_ControlRegRemoved)
            Counter_3_WriteControlRegister(Counter_3_backup.CounterControlRegister);
        #endif /* (!Counter_3_ControlRegRemoved) */

    #endif /* (!Counter_3_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_3_Sleep
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
*  Counter_3_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_3_Sleep(void) 
{
    #if(!Counter_3_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_3_CTRL_ENABLE == (Counter_3_CONTROL & Counter_3_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_3_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_3_backup.CounterEnableState = 0u;
        }
    #else
        Counter_3_backup.CounterEnableState = 1u;
        if(Counter_3_backup.CounterEnableState != 0u)
        {
            Counter_3_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_3_ControlRegRemoved) */
    
    Counter_3_Stop();
    Counter_3_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_3_Wakeup
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
*  Counter_3_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_3_Wakeup(void) 
{
    Counter_3_RestoreConfig();
    #if(!Counter_3_ControlRegRemoved)
        if(Counter_3_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_3_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_3_ControlRegRemoved) */
    
}


/* [] END OF FILE */
