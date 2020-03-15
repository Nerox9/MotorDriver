#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <map>
#include <functional>

#include "motordriver.h"

using namespace std;

enum WriteFlag
{
	Read = 0,
	Write = 1
};

class StateBase
{
public:
	MotorState motorState;		// MotorState
	MotorState nextState;		// Next MotorState

	StateBase();
	StateBase(MotorState, MotorState);
};

typedef function<bool(StateBase*, MotorDriver*)> TCond; // Transition Condition Function

// Transition Condition Functions
bool BOOT_TCond(StateBase*, MotorDriver*);
bool PREOP_TCond(StateBase*, MotorDriver*);
bool SAFEOP_TCond(StateBase*, MotorDriver*);
bool OP_TCond(StateBase*, MotorDriver*);

class State : public StateBase
{
public:
	const TCond transitionCond;	// Transition Condition Function

	//
	State();
	// Constructor
	State(MotorState, MotorState, const TCond);
	// Deconstructor
	~State();
};

class StateMachine
{
	// State map includes all states
	map<MotorState, State*> states;
	// Active state
	State *currentState;

public:
	// MotorDriver for transitions
	MotorDriver *motorDriver;

	// Constructor without transition
	StateMachine();
	// Constructor
	StateMachine(MotorDriver *mtrDriver);
	// Deconstructor
	~StateMachine();
	// Add a state to state map
	void addState(State *state);
	// Run transition
	void run();
};

#endif /* STATEMACHINE_H */
