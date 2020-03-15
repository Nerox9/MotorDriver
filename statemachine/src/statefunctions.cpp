#include "statemachine.h"

/**********************************/
/* TRANSITION CONDITION FUNCTIONS */
/**********************************/

// Boot State Transition Condition Function
bool BOOT_TCond(StateMachineBase *stateMachine)
{
	bool retval = false;

	// Send request and get Motor driver current state register response
	uint32_t response = stateMachine->read(MotorDriverRegisters::STATUSWORD);

	// Remove checksum from response
	uint32_t driverCurrentState = response >> 8;

	// Check transition condition
	if (stateMachine->getNextMotorState() == driverCurrentState)
		retval = true;
	return retval;
}

// Preop State Transition Condition Function
bool PREOP_TCond(StateMachineBase *stateMachine)
{
	// return true immediately
	bool retval = true;
	return retval;
}

// SafeOp State Transition Condition Function
bool SAFEOP_TCond(StateMachineBase *stateMachine)
{
	bool retval = false;

	// Send request and get Motor driver current fault register response
	uint32_t response = stateMachine->read(MotorDriverRegisters::FAULT);

	// Remove checksum from response
	uint32_t faultValue = response >> 8;

	// Check transition condition
	if (faultValue != 1)
		retval = true;

	return retval;
}

// Op State Transition Condition Function
bool OP_TCond(StateMachineBase *stateMachine)
{
	bool retval = false;

	// Send request and get Motor driver current fault register response
	uint32_t response = stateMachine->read(MotorDriverRegisters::FAULT);

	// Remove checksum from response
	uint32_t faultValue = response >> 8;

	// Check transition condition
	if (faultValue == 1)
		retval = true;

	return retval;
}

/*****************************************/
/* END OF TRANSITION CONDITION FUNCTIONS */
/*****************************************/

/**********************/
/* ON ENTRY FUNCTIONS */
/**********************/

// Boot state onEntry Function
void BOOT_TOnEntry(StateMachineBase* stateMachine)
{

}

// PreOp state onEntry Function
void PREOP_TOnEntry(StateMachineBase* stateMachine)
{

}

// SafeOp state onEntry Function
void SAFEOP_TOnEntry(StateMachineBase* stateMachine)
{
	// Write 0 to encoder register
	uint32_t response = stateMachine->write(MotorDriverRegisters::ENCODER_VALUE, 0x00);
}

// Op state onEntry Function
void OP_TOnEntry(StateMachineBase* stateMachine)
{
	// Enable output
	uint32_t response = stateMachine->write(MotorDriverRegisters::OUTPUT_ENABLE, 0x01);
}

/*****************************/
/* END OF ON ENTRY FUNCTIONS */
/*****************************/