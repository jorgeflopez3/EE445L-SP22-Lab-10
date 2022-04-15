#ifndef __TACHOMETER__
#define __TACHOMETER__
#include <stdint.h>

//------------Tachometer_Init------------
// Initialize input capture for tachometer using Timer0A
// 
// Input: none
// Output: none
void Tachometer_Init(void);

//------------Tachometer_Period------------
// Output the current period measured by the tachometer
// 
// Input: none
// Output: current period
uint32_t Tachometer_Period(void);

#endif