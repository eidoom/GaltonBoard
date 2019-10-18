/*******************************************************************************
* File Name: Counter_16.c  
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

#include "Counter_16.h"

uint8 Counter_16_initVar = 0u;


/*******************************************************************************
* Function Name: Counter_16_Init
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
void Counter_16_Init(void) 
{
        #if (!Counter_16_UsingFixedFunction && !Counter_16_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!Counter_16_UsingFixedFunction && !Counter_16_ControlRegRemoved) */
        
        #if(!Counter_16_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 Counter_16_interruptState;
        #endif /* (!Counter_16_UsingFixedFunction) */
        
        #if (Counter_16_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            Counter_16_CONTROL &= Counter_16_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                Counter_16_CONTROL2 &= ((uint8)(~Counter_16_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Counter_16_CONTROL3 &= ((uint8)(~Counter_16_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (Counter_16_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                Counter_16_CONTROL |= Counter_16_ONESHOT;
            #endif /* (Counter_16_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            Counter_16_CONTROL2 |= Counter_16_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            Counter_16_RT1 &= ((uint8)(~Counter_16_RT1_MASK));
            Counter_16_RT1 |= Counter_16_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            Counter_16_RT1 &= ((uint8)(~Counter_16_SYNCDSI_MASK));
            Counter_16_RT1 |= Counter_16_SYNCDSI_EN;

        #else
            #if(!Counter_16_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = Counter_16_CONTROL & ((uint8)(~Counter_16_CTRL_CMPMODE_MASK));
            Counter_16_CONTROL = ctrl | Counter_16_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = Counter_16_CONTROL & ((uint8)(~Counter_16_CTRL_CAPMODE_MASK));
            
            #if( 0 != Counter_16_CAPTURE_MODE_CONF)
                Counter_16_CONTROL = ctrl | Counter_16_DEFAULT_CAPTURE_MODE;
            #else
                Counter_16_CONTROL = ctrl;
            #endif /* 0 != Counter_16_CAPTURE_MODE */ 
            
            #endif /* (!Counter_16_ControlRegRemoved) */
        #endif /* (Counter_16_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!Counter_16_UsingFixedFunction)
            Counter_16_ClearFIFO();
        #endif /* (!Counter_16_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        Counter_16_WritePeriod(Counter_16_INIT_PERIOD_VALUE);
        #if (!(Counter_16_UsingFixedFunction && (CY_PSOC5A)))
            Counter_16_WriteCounter(Counter_16_INIT_COUNTER_VALUE);
        #endif /* (!(Counter_16_UsingFixedFunction && (CY_PSOC5A))) */
        Counter_16_SetInterruptMode(Counter_16_INIT_INTERRUPTS_MASK);
        
        #if (!Counter_16_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)Counter_16_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            Counter_16_WriteCompare(Counter_16_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Counter_16_interruptState = CyEnterCriticalSection();
            
            Counter_16_STATUS_AUX_CTRL |= Counter_16_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(Counter_16_interruptState);
            
        #endif /* (!Counter_16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_16_Enable
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
void Counter_16_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Counter_16_UsingFixedFunction)
        Counter_16_GLOBAL_ENABLE |= Counter_16_BLOCK_EN_MASK;
        Counter_16_GLOBAL_STBY_ENABLE |= Counter_16_BLOCK_STBY_EN_MASK;
    #endif /* (Counter_16_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!Counter_16_ControlRegRemoved || Counter_16_UsingFixedFunction)
        Counter_16_CONTROL |= Counter_16_CTRL_ENABLE;                
    #endif /* (!Counter_16_ControlRegRemoved || Counter_16_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: Counter_16_Start
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
*  Counter_16_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Counter_16_Start(void) 
{
    if(Counter_16_initVar == 0u)
    {
        Counter_16_Init();
        
        Counter_16_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    Counter_16_Enable();        
}


/*******************************************************************************
* Function Name: Counter_16_Stop
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
void Counter_16_Stop(void) 
{
    /* Disable Counter */
    #if(!Counter_16_ControlRegRemoved || Counter_16_UsingFixedFunction)
        Counter_16_CONTROL &= ((uint8)(~Counter_16_CTRL_ENABLE));        
    #endif /* (!Counter_16_ControlRegRemoved || Counter_16_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Counter_16_UsingFixedFunction)
        Counter_16_GLOBAL_ENABLE &= ((uint8)(~Counter_16_BLOCK_EN_MASK));
        Counter_16_GLOBAL_STBY_ENABLE &= ((uint8)(~Counter_16_BLOCK_STBY_EN_MASK));
    #endif /* (Counter_16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_16_SetInterruptMode
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
void Counter_16_SetInterruptMode(uint8 interruptsMask) 
{
    Counter_16_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: Counter_16_ReadStatusRegister
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
uint8   Counter_16_ReadStatusRegister(void) 
{
    return Counter_16_STATUS;
}


#if(!Counter_16_ControlRegRemoved)
/*******************************************************************************
* Function Name: Counter_16_ReadControlRegister
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
uint8   Counter_16_ReadControlRegister(void) 
{
    return Counter_16_CONTROL;
}


/*******************************************************************************
* Function Name: Counter_16_WriteControlRegister
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
void    Counter_16_WriteControlRegister(uint8 control) 
{
    Counter_16_CONTROL = control;
}

#endif  /* (!Counter_16_ControlRegRemoved) */


#if (!(Counter_16_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: Counter_16_WriteCounter
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
void Counter_16_WriteCounter(uint8 counter) \
                                   
{
    #if(Counter_16_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (Counter_16_GLOBAL_ENABLE & Counter_16_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        Counter_16_GLOBAL_ENABLE |= Counter_16_BLOCK_EN_MASK;
        CY_SET_REG16(Counter_16_COUNTER_LSB_PTR, (uint16)counter);
        Counter_16_GLOBAL_ENABLE &= ((uint8)(~Counter_16_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(Counter_16_COUNTER_LSB_PTR, counter);
    #endif /* (Counter_16_UsingFixedFunction) */
}
#endif /* (!(Counter_16_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: Counter_16_ReadCounter
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
uint8 Counter_16_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(Counter_16_UsingFixedFunction)
		(void)CY_GET_REG16(Counter_16_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(Counter_16_COUNTER_LSB_PTR_8BIT);
	#endif/* (Counter_16_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(Counter_16_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_16_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_16_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_16_ReadCapture
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
uint8 Counter_16_ReadCapture(void) 
{
    #if(Counter_16_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_16_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_16_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_16_WritePeriod
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
void Counter_16_WritePeriod(uint8 period) 
{
    #if(Counter_16_UsingFixedFunction)
        CY_SET_REG16(Counter_16_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(Counter_16_PERIOD_LSB_PTR, period);
    #endif /* (Counter_16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_16_ReadPeriod
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
uint8 Counter_16_ReadPeriod(void) 
{
    #if(Counter_16_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_16_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_16_PERIOD_LSB_PTR));
    #endif /* (Counter_16_UsingFixedFunction) */
}


#if (!Counter_16_UsingFixedFunction)
/*******************************************************************************
* Function Name: Counter_16_WriteCompare
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
void Counter_16_WriteCompare(uint8 compare) \
                                   
{
    #if(Counter_16_UsingFixedFunction)
        CY_SET_REG16(Counter_16_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(Counter_16_COMPARE_LSB_PTR, compare);
    #endif /* (Counter_16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_16_ReadCompare
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
uint8 Counter_16_ReadCompare(void) 
{
    return (CY_GET_REG8(Counter_16_COMPARE_LSB_PTR));
}


#if (Counter_16_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_16_SetCompareMode
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
void Counter_16_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    Counter_16_CONTROL &= ((uint8)(~Counter_16_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    Counter_16_CONTROL |= compareMode;
}
#endif  /* (Counter_16_COMPARE_MODE_SOFTWARE) */


#if (Counter_16_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_16_SetCaptureMode
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
void Counter_16_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    Counter_16_CONTROL &= ((uint8)(~Counter_16_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    Counter_16_CONTROL |= ((uint8)((uint8)captureMode << Counter_16_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (Counter_16_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: Counter_16_ClearFIFO
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
void Counter_16_ClearFIFO(void) 
{

    while(0u != (Counter_16_ReadStatusRegister() & Counter_16_STATUS_FIFONEMP))
    {
        (void)Counter_16_ReadCapture();
    }

}
#endif  /* (!Counter_16_UsingFixedFunction) */


/* [] END OF FILE */

