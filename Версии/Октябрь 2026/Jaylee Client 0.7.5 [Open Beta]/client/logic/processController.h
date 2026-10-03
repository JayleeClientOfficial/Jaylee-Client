#pragma once

#include <string>
#include <windows.h>

namespace processcontroller
{
    bool IsAnotherInstanceRunning();

    bool EnsureSingleInstance();

    void TerminateChildProcesses();

    void ShowDuplicateInstanceWarningAndExit();
}

