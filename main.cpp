#include "core/simulation_runner.h"
#include "ui/cli_manager.h"
#include "pretty_print.h"

// Include module implementations — enables single-file compilation:
#include "core/simulation_runner.cpp"
#include "core/otp_manager.cpp"
#include "email/smtp_client.cpp"
#include "email/email_service.cpp"
#include "ui/cli_manager.cpp"

int main() {
    system("chcp 65001 > nul");
    system("mode con cols=72");

    AuraCLI::printBanner();

    SimulationRunner systemRunner;
    systemRunner.initialize();

    std::cout << AuraCLI::Color::DIM
              << "\n  Press ENTER to launch the kiosk interface..."
              << AuraCLI::Color::RESET;
    std::cin.get();

    CLIManager cli(systemRunner);
    cli.run();

    return 0;
}