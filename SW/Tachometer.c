// external signal connected to PB6 (T0CCP0) (trigger on rising edge)
#include "Tachometer.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
uint32_t Period;                       // 24-bit, 12.5 ns units
uint32_t static First;                 // Timer0A first edge, 12.5 ns units
int32_t Done;   // mailbox status set each rising
uint32_t MotorRps = 0;

void Tachometer_Init(void){
	SYSCTL_RCGCTIMER_R |= 0x02;          // activate timer1
  SYSCTL_RCGCGPIO_R |= 0x02;           // activate port B
  First = 0;                           // first will be wrong
  Done = 0;                            // set on subsequent
  GPIO_PORTB_DIR_R   &= ~0x10;         // make PB4 input
  GPIO_PORTB_AFSEL_R |= 0x10;          // enable alt funct on PB4
  GPIO_PORTB_DEN_R   |= 0x10;          // configure PB4 as T0CCP0
  GPIO_PORTB_PCTL_R   = (GPIO_PORTB_PCTL_R
                       & 0xFFF0FFFF)
                       + 0x00070000;
  TIMER1_CTL_R       &= ~0x00000001;   // disable timer1A during setup
  TIMER1_CFG_R        =  0x00000004;   // configure for 16-bit capture mode
  TIMER1_TAMR_R       =  0x00000007;   // configure for rising edge event
  TIMER1_CTL_R       &= ~0x0000000C;   // rising edge
  TIMER1_TAILR_R      =  0x0000FFFF;   // start value
  TIMER1_TAPR_R       =  0xFF;         // activate prescale, creating 24-bit 
  TIMER1_IMR_R       |=  0x00000004;   // enable capture match interrupt
  TIMER1_ICR_R        =  0x00000004;   // clear timer1A capture match flag
  TIMER1_CTL_R       |=  0x00000001;   // timer0A 24-b, +edge, interrupts
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|(2<<13); // priority 
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
  EnableInterrupts();                  
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = 0x00000004; // acknowledge timer1A
  Period = (First - TIMER1_TAR_R)&0x00FFFFFF; 
	if (Period > 83333) {
		MotorRps = 6666666 / Period;
	}
  First = TIMER1_TAR_R;      // setup for next
  Done = 1;                  // set semaphore
}

uint32_t Tachometer_Period(void){
	return Period;
}

uint32_t Tachometer_RPS() {
	return MotorRps;
}