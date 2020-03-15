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

class StateMachineBase
{
public:
	// Write to motor driver
	virtual	uint32_t write(MotorDriverRegisters, uint16_t) = 0;
	// Read from motor driver
	virtual uint32_t read(MotorDriverRegisters) = 0;
	// Get next state as motor state
	virtual MotorState getNextMotorState() = 0;
};

typedef function<bool(StateMachineBase*)> TCond; // Transition Condition Function template
typedef function<void(StateMachineBase*)> TOnEntry; // On Entry Function template


// Transition Condition Functions
bool BOOT_TCond(StateMachineBase*);
bool PREOP_TCond(StateMachineBase*);
bool SAFEOP_TCond(StateMachineBase*);
bool OP_TCond(StateMachineBase*);

// On Entry Functions
void BOOT_TOnEntry(StateMachineBase*);
void PREOP_TOnEntry(StateMachineBase*);
void SAFEOP_TOnEntry(StateMachineBase*);
void OP_TOnEntry(StateMachineBase*);

class State
{
public:
	MotorState motorState;		// MotorState
	MotorState nextState;		// Next MotorState
	const TCond transitionCond;	// Transition Condition Function
	const TOnEntry onEntry;		// OnEntry Function

	// Constructor
	State(MotorState, MotorState, const TCond, const TOnEntry);
	// Deconstructor
	~State();
};

class StateMachine : public StateMachineBase
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
	// Get current state as motor state
	MotorState getMotorState();
	// Get next state as motor state
	MotorState getNextMotorState();

	// Checksum Calculator
	uint8_t checksumCalc(uint32_t);
	// Write to motor driver
	uint32_t write(MotorDriverRegisters, uint16_t);
	// Read from motor driver
	uint32_t read(MotorDriverRegisters);

	// Run transition
	void run();
};

#endif /* STATEMACHINE_H */
