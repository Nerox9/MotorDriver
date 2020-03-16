#include <cstdint>
#include "motordriver.h"


uint16_t Registers[REGISTERS_MAX_VALUE];


MotorDriver::MotorDriver()
{
    // Clear all registers
    for(uint8_t i = 0; i < REGISTERS_MAX_VALUE; ++i)
    {
        Registers[i] = 0;
    }

    // Set the fault flag on boot
    Registers[FAULT] = 1;
}

uint32_t MotorDriver::transferData(uint32_t data)
{
    uint8_t writing = (data >> 31);
    uint8_t command = (data >> 24) & 0x7F;
    uint16_t value = (data >> 8) & 0xFFFF;
    uint8_t checksum = data & 0xFF;

	uint8_t checksum_calculated = 0;
	// Calculate true checksum
	for (uint8_t i = 1; i < sizeof(data) / sizeof(uint8_t); ++i)
	{
		checksum_calculated ^= (data >> i * 8) & 0xFF;
	}

	// Compare input checksum and true checksum
	if (checksum != checksum_calculated)
	{
		Registers[MotorDriverRegisters::FAULT] = 1;
		return 0;
	}

    if(writing)
    {
		// Write encoder on SafeOp state only
		if(command == ENCODER_VALUE && Registers[STATUSWORD] != MotorState::STATE_SAFEOP)
		{
			return 0;
		}

		// Write velocity command on Op state only
		if (command == MOTOR_VELOCITY_COMMAND && Registers[STATUSWORD] != MotorState::STATE_OP)
		{
			return 0;
		}

        Registers[command] = value;
        return 0;
    }
    else
    {
		// Read encoder on SafeOp or Op states only
		if (command == ENCODER_VALUE && (Registers[STATUSWORD] != MotorState::STATE_SAFEOP && Registers[STATUSWORD] != MotorState::STATE_OP))
		{
			return 0;
		}

		// Calculate with checksum (command value checksum)
		uint32_t outData = command << 24 | Registers[command] << 8 | command^Registers[command];
        return outData;
    }


}

void MotorDriver::update()
{
	// Boot State
    if(Registers[STATUSWORD] == MotorState::STATE_BOOT)
    {
		// If any control status request, raises fault flag
		if (Registers[STATUSWORD] != Registers[CONTROLWORD])
		{
			Registers[FAULT] = 1;
		}
		// Else delay and go to PreOp State
		else
		{
			for (uint8_t i = 0; i < 255; i++)
			{
				//nop delay
			}
			Registers[STATUSWORD] = MotorState::STATE_PREOP;
			Registers[CONTROLWORD] = MotorState::STATE_PREOP;
		}
    }

	// PreOp State
	else if (Registers[STATUSWORD] == MotorState::STATE_PREOP)
	{
		// If control state is not SafeOp or PreOp raises fault flag
		if (Registers[CONTROLWORD] != MotorState::STATE_SAFEOP && Registers[CONTROLWORD] != MotorState::STATE_PREOP)
		{
			Registers[FAULT] = 1;
		}
		// Else go to SafeOp state
		else 
		{
			Registers[STATUSWORD] = MotorState::STATE_SAFEOP;
			Registers[CONTROLWORD] = MotorState::STATE_SAFEOP;
		}
	}

	// SafeOp State
	else if (Registers[STATUSWORD] == MotorState::STATE_SAFEOP)
	{
		// If control state is not Op, PreOp or SafeOp raises fault flag
		if (Registers[CONTROLWORD] != MotorState::STATE_OP && Registers[CONTROLWORD] != MotorState::STATE_PREOP && Registers[CONTROLWORD] != MotorState::STATE_SAFEOP)
		{
			Registers[FAULT] = 1;
		}
		// If fault flag set, set status to Op state
		else if (0 == Registers[FAULT])
		{
			Registers[STATUSWORD] = MotorState::STATE_OP;
			Registers[CONTROLWORD] = MotorState::STATE_OP;
		}
		// If control state is PreOp, go to PreOp
		else if (Registers[CONTROLWORD] == MotorState::STATE_PREOP)
		{
			Registers[STATUSWORD] = MotorState::STATE_PREOP;
		}
		// Stay same state
		else if(Registers[CONTROLWORD] == MotorState::STATE_SAFEOP)
		{
			// Do nothing
		}
		// Else raise fault flag
		else
		{
			Registers[FAULT] = 1;
		}
	}

	// OP State
	else if (Registers[STATUSWORD] == MotorState::STATE_OP)
	{
		// If control state is not Op, PreOp or SafeOp raises fault flag
		if (Registers[CONTROLWORD] != MotorState::STATE_SAFEOP && Registers[CONTROLWORD] != MotorState::STATE_PREOP && Registers[CONTROLWORD] != MotorState::STATE_OP)
		{
			Registers[FAULT] = 1;
		}
		// If fault flag set, set status to SafeOp state
		if (1 == Registers[FAULT])
		{
			Registers[STATUSWORD] = MotorState::STATE_SAFEOP;
			Registers[CONTROLWORD] = MotorState::STATE_SAFEOP;
		}
		// If control state is PreOp go to the PreOp state
		else if (MotorState::STATE_PREOP == Registers[CONTROLWORD])
		{
			Registers[STATUSWORD] = MotorState::STATE_PREOP;
		}
		// Stay same state
		else if (Registers[CONTROLWORD] == MotorState::STATE_OP)
		{
			// Do nothing
		}
		// Else raise fault flag
		else
		{
			Registers[FAULT] = 1;
		}
	}
	else
	{
		Registers[FAULT] = 1;
	}

    Registers[ENCODER_VALUE] += Registers[MOTOR_VELOCITY_COMMAND];
}
