/*******************************************************************************
* File Name: Counter_20.c  
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

#include "Counter_20.h"

uint8 Counter_20_initVar = 0u;


/*******************************************************************************
* Function Name: Counter_20_Init
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
void Counter_20_Init(void) 
{
        #if (!Counter_20_UsingFixedFunction && !Counter_20_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!Counter_20_UsingFixedFunction && !Counter_20_ControlRegRemoved) */
        
        #if(!Counter_20_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 Counter_20_interruptState;
        #endif /* (!Counter_20_UsingFixedFunction) */
        
        #if (Counter_20_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            Counter_20_CONTROL &= Counter_20_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                Counter_20_CONTROL2 &= ((uint8)(~Counter_20_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Counter_20_CONTROL3 &= ((uint8)(~Counter_20_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (Counter_20_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                Counter_20_CONTROL |= Counter_20_ONESHOT;
            #endif /* (Counter_20_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            Counter_20_CONTROL2 |= Counter_20_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            Counter_20_RT1 &= ((uint8)(~Counter_20_RT1_MASK));
            Counter_20_RT1 |= Counter_20_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            Counter_20_RT1 &= ((uint8)(~Counter_20_SYNCDSI_MASK));
            Counter_20_RT1 |= Counter_20_SYNCDSI_EN;

        #else
            #if(!Counter_20_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = Counter_20_CONTROL & ((uint8)(~Counter_20_CTRL_CMPMODE_MASK));
            Counter_20_CONTROL = ctrl | Counter_20_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = Counter_20_CONTROL & ((uint8)(~Counter_20_CTRL_CAPMODE_MASK));
            
            #if( 0 != Counter_20_CAPTURE_MODE_CONF)
                Counter_20_CONTROL = ctrl | Counter_20_DEFAULT_CAPTURE_MODE;
            #else
                Counter_20_CONTROL = ctrl;
            #endif /* 0 != Counter_20_CAPTURE_MODE */ 
            
            #endif /* (!Counter_20_ControlRegRemoved) */
        #endif /* (Counter_20_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!Counter_20_UsingFixedFunction)
            Counter_20_ClearFIFO();
        #endif /* (!Counter_20_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        Counter_20_WritePeriod(Counter_20_INIT_PERIOD_VALUE);
        #if (!(Counter_20_UsingFixedFunction && (CY_PSOC5A)))
            Counter_20_WriteCounter(Counter_20_INIT_COUNTER_VALUE);
        #endif /* (!(Counter_20_UsingFixedFunction && (CY_PSOC5A))) */
        Counter_20_SetInterruptMode(Counter_20_INIT_INTERRUPTS_MASK);
        
        #if (!Counter_20_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)Counter_20_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            Counter_20_WriteCompare(Counter_20_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Counter_20_interruptState = CyEnterCriticalSection();
            
            Counter_20_STATUS_AUX_CTRL |= Counter_20_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(Counter_20_interruptState);
            
        #endif /* (!Counter_20_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_20_Enable
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
void Counter_20_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Counter_20_UsingFixedFunction)
        Counter_20_GLOBAL_ENABLE |= Counter_20_BLOCK_EN_MASK;
        Counter_20_GLOBAL_STBY_ENABLE |= Counter_20_BLOCK_STBY_EN_MASK;
    #endif /* (Counter_20_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!Counter_20_ControlRegRemoved || Counter_20_UsingFixedFunction)
        Counter_20_CONTROL |= Counter_20_CTRL_ENABLE;                
    #endif /* (!Counter_20_ControlRegRemoved || Counter_20_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: Counter_20_Start
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
*  Counter_20_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Counter_20_Start(void) 
{
    if(Counter_20_initVar == 0u)
    {
        Counter_20_Init();
        
        Counter_20_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    Counter_20_Enable();        
}


/*******************************************************************************
* Function Name: Counter_20_Stop
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
void Counter_20_Stop(void) 
{
    /* Disable Counter */
    #if(!Counter_20_ControlRegRemoved || Counter_20_UsingFixedFunction)
        Counter_20_CONTROL &= ((uint8)(~Counter_20_CTRL_ENABLE));        
    #endif /* (!Counter_20_ControlRegRemoved || Counter_20_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Counter_20_UsingFixedFunction)
        Counter_20_GLOBAL_ENABLE &= ((uint8)(~Counter_20_BLOCK_EN_MASK));
        Counter_20_GLOBAL_STBY_ENABLE &= ((uint8)(~Counter_20_BLOCK_STBY_EN_MASK));
    #endif /* (Counter_20_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_20_SetInterruptMode
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
void Counter_20_SetInterruptMode(uint8 interruptsMask) 
{
    Counter_20_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: Counter_20_ReadStatusRegister
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
uint8   Counter_20_ReadStatusRegister(void) 
{
    return Counter_20_STATUS;
}


#if(!Counter_20_ControlRegRemoved)
/*******************************************************************************
* Function Name: Counter_20_ReadControlRegister
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
uint8   Counter_20_ReadControlRegister(void) 
{
    return Counter_20_CONTROL;
}


/*******************************************************************************
* Function Name: Counter_20_WriteControlRegister
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
void    Counter_20_WriteControlRegister(uint8 control) 
{
    Counter_20_CONTROL = control;
}

#endif  /* (!Counter_20_ControlRegRemoved) */


#if (!(Counter_20_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: Counter_20_WriteCounter
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
void Counter_20_WriteCounter(uint8 counter) \
                                   
{
    #if(Counter_20_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (Counter_20_GLOBAL_ENABLE & Counter_20_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        Counter_20_GLOBAL_ENABLE |= Counter_20_BLOCK_EN_MASK;
        CY_SET_REG16(Counter_20_COUNTER_LSB_PTR, (uint16)counter);
        Counter_20_GLOBAL_ENABLE &= ((uint8)(~Counter_20_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(Counter_20_COUNTER_LSB_PTR, counter);
    #endif /* (Counter_20_UsingFixedFunction) */
}
#endif /* (!(Counter_20_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: Counter_20_ReadCounter
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
uint8 Counter_20_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(Counter_20_UsingFixedFunction)
		(void)CY_GET_REG16(Counter_20_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(Counter_20_COUNTER_LSB_PTR_8BIT);
	#endif/* (Counter_20_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(Counter_20_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_20_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_20_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_20_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_20_ReadCapture
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
uint8 Counter_20_ReadCapture(void) 
{
    #if(Counter_20_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_20_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_20_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_20_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_20_WritePeriod
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
void Counter_20_WritePeriod(uint8 period) 
{
    #if(Counter_20_UsingFixedFunction)
        CY_SET_REG16(Counter_20_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(Counter_20_PERIOD_LSB_PTR, period);
    #endif /* (Counter_20_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_20_ReadPeriod
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
uint8 Counter_20_ReadPeriod(void) 
{
    #if(Counter_20_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_20_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_20_PERIOD_LSB_PTR));
    #endif /* (Counter_20_UsingFixedFunction) */
}


#if (!Counter_20_UsingFixedFunction)
/*******************************************************************************
* Function Name: Counter_20_WriteCompare
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
void Counter_20_WriteCompare(uint8 compare) \
                                   
{
    #if(Counter_20_UsingFixedFunction)
        CY_SET_REG16(Counter_20_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(Counter_20_COMPARE_LSB_PTR, compare);
    #endif /* (Counter_20_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_20_ReadCompare
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
uint8 Counter_20_ReadCompare(void) 
{
    return (CY_GET_REG8(Counter_20_COMPARE_LSB_PTR));
}


#if (Counter_20_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_20_SetCompareMode
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
void Counter_20_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    Counter_20_CONTROL &= ((uint8)(~Counter_20_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    Counter_20_CONTROL |= compareMode;
}
#endif  /* (Counter_20_COMPARE_MODE_SOFTWARE) */


#if (Counter_20_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_20_SetCaptureMode
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
void Counter_20_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    Counter_20_CONTROL &= ((uint8)(~Counter_20_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    Counter_20_CONTROL |= ((uint8)((uint8)captureMode << Counter_20_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (Counter_20_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: Counter_20_ClearFIFO
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
void Counter_20_ClearFIFO(void) 
{

    while(0u != (Counter_20_ReadStatusRegister() & Counter_20_STATUS_FIFONEMP))
    {
        (void)Counter_20_ReadCapture();
    }

}
#endif  /* (!Counter_20_UsingFixedFunction) */


/* [] END OF FILE */

