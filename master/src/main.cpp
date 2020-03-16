#include <thread>
#include <chrono>
#include <iostream>

#include "motordriver.h"
#include "tests.h"


/**
 * @brief main This is a minimal main function.
 * @return
 */

int main(void)
{
	std::cout << "Starting tests:" << std::endl;
	Tests();
	std::cout << std::endl;

    std::cout << "Starting simulated motor driver" << std::endl;


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

    while(1)
    {
		
		stateMachine.run();
		motorDriver.update();

		uint32_t response = stateMachine.read(MotorDriverRegisters::STATUSWORD);

        std::cout << "Response to status word request: " << response << std::endl;


        // Call update to simulate motor driver doing work
        motorDriver.update();

        // Sleep for 100ms to simulate cyclic control
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
}
