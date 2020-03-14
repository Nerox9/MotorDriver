#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "motordriver.h"
#include <map>

using namespace std;

class State
{
public:
	MotorState motorState;
	State(MotorState);
	~State();
};

class StateMachine 
{
	map<MotorState, State> states;

public:
	void addState(State state);
};



#endif /* STATEMACHINE_H */
