#include "statemachine.h"

/*********/
/* STATE */
/*********/
State::State(MotorState mtrState, MotorState next, const TCond transCond)
	: motorState(mtrState), nextState(next), transitionCond(transCond)
{
	
}

State::~State()
{

}
/****************/
/* END OF STATE */
/****************/

/*****************/
/* STATE MACHINE */
/*****************/

StateMachine::StateMachine(MotorDriver *mtrDriver)
	: motorDriver(mtrDriver)
{

}

StateMachine::StateMachine()
{

}

StateMachine::~StateMachine()
{

}

// Add a state to state map
void StateMachine::addState(State *state)
{
	if (states.empty())
	{
		currentState = state;
	}

	states.emplace(state->motorState, state);
}

// Get next state as motor state
MotorState StateMachine::getNextMotorState()
{
	return currentState->nextState;
}

// Write to motor driver
uint32_t StateMachine::Write(MotorDriverRegisters mtrDriverReg, uint16_t value)
{
	uint32_t request = WriteFlag::Write << 31 | mtrDriverReg << 24 |value << 8;
	
	// Calculate checksum
	for (uint8_t i = 0; i < sizeof(request)/sizeof(uint8_t); ++i)
	{
		uint8_t checksum = request >> i * 8;
		checksum ^= request & 0xFF;
		request |= checksum;
	}

	uint32_t response = motorDriver->transferData(request);
	return response;
}

// Read from motor driver
uint32_t StateMachine::Read(MotorDriverRegisters mtrDriverReg)
{
	uint32_t request = WriteFlag::Read << 31 | mtrDriverReg << 24;

	// Calculate checksum
	for (uint8_t i = 1; i < sizeof(request) / sizeof(uint8_t); ++i)
	{
		uint8_t checksum = request >> i * 8;
		checksum ^= request & 0xFF;
		request |= checksum;
	}

	uint32_t response = motorDriver->transferData(request);
	return response;
}

void StateMachine::run()
{
	if (motorDriver == NULL)
	{
		return;
	}

	auto transitionCondition = currentState->transitionCond;

	// Check Transition Condition of Current State
	if (transitionCondition(this))
	{
		// Set current state as next state
		currentState = states.at(currentState->nextState);
	}
	
}
/************************/
/* END OF STATE MACHINE */
/************************/

