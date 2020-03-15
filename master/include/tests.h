#ifndef TESTS_H
#define TESTS_H

#include <iostream>
#include <chrono>
#include <thread>
#include "motordriver.h"
#include "statemachine.h"

void Tests();
void TestStateCreate();
void TestChecksum();
void TestTransition();
void TestOnEntry();
void TestOnExit();
void TestAction();
void TestInvalidCommand();
void TestInvalidStateChange();
#endif /* TESTS_H */
