#include "fileio.h"
#include <fstream>
#include <iostream>
#include <string>
#include "editor.h"
#include <ncurses.h>

using namespace std;

// Save file to disk
void saveFile(const string &filename) {
    FILE *file = fopen(filename.c_str(), "w");
    for (const string &line : fileContent) {
        fprintf(file, "%s\n", line.c_str());
    }
    fclose(file);
    mvprintw(LINES - 1, 0, "File saved to %s", filename.c_str());
    clrtoeol();
    getch();
}

// Load file from disk or create a new file if it doesn't exist
void loadFile(const string &filename) {
    FILE *file = fopen(filename.c_str(), "r");
    if (!file) {
        mvprintw(LINES - 1, 0, "File does not exist. Creating a new file: %s", filename.c_str());
        clrtoeol();
        getch(); // Wait for key press

        // Initialize the file content as empty
        fileContent.clear();
        allFileContents[currentFileIndex] = fileContent; // Save the content in the global store

        cursorX = 0;
        cursorY = 0;
        scrollOffset = 0;
        while (!undoStack.empty()) undoStack.pop();
        while (!redoStack.empty()) redoStack.pop();
        return;
    }

    // Read existing file content
    fileContent.clear();
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        string line(buffer);
        if (!line.empty() && line.back() == '\n') {
            line.pop_back(); // Remove newline character
        }
        fileContent.push_back(line);
    }
    fclose(file);

    allFileContents[currentFileIndex] = fileContent; // Save the content in the global store

    cursorX = 0;
    cursorY = 0;
    scrollOffset = 0;

    mvprintw(LINES - 1, 0, "File loaded: %s", filename.c_str());
    clrtoeol();
    getch(); // Wait for key press
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
}