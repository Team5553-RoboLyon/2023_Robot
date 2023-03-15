// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/ChekMachine.h"

ChekMachine::ChekMachine()
{
    BITCLEAR(m_val, 0);
}

void ChekMachine::SubsystemsRunning(int id)
{
    BITSET(m_val, id);
}

void ChekMachine::SubsystemsStop(int id)
{
    BITCLEAR(m_val, id);
}

bool ChekMachine::AuthorisationToRun(int id)
{
    return BITGET(m_val, id);
}
