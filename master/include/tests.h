#ifndef TESTS_H
#define TESTS_H

#include <iostream>
#include <chrono>
#include <thread>
#include "motordriver.h"
#include "statemachine.h"

/**
* @brief Runs all test functions
*/
void Tests();

/**
* @brief Create state and test
*/
void TestStateCreate();

/**
* @brief Create checksum and test
*/
void TestChecksum();

/**
* @brief Create two state and test transition
*/
void TestTransition();

/**
* @brief Create all states and test onEntry function
*/
void TestOnEntry();

/**
* @brief Create all states and test onExit function with Op state
*/
void TestOnExit();

/**
* @brief Create all states and test action function with Op state
*/
void TestAction();

/**
* @brief Create invalid command with false checksum and test fault register
*/
void TestInvalidCommand();

/**
* @brief Create invalid state transition and test fault register
*/
void TestInvalidStateChange();
#endif /* TESTS_H */
