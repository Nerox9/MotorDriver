#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <map>
#include <functional>

#include "motordriver.h"

using namespace std;

enum WriteFlag
{
	Read = 0,
	Write = 1
};

enum TransitionStates
{
	noTransition = 0,
	transOnEntry,
	transOnExit
};

/**
 * @brief The StateMachineBase class
 *
 * This is the base class implementation of state machine because of State Functions Declearation
 *
 */
class StateMachineBase
{
public:
	/**
	 * @brief Write data to motor driver register
	 * @param mtrDriverReg Register data to write
	 * @param value Data to write
	 * @return Received data
	 */
	virtual	uint32_t write(MotorDriverRegisters mtrDriverReg, uint16_t value) = 0;

	/**
	 * @brief Read data from motor driver register
	 * @param mtrDriverReg Register data to read
	 * @return Received data
	 */
	virtual uint32_t read(MotorDriverRegisters mtrDriverReg) = 0;

	/**
	* @brief Get current state as motor state
	* @return Next Motor State as MotorDriver MotorState enum
	*/
	virtual MotorState getNextMotorState() = 0;
};

/**
* @brief Templates of State Functions
*/
typedef function<bool(StateMachineBase*)> TCond; // Transition Condition Function template
typedef function<void(StateMachineBase*)> TOnEntry; // On Entry Function template
typedef function<void(StateMachineBase*)> TOnExit; // On Exit Function template
typedef function<uint16_t(StateMachineBase*)> TAction; // Action Function template


/**
* @brief Transition Condition Functions for all states
* @param stateMachine State Machine pointer
* @return True if condition is met
*/
bool BOOT_TCond(StateMachineBase* stateMachine);
bool PREOP_TCond(StateMachineBase* stateMachine);
bool SAFEOP_TCond(StateMachineBase* stateMachine);
bool OP_TCond(StateMachineBase* stateMachine);

/**
* @brief On Entry Functions for all states
* @param stateMachine State Machine pointer
*/
void BOOT_TOnEntry(StateMachineBase* stateMachine);
void PREOP_TOnEntry(StateMachineBase* stateMachine);
void SAFEOP_TOnEntry(StateMachineBase* stateMachine);
void OP_TOnEntry(StateMachineBase* stateMachine);


/**
* @brief On Exit Functions for all states
* @param stateMachine State Machine pointer
*/
void BOOT_TOnExit(StateMachineBase* stateMachine);
void PREOP_TOnExit(StateMachineBase* stateMachine);
void SAFEOP_TOnExit(StateMachineBase* stateMachine);
void OP_TOnExit(StateMachineBase* stateMachine);

/**
* @brief Action Functions for all states
* @param stateMachine State Machine pointer
* @return Received data
*/
uint16_t BOOT_TAction(StateMachineBase* stateMachine);
uint16_t PREOP_TAction(StateMachineBase* stateMachine);
uint16_t SAFEOP_TAction(StateMachineBase* stateMachine);
uint16_t OP_TAction(StateMachineBase* stateMachine);

class State
{
public:
	/**
	 * @brief Current MotorDriver Motor State
	 */
	MotorState motorState;		// MotorState

	/**
	 * @brief Next MotorDriver Motor State
	 */
	MotorState nextState;		// Next MotorState

	/**
	 * @brief Transition Condition Function of State
	 */
	const TCond transitionCond;	// Transition Condition Function

	/**
	 * @brief On Entry Function of State
	 */
	const TOnEntry onEntry;		// OnEntry Function

	/**
	 * @brief On Exit Function of State
	 */
	const TOnExit onExit;		// OnExit Function

	/**
	 * @brief Action Function of State
	 */
	const TAction action;		// Action Function

	/**
	 * @brief State
	 * Constructor. Creates the MotorState
	 * @param mtrState MotorDriver motor state enum
	 * @param next Next MotorDriver motor state enum
	 * @param transCond Transition Condition for state
	 */
	State(MotorState mtrState, MotorState next, const TCond transCond);

	/**
	 * @brief State
	 * Constructor. Creates the MotorState
	 * @param mtrState MotorDriver motor state enum
	 * @param next Next MotorDriver motor state enum
	 * @param transCond Transition Condition for state
	 * @param onEntry On Entry Function for state
	 */
	State(MotorState mtrState, MotorState next, const TCond transCond, const TOnEntry onEntry);

	/**
	 * @brief State
	 * Constructor. Creates the MotorState
	 * @param mtrState MotorDriver motor state enum
	 * @param next Next MotorDriver motor state enum
	 * @param transCond Transition Condition for state
	 * @param onEntry On Entry Function for state
	 * @param onExit On Exit Function for state
	 */
	State(MotorState mtrState, MotorState next, const TCond transCond, const TOnEntry onEntry, const TOnExit onExit);

	/**
	 * @brief State
	 * Constructor. Creates the MotorState
	 * @param mtrState MotorDriver motor state enum
	 * @param next Next MotorDriver motor state enum
	 * @param transCond Transition Condition for state
	 * @param onEntry On Entry Function for state
	 * @param onExit On Exit Function for state
	 * @param action Action Function for state
	 */
	State(MotorState mtrState, MotorState next, const TCond transCond, const TOnEntry onEntry, const TOnExit onExit, const TAction action);
	// Destructor
	//~State();
};

class StateMachine : public StateMachineBase
{
	/**
	 * @brief State map includes all states of State Machine
	 */
	map<MotorState, State*> states;

	/**
	 * @brief Active state of State Machine
	 */
	State *currentState;

	/**
	 * @brief Indicates transition state
	 */
	TransitionStates activeTransition = TransitionStates::noTransition;

public:
	/**
	 * @brief MotorDriver for transitions
	 */
	MotorDriver *motorDriver;

	/**
	 * @brief StateMachine
	 * Constructor. Creates the State Machine
	 */
	StateMachine();
	
	/**
	 * @brief StateMachine
	 * Constructor. Creates the State Machine
	 * @param mtrDriver MotorDriver to send and receive data
	 */
	StateMachine(MotorDriver *mtrDriver);
	// Destructor
	//~StateMachine();

	/**
	* @brief Add a state to state map
	* @param state State pointer
	*/
	void addState(State *state);

	/**
	* @brief Get current state as motor state
	* @return Current Motor State as MotorDriver MotorState enum
	*/
	MotorState getMotorState();
	
	/**
	* @brief Get current state as motor state
	* @return Next Motor State as MotorDriver MotorState enum
	*/
	MotorState getNextMotorState();
	
	/**
	* @brief Get active transition status
	* @return True if status is noTransition
	*/
	bool getTransition();
	
	/**
	* @brief Get current state pointer
	* @return Current state pointer
	*/
	State* getCurrentState();

	/**
	* @brief Checksum Calculator
	* @param data Checksum calculation data
	* @return Checksum of data
	*/
	uint8_t checksumCalc(uint32_t data);
	
	/**
	 * @brief Write data to motor driver register
	 * @param mtrDriverReg Register data to write
	 * @param value Data to write
	 * @return Received data
	 */
	uint32_t write(MotorDriverRegisters, uint16_t);
	
	/**
	 * @brief Read data from motor driver register
	 * @param mtrDriverReg Register data to read
	 * @return Received data
	 */
	uint32_t read(MotorDriverRegisters);

	/**
	 * @brief Run Transition and State Functions
	 */
	void run();
};

#endif /* STATEMACHINE_H */
