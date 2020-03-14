#include <iostream>
#include "motordriver.h"
#include "statemachine.h"

/* Firstly I have tried VS Native Unit Test Project */
/* However it did not work with Cmake appropriately */

void TestStateCreate()
{
	StateMachine stateMachine;
	State testState(STATE_BOOT);
	
	stateMachine.addState(testState);

	if (STATE_BOOT == testState.motorState)
	{
		std::cout << "TestStateCreate passed" << std::endl;
	}
	else
	{
		std::cout << "TestStateCreate Failed" << std::endl;
	}
}

void Tests()
{
	TestStateCreate();
}