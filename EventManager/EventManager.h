#ifndef EVENT_CONTROLLER_H
#define EVENT_CONTROLLER_H

//Macro Defines
#define MAX_NUM_EVENTS 256
#define A_REALLY_BIG_NUMBER 100000000

//Declarations
void EM_StartPeriodicEvent(int id_,int period_,void(*fnc_)());
void EM_StartTimerEvent(int id_,int delay_,void(*fnc_)());
void EM_StopPeriodicEvent(int id_);
void EM_Init();
void EM_Run();

#endif