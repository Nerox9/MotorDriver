#include "statemachine.h"

/*********/
/* STATE */
/*********/
State::State(MotorState mtrState, MotorState next, const TCond transCond, const TOnEntry onEntry)
	: motorState(mtrState), nextState(next), transitionCond(transCond), onEntry(onEntry)
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

// Get current state as motor state
MotorState StateMachine::getMotorState()
{
	return currentState->motorState;
}

// Checksum Calculator
uint8_t StateMachine::checksumCalc(uint32_t data)
{
	uint8_t checksum = 0;
	// Calculate checksum
	for (uint8_t i = 0; i < sizeof(data) / sizeof(uint8_t); ++i)
	{
		checksum ^= (data >> i * 8) & 0xFF;
	}
	return checksum;
}

// Write to motor driver
uint32_t StateMachine::write(MotorDriverRegisters mtrDriverReg, uint16_t value)
{
	uint32_t request = WriteFlag::Write << 31 | mtrDriverReg << 24 |value << 8;
	
	// Calculate checksum
	uint8_t checksum = checksumCalc(request);
	request |= checksum;

	// Send data and get response
	uint32_t response = motorDriver->transferData(request);
	return response;
}

// Read from motor driver
uint32_t StateMachine::read(MotorDriverRegisters mtrDriverReg)
{
	uint32_t request = WriteFlag::Read << 31 | mtrDriverReg << 24;

	// Calculate checksum
	uint8_t checksum = checksumCalc(request);
	request |= checksum;

	// Send data and get response
	uint32_t response = motorDriver->transferData(request);
	response = response >> 8;
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
		// Call onExit function of current state
		//currentState->onExit(this);
		// Set current state as next state
		currentState = states.at(currentState->nextState);
		// Call onEntry function of current state
		currentState->onEntry(this);
	}
	
}
/************************/
/* END OF STATE MACHINE */
/************************/

