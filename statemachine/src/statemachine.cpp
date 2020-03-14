#include "statemachine.h"

State::State(MotorState motorState)
{
	this->motorState = motorState;
}

State::~State()
{

}


void StateMachine::addState(State state)
{
	states.emplace(state.motorState, state);
}