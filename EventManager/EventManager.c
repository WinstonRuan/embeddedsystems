//Author: Robert Dykes
//Date:   10/09/17

//Include
#include "EventManager.h"
#include <pob-eye.h>
#include <timer.h>

//Typedefs and Enums
typedef struct
{
	int id;
	int period;
	int delay;
	void(*fnc)();
	int lastExecutionTime;
} event;

//Local Data
static event s_EventList[MAX_NUM_EVENTS];
static int s_NumEvents = 0;

//Function Definitions
void EM_StartPeriodicEvent(int id_,int period_,void(*fnc_)())
{
	s_EventList[s_NumEvents].id = id_;
	s_EventList[s_NumEvents].period = period_;
	s_EventList[s_NumEvents].delay = -1;
	s_EventList[s_NumEvents].fnc = fnc_;
	s_EventList[s_NumEvents].lastExecutionTime = GetClock();
	s_NumEvents += 1;	
}

void EM_StartTimerEvent(int id_,int delay_,void(*fnc_)())
{
	s_EventList[s_NumEvents].id = id_;
	s_EventList[s_NumEvents].period = -1;
	s_EventList[s_NumEvents].delay = delay_;
	s_EventList[s_NumEvents].fnc = fnc_;
	s_EventList[s_NumEvents].lastExecutionTime = -1;
	s_NumEvents += 1;	
}

void EM_StopPeriodicEvent(int id_)
{
	int i;
	for(i=0;i<s_NumEvents;i++)
	{
		if(	s_EventList[i].id == id_ )
		{
			s_EventList[i].period = A_REALLY_BIG_NUMBER;
			s_EventList[i].lastExecutionTime = 0; //edge case if execution time is -1 which always executes
			s_NumEvents -= 1;	
		}
	}
}

void EM_Init()
{
        StartClock();
}

void EM_Run()
{
	int currentTime = GetClock();
	int i;
	for(i=0;i<s_NumEvents;i++)
	{
			if( currentTime - s_EventList[i].lastExecutionTime > s_EventList[i].period )
			{
				s_EventList[i].fnc();
				s_EventList[i].lastExecutionTime = currentTime;
				currentTime = GetClock();
			}
	}	
}