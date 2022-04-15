#include <stdint.h>

// ***************** Timer2A_Init ****************
// Activate Timer2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
//          priority 0 (highest) to 7 (lowest)
// Outputs: none
void Timer2A_Init( uint32_t period, uint32_t priority); 

void Timer2A_Handler(void); 

void Controller_Init();

void Controller_SetSpeed(int32_t speed);

void Controller_SetPConsts(int32_t kp1, int32_t kp2);

void Controller_SetIConsts(int32_t ki1, int32_t ki2);

void Controller_SetCurrentRps(uint32_t currentRps);

uint32_t Controller_GetRps();

// Global Count Var
extern uint8_t Count10ms; 