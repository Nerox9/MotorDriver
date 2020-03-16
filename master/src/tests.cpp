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
	
	State testState1(STATE_BOOT, STATE_PREOP, BOOT_TCond);
	State testState2(STATE_PREOP, STATE_SAFEOP, PREOP_TCond);

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

	State testState1(STATE_BOOT, STATE_PREOP, BOOT_TCond);
	State testState2(STATE_PREOP, STATE_SAFEOP, PREOP_TCond);
	State testState3(STATE_SAFEOP, STATE_OP, SAFEOP_TCond, SAFEOP_TOnEntry, SAFEOP_TOnExit, SAFEOP_TAction);
	State testState4(STATE_OP, STATE_SAFEOP, OP_TCond, OP_TOnEntry, OP_TOnExit, OP_TAction);

	stateMachine.addState(&testState1);
	stateMachine.addState(&testState2);
	stateMachine.addState(&testState3);
	stateMachine.addState(&testState4);

	// Change State from BOOT to OP
	for (uint8_t i = 0; i < 4; i++)
	{
		stateMachine.run();
		motorDriver.update();

		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// Set velocity to 10 to determine encoder value
	stateMachine.write(MotorDriverRegisters::MOTOR_VELOCITY_COMMAND, 10);
	motorDriver.update();
	// Sleep for 100ms to simulate cyclic control
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	// Change State from SAFEOP to OP and encoder counts up to 50
	for (uint8_t i = 0; i < 5; i++)
	{
		stateMachine.run();
		motorDriver.update();

		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// Change State from OP to SAFEOP and reset encoder
	stateMachine.write(MotorDriverRegisters::FAULT, 1);
	for (uint8_t i = 0; i < 1; i++)
	{
		stateMachine.run();
		motorDriver.update();

		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// Read encoder value
	uint32_t encoder = stateMachine.read(MotorDriverRegisters::ENCODER_VALUE);

	// Change State from SAFEOP to OP and count
	stateMachine.write(MotorDriverRegisters::FAULT, 0);
	for (uint8_t i = 0; i < 2; i++)
	{
		stateMachine.run();
		motorDriver.update();

		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// Read encoder value
	encoder = stateMachine.read(MotorDriverRegisters::ENCODER_VALUE);

	// It checks encoder is reset on SafeOp state transition
	// If its value is higher than 50, than it does not reset the encoder value
	// 
	if (20 == encoder)
	{
		std::cout << "TestOnEntry passed" << std::endl;
	}
	else
	{
		std::cout << "TestOnEntry failed" << std::endl;
	}
}

void TestOnExit()
{
	MotorDriver motorDriver;
	StateMachine stateMachine(&motorDriver);

	State testState1(STATE_BOOT, STATE_PREOP, BOOT_TCond);
	State testState2(STATE_PREOP, STATE_SAFEOP, PREOP_TCond);
	State testState3(STATE_SAFEOP, STATE_OP, SAFEOP_TCond, SAFEOP_TOnEntry, SAFEOP_TOnExit, SAFEOP_TAction);
	State testState4(STATE_OP, STATE_SAFEOP, OP_TCond, OP_TOnEntry, OP_TOnExit, OP_TAction);

	stateMachine.addState(&testState1);
	stateMachine.addState(&testState2);
	stateMachine.addState(&testState3);
	stateMachine.addState(&testState4);

	// Change State from BOOT to OP
	for (uint8_t i = 0; i < 4; i++)
	{
		stateMachine.run();
		motorDriver.update();
		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	stateMachine.write(MotorDriverRegisters::FAULT, 1);
	// Change State from OP to SAFEOP
	for (uint8_t i = 0; i < 2; i++)
	{
		stateMachine.run();
		motorDriver.update();
		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// Read output enable register
	uint32_t output = stateMachine.read(MotorDriverRegisters::OUTPUT_ENABLE);

	// Check the output disable
	if (0 == output)
	{
		std::cout << "TestOnExit passed" << std::endl;
	}
	else
	{
		std::cout << "TestOnExit failed" << std::endl;
	}
}

void TestAction()
{
	MotorDriver motorDriver;
	StateMachine stateMachine(&motorDriver);

	State testState1(STATE_BOOT, STATE_PREOP, BOOT_TCond);
	State testState2(STATE_PREOP, STATE_SAFEOP, PREOP_TCond);
	State testState3(STATE_SAFEOP, STATE_OP, SAFEOP_TCond, SAFEOP_TOnEntry, SAFEOP_TOnExit, SAFEOP_TAction);
	State testState4(STATE_OP, STATE_SAFEOP, OP_TCond, OP_TOnEntry, OP_TOnExit, OP_TAction);

	stateMachine.addState(&testState1);
	stateMachine.addState(&testState2);
	stateMachine.addState(&testState3);
	stateMachine.addState(&testState4);

	// Change State from BOOT to OP
	for (uint8_t i = 0; i < 5; i++)
	{
		stateMachine.run();
		motorDriver.update();
		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// Read encoder value
	uint16_t encoder = stateMachine.getCurrentState()->action(&stateMachine);

	// Check the encoder value is true
	if (10 == encoder)
	{
		std::cout << "TestAction passed" << std::endl;
	}
	else
	{
		std::cout << "TestAction failed" << std::endl;
	}
}

void TestInvalidCommand()
{
	MotorDriver motorDriver;
	StateMachine stateMachine(&motorDriver);
	
	State testState1(STATE_BOOT, STATE_PREOP, BOOT_TCond);
	State testState2(STATE_PREOP, STATE_SAFEOP, PREOP_TCond);
	State testState3(STATE_SAFEOP, STATE_OP, OP_TCond, OP_TOnEntry, OP_TOnExit);

	stateMachine.addState(&testState1);
	stateMachine.addState(&testState2);
	stateMachine.addState(&testState3);

	// Set state as SafeOp
	for (uint8_t i = 0; i < 3; i++)
	{
		stateMachine.run();
		motorDriver.update();

		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}


	// Send invalid checksum
	uint32_t request = WriteFlag::Write << 31 | MotorDriverRegisters::STATUSWORD << 24 | 0xFFAA << 8;
	uint32_t response = motorDriver.transferData(request);

	// Call update to simulate motor driver doing work
	motorDriver.update();

	uint32_t fault = stateMachine.read(MotorDriverRegisters::FAULT);

	// Sleep for 100ms to simulate cyclic control
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	if (1 == fault)
	{
		std::cout << "TestInvalidCommand passed" << std::endl;
	}
	else
	{
		std::cout << "TestInvalidCommand failed" << std::endl;
	}
}

void TestInvalidStateChange()
{
	MotorDriver motorDriver;
	StateMachine stateMachine(&motorDriver);

	State testState1(STATE_BOOT, STATE_OP, PREOP_TCond);
	State testState2(STATE_OP, STATE_SAFEOP, OP_TCond);
	State testState3(STATE_SAFEOP, STATE_OP, OP_TCond, OP_TOnEntry, OP_TOnExit);

	stateMachine.addState(&testState1);
	stateMachine.addState(&testState2);
	stateMachine.addState(&testState3);

	stateMachine.write(MotorDriverRegisters::FAULT, 0);
	// Set state from Boot to SafeOp
	for (uint8_t i = 0; i < 2; i++)
	{
		stateMachine.run();
		motorDriver.update();

		// Sleep for 100ms to simulate cyclic control
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// Get fault because of invalid change
	uint32_t fault = stateMachine.read(MotorDriverRegisters::FAULT);

	if (1 == fault)
	{
		std::cout << "TestInvalidStateChange passed" << std::endl;
	}
	else
	{
		std::cout << "TestInvalidStateChange failed" << std::endl;
	}
}

void Tests()
{
	TestStateCreate();
	TestChecksum();
	TestTransition();
	TestOnEntry();
	TestOnExit();
	TestAction();
	TestInvalidCommand();
	TestInvalidStateChange();
}