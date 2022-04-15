// Python.c
// Runs on LM4F120/TM4C123
// Tests the UART0 to implement bidirectional data transfer to and from a
// computer running a python script ser.py.  
// Daniel and Jonathan Valvano
// April 12, 2022

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2020

 Copyright 2022 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#include <stdint.h>
#include "../inc/PLL.h"
#include "../inc/CortexM.h"
#include "../inc/UART0int.h"
#include "../inc/LaunchPad.h"
#include "../inc/SysTickInts.h"
#include "../inc/tm4c123gh6pm.h"
#include "PWM.h" 

// receive commands
// L turns on LED with a numerical value
// l turns off LED
// R starts data log
//    Sends ten measurements of switch data at 10 Hz
int Count;     // number of data points in measurement
int Flag=0;    // semaphore, set when measuremenst done
uint32_t data; // measured data from switches
uint32_t led;  // value from Python script
#define SIZE 10
uint32_t buffer[SIZE];
int main(void){
char cmd;
  PLL_Init(Bus80MHz);       // set system clock to 80 MHz
  UART_Init();              // initialize UART
  LaunchPad_Init();
	PWM0A_Init(40000,5000);
  Count = 0;
  Flag = 0;
  EnableInterrupts();       // Enable interrupts
  while(1){int i;
    cmd = UART_InCharNonBlock();
    if(cmd == 'L'){
      led = UART_InUDec(); // CR terminated
      LaunchPad_Output(led);
    }
    if(cmd == 'l'){
      LaunchPad_Output(0);
    }
    if(cmd == 'R'){
      Count = 0;
      Flag = 0;
      SysTick_Init(8000000); // 10 Hz
    }
    if(Flag){
      Flag = 0; // clear semaphore
      for(i=0; i<SIZE; i++){
        UART_OutUDec(buffer[i]);
        UART_OutChar(' ');
      }
      UART_OutChar(LF);
    }
  }
}
void SysTick_Handler(void){
  if(Count > SIZE-1){
    NVIC_ST_CTRL_R = 0; // stop interrupts
    Flag = 1; // signal semaphore
  }else{
    data = LaunchPad_Input();
    buffer[Count] = data;
    Count++;
  }
}
