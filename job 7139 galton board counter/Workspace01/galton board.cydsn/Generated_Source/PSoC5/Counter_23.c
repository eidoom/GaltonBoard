/*******************************************************************************
* File Name: Counter_23.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

#include "Counter_23.h"

uint8 Counter_23_initVar = 0u;


/*******************************************************************************
* Function Name: Counter_23_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void Counter_23_Init(void) 
{
        #if (!Counter_23_UsingFixedFunction && !Counter_23_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!Counter_23_UsingFixedFunction && !Counter_23_ControlRegRemoved) */
        
        #if(!Counter_23_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 Counter_23_interruptState;
        #endif /* (!Counter_23_UsingFixedFunction) */
        
        #if (Counter_23_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            Counter_23_CONTROL &= Counter_23_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                Counter_23_CONTROL2 &= ((uint8)(~Counter_23_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Counter_23_CONTROL3 &= ((uint8)(~Counter_23_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (Counter_23_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                Counter_23_CONTROL |= Counter_23_ONESHOT;
            #endif /* (Counter_23_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            Counter_23_CONTROL2 |= Counter_23_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            Counter_23_RT1 &= ((uint8)(~Counter_23_RT1_MASK));
            Counter_23_RT1 |= Counter_23_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            Counter_23_RT1 &= ((uint8)(~Counter_23_SYNCDSI_MASK));
            Counter_23_RT1 |= Counter_23_SYNCDSI_EN;

        #else
            #if(!Counter_23_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = Counter_23_CONTROL & ((uint8)(~Counter_23_CTRL_CMPMODE_MASK));
            Counter_23_CONTROL = ctrl | Counter_23_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = Counter_23_CONTROL & ((uint8)(~Counter_23_CTRL_CAPMODE_MASK));
            
            #if( 0 != Counter_23_CAPTURE_MODE_CONF)
                Counter_23_CONTROL = ctrl | Counter_23_DEFAULT_CAPTURE_MODE;
            #else
                Counter_23_CONTROL = ctrl;
            #endif /* 0 != Counter_23_CAPTURE_MODE */ 
            
            #endif /* (!Counter_23_ControlRegRemoved) */
        #endif /* (Counter_23_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!Counter_23_UsingFixedFunction)
            Counter_23_ClearFIFO();
        #endif /* (!Counter_23_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        Counter_23_WritePeriod(Counter_23_INIT_PERIOD_VALUE);
        #if (!(Counter_23_UsingFixedFunction && (CY_PSOC5A)))
            Counter_23_WriteCounter(Counter_23_INIT_COUNTER_VALUE);
        #endif /* (!(Counter_23_UsingFixedFunction && (CY_PSOC5A))) */
        Counter_23_SetInterruptMode(Counter_23_INIT_INTERRUPTS_MASK);
        
        #if (!Counter_23_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)Counter_23_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            Counter_23_WriteCompare(Counter_23_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Counter_23_interruptState = CyEnterCriticalSection();
            
            Counter_23_STATUS_AUX_CTRL |= Counter_23_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(Counter_23_interruptState);
            
        #endif /* (!Counter_23_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_23_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void Counter_23_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Counter_23_UsingFixedFunction)
        Counter_23_GLOBAL_ENABLE |= Counter_23_BLOCK_EN_MASK;
        Counter_23_GLOBAL_STBY_ENABLE |= Counter_23_BLOCK_STBY_EN_MASK;
    #endif /* (Counter_23_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!Counter_23_ControlRegRemoved || Counter_23_UsingFixedFunction)
        Counter_23_CONTROL |= Counter_23_CTRL_ENABLE;                
    #endif /* (!Counter_23_ControlRegRemoved || Counter_23_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: Counter_23_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  Counter_23_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Counter_23_Start(void) 
{
    if(Counter_23_initVar == 0u)
    {
        Counter_23_Init();
        
        Counter_23_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    Counter_23_Enable();        
}


/*******************************************************************************
* Function Name: Counter_23_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void Counter_23_Stop(void) 
{
    /* Disable Counter */
    #if(!Counter_23_ControlRegRemoved || Counter_23_UsingFixedFunction)
        Counter_23_CONTROL &= ((uint8)(~Counter_23_CTRL_ENABLE));        
    #endif /* (!Counter_23_ControlRegRemoved || Counter_23_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Counter_23_UsingFixedFunction)
        Counter_23_GLOBAL_ENABLE &= ((uint8)(~Counter_23_BLOCK_EN_MASK));
        Counter_23_GLOBAL_STBY_ENABLE &= ((uint8)(~Counter_23_BLOCK_STBY_EN_MASK));
    #endif /* (Counter_23_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_23_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void Counter_23_SetInterruptMode(uint8 interruptsMask) 
{
    Counter_23_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: Counter_23_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   Counter_23_ReadStatusRegister(void) 
{
    return Counter_23_STATUS;
}


#if(!Counter_23_ControlRegRemoved)
/*******************************************************************************
* Function Name: Counter_23_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   Counter_23_ReadControlRegister(void) 
{
    return Counter_23_CONTROL;
}


/*******************************************************************************
* Function Name: Counter_23_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    Counter_23_WriteControlRegister(uint8 control) 
{
    Counter_23_CONTROL = control;
}

#endif  /* (!Counter_23_ControlRegRemoved) */


#if (!(Counter_23_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: Counter_23_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void Counter_23_WriteCounter(uint8 counter) \
                                   
{
    #if(Counter_23_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (Counter_23_GLOBAL_ENABLE & Counter_23_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        Counter_23_GLOBAL_ENABLE |= Counter_23_BLOCK_EN_MASK;
        CY_SET_REG16(Counter_23_COUNTER_LSB_PTR, (uint16)counter);
        Counter_23_GLOBAL_ENABLE &= ((uint8)(~Counter_23_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(Counter_23_COUNTER_LSB_PTR, counter);
    #endif /* (Counter_23_UsingFixedFunction) */
}
#endif /* (!(Counter_23_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: Counter_23_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) The present value of the counter.
*
*******************************************************************************/
uint8 Counter_23_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(Counter_23_UsingFixedFunction)
		(void)CY_GET_REG16(Counter_23_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(Counter_23_COUNTER_LSB_PTR_8BIT);
	#endif/* (Counter_23_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(Counter_23_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_23_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_23_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_23_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_23_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) Present Capture value.
*
*******************************************************************************/
uint8 Counter_23_ReadCapture(void) 
{
    #if(Counter_23_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_23_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_23_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_23_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_23_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint8) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void Counter_23_WritePeriod(uint8 period) 
{
    #if(Counter_23_UsingFixedFunction)
        CY_SET_REG16(Counter_23_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(Counter_23_PERIOD_LSB_PTR, period);
    #endif /* (Counter_23_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_23_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) Present period value.
*
*******************************************************************************/
uint8 Counter_23_ReadPeriod(void) 
{
    #if(Counter_23_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_23_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_23_PERIOD_LSB_PTR));
    #endif /* (Counter_23_UsingFixedFunction) */
}


#if (!Counter_23_UsingFixedFunction)
/*******************************************************************************
* Function Name: Counter_23_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void Counter_23_WriteCompare(uint8 compare) \
                                   
{
    #if(Counter_23_UsingFixedFunction)
        CY_SET_REG16(Counter_23_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(Counter_23_COMPARE_LSB_PTR, compare);
    #endif /* (Counter_23_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_23_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint8) Present compare value.
*
*******************************************************************************/
uint8 Counter_23_ReadCompare(void) 
{
    return (CY_GET_REG8(Counter_23_COMPARE_LSB_PTR));
}


#if (Counter_23_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_23_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void Counter_23_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    Counter_23_CONTROL &= ((uint8)(~Counter_23_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    Counter_23_CONTROL |= compareMode;
}
#endif  /* (Counter_23_COMPARE_MODE_SOFTWARE) */


#if (Counter_23_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_23_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void Counter_23_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    Counter_23_CONTROL &= ((uint8)(~Counter_23_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    Counter_23_CONTROL |= ((uint8)((uint8)captureMode << Counter_23_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (Counter_23_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: Counter_23_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void Counter_23_ClearFIFO(void) 
{

    while(0u != (Counter_23_ReadStatusRegister() & Counter_23_STATUS_FIFONEMP))
    {
        (void)Counter_23_ReadCapture();
    }

}
#endif  /* (!Counter_23_UsingFixedFunction) */


/* [] END OF FILE */

