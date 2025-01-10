#include <ncurses.h>
#include "editor.h"
#include "utils.h"
#include "fileio.h"

int main(int argc, char *argv[]) {
    initializeScreen();
    enableColors();

    // Command-line argument handling for file opening
    if (argc > 1) {
        string initialFile = argv[1];
        openFiles.push_back(initialFile); // Add file to the open files list
        allFileContents.push_back({}); // Initialize content for the file
        currentFileIndex = openFiles.size() - 1;
        fileContent = allFileContents[currentFileIndex];
        loadFile(initialFile);  // Load the file specified on command line
    } else {
        // If no file is specified, open the default output.txt
        openFiles.push_back("output.txt");
        allFileContents.push_back({});
        currentFileIndex = openFiles.size() - 1;
        fileContent = allFileContents[currentFileIndex];
        loadFile("output.txt");
    }

    // Main loop
    while (true) {
        drawScreen();

        if (mode == "NORMAL") {
            handleNormalMode();
        } else if (mode == "INSERT") {
            handleInsertMode();
        } else if (mode == "COMMAND") {
            handleCommandMode();
        }
    }

    cleanupScreen();
    return 0;
}
