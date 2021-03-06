#include <stdint.h>
#include "Controller.h" 
#include "Tachometer.h"
#include "PWM.h" 
#include "../inc/tm4c123gh6pm.h"

#define PF1   (*((volatile uint32_t *)0x40025008)) // RED LED

	
uint32_t desired; 
//uint32_t period;     		// 24-bit, 12.5 ns units
//uint32_t Period;     		// 24-bit, 12.5 ns units
uint32_t rps;      		// motor speed in 1 rps
int32_t E;           		// speed error in 1 rps
int32_t U;           		// duty cycle 40 to 39960
uint8_t Count10ms;      
int32_t P;             // proportional component of controller
int32_t I;             // integral portion of controller 
int32_t Kp1; 
int32_t Kp2; 
int32_t Ki1; 
int32_t Ki2; 
int32_t D; 
int32_t MotorSpeed;    
uint8_t NewData;

void Controller_Init() {	
	PWM0A_Init(40000,36000);      // duty cycle 
	desired = 500; 
	Kp1 = 75; 
	Kp2 = 337; 
	Ki1 = 75;
	Ki2 = 337; 
	Timer2A_Init(800000,2);
}

// ***************** Timer2A_Init ****************
// Activate Timer2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
//          priority 0 (highest) to 7 (lowest)
// Outputs: none
void Timer2A_Init( uint32_t period, uint32_t priority){
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|(priority<<29); // priority  
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A

}

void Timer2A_Handler(void){
		TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
		PF1 = 0x02;
	
		Count10ms++;
		if(Count10ms>3){                // stopped motor
			rps = 0;                      // set rps to zero
			Count10ms = 0;
		}
		
		rps = Tachometer_RPS();
    E = desired-rps;                // Determine ERROR
    MotorSpeed = rps;             // Set the Motor Speed
		NewData = 1;
    P  =  (Kp1 * E)/Kp2;          // Proportional terms from Blynk    
    if(P <  300) P = 300;         // Minimum PWM output
    if(P >39900) P = 39900;       // 3000 to 39900
    
    I  = I + (Ki1 * E)/Ki2;       // Integral terms from Blynk -- SUM(KiDt)   
    if(I <  300) I = 300;         // Minimum PWM output = 300
    if(I >39900) I = 39900;       // Maximum PWM output = 39900

    U   = P + I;              // Calculate the actuator value
    if(U < 7000)  U=7000;           // Minimum PWM output
    if(U >39900) U=39900;         // 3000 to 39900

    PWM0A_Duty(U);                // Send to PWM
		PF1 = 0x00;
}  

void Controller_SetSpeed(int32_t speed) {
	desired = speed;
}

void Controller_SetPConsts(int32_t kp1, int32_t kp2) {
	Kp1 = kp1;
	Kp2 = kp2;
}

void Controller_SetIConsts(int32_t ki1, int32_t ki2) {
	Ki1 = ki1;
	Ki2 = ki2;
}

void Controller_SetCurrentRps(uint32_t currentRps) {
	rps = currentRps;
}

uint32_t Controller_GetRps() {
	return MotorSpeed;
}

int32_t Controller_GetE() {
	return E;
}

int32_t Controller_GetU() {
	return U;
}

int32_t Controller_GetDesiredSpeed() {
	return desired;
}
