#include "LogObserver.h"
#include "CommandProcessing.h"


void testLoggingObserver() {
    LogObserver logObserver("gamelog.txt");

    // Setup CommandProcessor and attach observer
    CommandProcessor cmdProcessor;
    cmdProcessor.attach(&logObserver);
    
    // Create and save a command
    Command* cmd = new Command("loadmap");
    cmd->saveEffect("Map loaded successfully");
    cmdProcessor.saveCommand(cmd);

    // Clean up
    delete cmd;
}

int main() {
    testLoggingObserver();
    return 0;
}
