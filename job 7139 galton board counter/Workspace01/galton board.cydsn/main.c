/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>

char TransmitBuffer[255]="";
int rx_flag=0;
int datain=0;

int count1=0;
int count2=0;
int count3=0;
int count4=0;
int count5=0;
int count6=0;
int count7=0;
int count8=0;
int count9=0;
int count10=0;
int count11=0;
int count12=0;
int count13=0;
int total=0;

CY_ISR(MyISR_1)
{
 count1++; 
 isr_1_sensor_ClearPending();
}

CY_ISR(MyISR_2)
{
 count2++; 
 isr_2_sensor_ClearPending();
}
CY_ISR(MyISR_3)
{
 count3++; 
 isr_3_sensor_ClearPending();
}
CY_ISR(MyISR_4)
{
 count4++; 
 isr_4_sensor_ClearPending();
}

CY_ISR(MyISR_5)
{
 count5++; 
 isr_5_sensor_ClearPending();
}

CY_ISR(MyISR_6)
{
 count6++; 
 isr_6_sensor_ClearPending();

}

CY_ISR(MyISR_7)
{
 count7++; 
 isr_7_sensor_ClearPending();
}
CY_ISR(MyISR_8)
{
 count8++; 
 isr_8_sensor_ClearPending();
}
CY_ISR(MyISR_9)
{
 count9++; 
 isr_9_sensor_ClearPending();
}

CY_ISR(MyISR_10)
{
 count10++; 
 isr_10_sensor_ClearPending();
}

CY_ISR(MyISR_11)
{
 count11++; 
 isr_11_sensor_ClearPending();
}

CY_ISR(MyISR_12)
{
 count12++; 
 isr_12_sensor_ClearPending();
}
CY_ISR(MyISR_13)
{
 count13++; 
 isr_13_sensor_ClearPending();
}

CY_ISR(MyUART_RX_INTERRUPT)
{
 datain=UART_1_GetChar();
 rx_flag=1; 
 }

void Clear_counters(void)
{
    count1=0;
    count2=0;
    count3=0;
    count4=0;
    count5=0;
    count6=0;
    count7=0;
    count8=0;
    count9=0;
    count10=0;
    count11=0;
    count12=0;
    count13=0;
    total=0;
}
void Send_Couters(void)
{
/* Send out the data */ 
total=count1+count2+count3+count4+count5+count6+count7+count8+count9+count10+count11+count12+count13;   
sprintf(TransmitBuffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d > %d\r\n", count1,count2,count3,count4,count5,count6,count7,count8,count9,count10,count11,count12,count13,total);
UART_1_PutString(TransmitBuffer);   
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
   
    isr_1_sensor_StartEx(MyISR_1);
    isr_2_sensor_StartEx(MyISR_2);
    isr_3_sensor_StartEx(MyISR_3);
    isr_4_sensor_StartEx(MyISR_4);
    isr_5_sensor_StartEx(MyISR_5);
    isr_6_sensor_StartEx(MyISR_6);
    isr_7_sensor_StartEx(MyISR_7);
    isr_8_sensor_StartEx(MyISR_8);
    isr_9_sensor_StartEx(MyISR_9);
    isr_10_sensor_StartEx(MyISR_10);
    isr_11_sensor_StartEx(MyISR_11);
    isr_12_sensor_StartEx(MyISR_12);
    isr_13_sensor_StartEx(MyISR_13);
    serial_in_buff_StartEx(MyUART_RX_INTERRUPT);
   
    UART_1_Start();
    UART_1_PutString("Galton board couter\r\n"); 
   
    
    for(;;)
    {
        /* Place your application code here. */
        
    if (rx_flag==1)
        {
        switch(datain)
        {
         case 'C':
         Clear_counters();
         break;
         
         case 'S':
         Send_Couters();
         break;   
         }
        rx_flag=0;
        }
    }
}

/* [] END OF FILE */
