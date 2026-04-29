#pragma once
#include <string>
#include "../core/simulation_runner.h"

enum class Role { NONE, USER, ADMIN };

class CLIManager {
    SimulationRunner& sim;
    Role currentRole = Role::NONE;
    std::string currentUser;

public:
    CLIManager(SimulationRunner& s);
    void run();
};
