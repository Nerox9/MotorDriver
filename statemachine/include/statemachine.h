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

enum TransitionStates
{
	noTransition = 0,
	transOnEntry,
	transOnExit
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
typedef function<void(StateMachineBase*)> TOnExit; // On Exit Function template
typedef function<uint16_t(StateMachineBase*)> TAction; // Action Function template


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

// On Exit Functions
void BOOT_TOnExit(StateMachineBase*);
void PREOP_TOnExit(StateMachineBase*);
void SAFEOP_TOnExit(StateMachineBase*);
void OP_TOnExit(StateMachineBase*);

// Action Functions
uint16_t BOOT_TAction(StateMachineBase*);
uint16_t PREOP_TAction(StateMachineBase*);
uint16_t SAFEOP_TAction(StateMachineBase*);
uint16_t OP_TAction(StateMachineBase*);

class State
{
public:
	MotorState motorState;		// MotorState
	MotorState nextState;		// Next MotorState
	const TCond transitionCond;	// Transition Condition Function
	const TOnEntry onEntry;		// OnEntry Function
	const TOnExit onExit;		// OnExit Function
	const TAction action;		// Action Function

	// Constructor
	State(MotorState, MotorState, const TCond);
	State(MotorState, MotorState, const TCond, const TOnEntry);
	State(MotorState, MotorState, const TCond, const TOnEntry, const TOnExit);
	State(MotorState, MotorState, const TCond, const TOnEntry, const TOnExit, const TAction);
	// Destructor
	//~State();
};

class StateMachine : public StateMachineBase
{
	// State map includes all states
	map<MotorState, State*> states;
	// Active state
	State *currentState;
	// Indicates transition state
	TransitionStates activeTransition = TransitionStates::noTransition;

public:
	// MotorDriver for transitions
	MotorDriver *motorDriver;

	// Constructor without transition
	StateMachine();
	// Constructor
	StateMachine(MotorDriver *mtrDriver);
	// Destructor
	//~StateMachine();

	// Add a state to state map
	void addState(State *state);
	// Get current state as motor state
	MotorState getMotorState();
	// Get next state as motor state
	MotorState getNextMotorState();
	// Get Active Transition Flag
	bool getTransition();
	// Get Current State Pointer
	State* getCurrentState();

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
