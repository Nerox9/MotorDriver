#include "tests.h"

/* Firstly I have tried VS Native Unit Test Project */
/* However it did not work with Cmake appropriately */

void TestStateCreate()
{
	// Create one state State Machine
	StateMachine stateMachine;

	State testState(STATE_OP, STATE_PREOP, BOOT_TCond, BOOT_TOnEntry);

	stateMachine.addState(&testState);

	// Check the state created
	if (STATE_OP == testState.motorState)
	{
		std::cout << "TestStateCreate passed" << std::endl;
	}
	else
	{
		std::cout << "TestStateCreate failed" << std::endl;
	}

}

void TestChecksum()
{
	MotorDriver motorDriver;
	StateMachine stateMachine(&motorDriver);

	uint32_t response = stateMachine.checksumCalc(0x83555a);

	if (0x8c == (response & 0xFF))
	{
		std::cout << "TestChecksum passed" << std::endl;
	}
	else
	{
		std::cout << "TestChecksum failed" << std::endl;
	}
}

void TestTransition()
{
	MotorDriver motorDriver;
	StateMachine stateMachine(&motorDriver);
	
	State testState1(STATE_BOOT, STATE_PREOP, BOOT_TCond, BOOT_TOnEntry);
	State testState2(STATE_PREOP, STATE_SAFEOP, PREOP_TCond, PREOP_TOnEntry);

	stateMachine.addState(&testState1);
	stateMachine.addState(&testState2);

	for (uint8_t i = 0; i < 2; i++)
	{
		stateMachine.run();
		motorDriver.update();
		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	if (STATE_PREOP == stateMachine.getMotorState())
	{
		std::cout << "TestTransition passed" << std::endl;
	}
	else
	{
		std::cout << "TestTransition failed" << std::endl;
	}
}

void TestOnEntry()
{
	MotorDriver motorDriver;
	StateMachine stateMachine(&motorDriver);

	State testState1(STATE_SAFEOP, STATE_OP, SAFEOP_TCond, SAFEOP_TOnEntry);
	State testState2(STATE_OP, STATE_SAFEOP, OP_TCond, OP_TOnEntry);

	stateMachine.addState(&testState1);
	stateMachine.addState(&testState2);

	stateMachine.write(MotorDriverRegisters::MOTOR_VELOCITY_COMMAND, 10);

	for (uint8_t i = 0; i < 5; i++)
	{
		stateMachine.run();
		motorDriver.update();
		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// Read encoder value
	uint32_t encoder = stateMachine.read(MotorDriverRegisters::ENCODER_VALUE);

	if (50 == encoder)
	{
		std::cout << "TestOnEntry passed" << std::endl;
	}
	else
	{
		std::cout << "TestOnEntry failed" << std::endl;
	}
}

void Tests()
{
	TestStateCreate();
	TestChecksum();
	TestTransition();
	TestOnEntry();
}