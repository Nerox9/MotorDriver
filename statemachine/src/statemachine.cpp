#include "statemachine.h"

StateBase::StateBase()
{

}

StateBase::StateBase(MotorState mtrState, MotorState next)
	: motorState(mtrState), nextState(next)
{

}

State::State()
{

}

State::State(MotorState mtrState, MotorState next, const TCond transCond)
	: StateBase(mtrState, next), transitionCond(transCond)
{
	
}

State::~State()
{

}

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

void StateMachine::run()
{
	if (motorDriver == NULL)
	{
		return;
	}

	TCond transitionCondition = currentState->transitionCond;

	// Check Transition Condition of Current State
	if (transitionCondition(currentState, motorDriver))
	{
		// Set current state as next state
		currentState = states.at(currentState->nextState);
	}
	
	
}


// Boot State Transition Condition Function
bool BOOT_TCond(StateBase *state, MotorDriver *motorDriver)
{
	bool retval = false;

	// Send request and get Motor driver current state register response
	uint32_t request = WriteFlag::Read << 31 | MotorDriverRegisters::STATUSWORD << 24 | MotorDriverRegisters::STATUSWORD;
	uint32_t response = motorDriver->transferData(request);

	// Remove checksum from response
	uint32_t driverCurrentState = response >> 8;

	// Check transition condition
	if (state->nextState == driverCurrentState)
		retval = true;
	return retval;
}

// Preop State Transition Condition Function
bool PREOP_TCond(StateBase *state, MotorDriver *motorDriver)
{
	bool retval = false;
	return retval;
}

// SafeOp State Transition Condition Function
bool SAFEOP_TCond(State &state, MotorDriver &motorDriver)
{
	bool retval = false;
	return retval;
}

// Op State Transition Condition Function
bool OP_TCond(State &state, MotorDriver &motorDriver)
{
	bool retval = false;
	return retval;
}