#include "editor.h"
#include "utils.h"
#include "fileio.h"
#include <ncurses.h>
#include <stack>
#include <algorithm>

// Global Variables
vector<string> fileContent;
int cursorX = 0, cursorY = 0;
string mode = "NORMAL";
stack<EditorState> undoStack;
stack<EditorState> redoStack;
int scrollOffset = 0;
string clipboard;
vector<string> openFiles;
int currentFileIndex = 0;
vector<vector<string>> allFileContents;
const int MAX_HISTORY = 100;

// Function definitions
void initializeScreen() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    fileContent.push_back("Welcome to Mini Vim!");
    fileContent.push_back("This is a test line.");
}

void cleanupScreen() {
    endwin();
}

void drawScreen() {
    clear();
    int visibleLines = LINES - 2;
    for (int i = 0; i < visibleLines && (i + scrollOffset) < fileContent.size(); i++) {
        if (mode == "INSERT") {
            attron(COLOR_PAIR(2)); 
        } else if (mode == "NORMAL") {
            attron(COLOR_PAIR(3)); 
        }
        mvprintw(i, 0, "%3d | %s", i + 1 + scrollOffset, fileContent[i + scrollOffset].c_str());
        if (mode == "INSERT") {
            attroff(COLOR_PAIR(2));
        } else if (mode == "NORMAL") {
            attroff(COLOR_PAIR(3));
        }
    }
    drawStatusBar();
    move(cursorY - scrollOffset, cursorX + 5);
    refresh();
}

void drawStatusBar() {
    attron(COLOR_PAIR(1));
    mvprintw(LINES - 1, 0, "-- %s MODE -- Cursor: (%d, %d) Lines: %d File: %s", 
             mode.c_str(), cursorY + 1, cursorX + 1, fileContent.size(), openFiles[currentFileIndex].c_str());
    attroff(COLOR_PAIR(1));
    clrtoeol();
}

void handleNormalMode() {
    int ch = getch();
    if (fileContent.empty()) {
        switch (ch) {
            case 'i':
                mode = "INSERT";
                break;
            case ':':
                mode = "COMMAND";
                break;
        }
    }else{
        switch (ch) {
            case 'h': case KEY_LEFT:
                moveCursor(cursorX, cursorY, -1, 0, fileContent[cursorY].length(), fileContent.size() - 1, scrollOffset);
                break;
            case 'l': case KEY_RIGHT:
                moveCursor(cursorX, cursorY, 1, 0, fileContent[cursorY].length(), fileContent.size() - 1, scrollOffset);
                break;
            case 'j': case KEY_DOWN:
                moveCursor(cursorX, cursorY, 0, 1, fileContent[cursorY].length(), fileContent.size() - 1, scrollOffset);
                break;
            case 'k': case KEY_UP:
                moveCursor(cursorX, cursorY, 0, -1, fileContent[cursorY].length(), fileContent.size() - 1, scrollOffset);
                break;
            case '0':
                cursorX = 0;
                break;
            case '$':
                cursorX = fileContent[cursorY].length();
                break;
            case 'g': {
                int nextChar = getch();
                if (nextChar == 'g') {
                    cursorY = 0;
                    cursorX = 0;
                    scrollOffset = 0;
                }
                break;
            }
            case 'G':
                cursorY = fileContent.size() - 1;
                cursorX = 0;
                if (cursorY >= scrollOffset + (LINES - 2)) {
                    scrollOffset = max(0, (int)fileContent.size() - (LINES - 2));
                }
                break;
            case 'd': {
                int nextChar = getch();
                if (nextChar == 'd') {
                    pushToStack(undoStack, {fileContent, cursorX, cursorY, scrollOffset});
                    redoStack = stack<EditorState>();
                    fileContent.erase(fileContent.begin() + cursorY);
                    if (cursorY >= fileContent.size()) cursorY = fileContent.size() - 1;
                }
                break;
            }
            case 'y': {
                int nextChar = getch();
                if (nextChar == 'y') {
                    clipboard = fileContent[cursorY];
                }
                break;
            }
            case 'p': {
                pushToStack(undoStack, {fileContent, cursorX, cursorY, scrollOffset});
                redoStack = stack<EditorState>();
                fileContent.insert(fileContent.begin() + cursorY + 1, clipboard);
                cursorY++;
                cursorX = 0;
                break;
            }
            case 'i':
                mode = "INSERT";
                break;
            case ':':
                mode = "COMMAND";
                break;
            case 'u':
                undo();
                break;
            case 'r': // Redo
                redo();
                break;
            case 18: // Ctrl+R for redo
                redo();
                break;
        }
    }
}



// Handle INSERT mode
void handleInsertMode() {
    pushToStack(undoStack, {fileContent, cursorX, cursorY, scrollOffset}); // Save state for undo
    redoStack = stack<EditorState>(); // Clear redo stack
    if (fileContent.empty()) {
        fileContent.push_back(""); // Add an empty line to start with
    }
    while (true) {
        int ch = getch();
        switch (ch) {
            case 27: // ESC key to return to NORMAL mode
                mode = "NORMAL";
                return;
            case 10: // Enter key to create a new line
                fileContent.insert(fileContent.begin() + cursorY + 1, fileContent[cursorY].substr(cursorX));
                fileContent[cursorY] = fileContent[cursorY].substr(0, cursorX);
                cursorY++;
                cursorX = 0;
                if (cursorY >= scrollOffset + (LINES - 2)) scrollOffset++;
                break;
            case KEY_BACKSPACE: // Backspace key
            case 127:
                if (cursorX > 0) {
                    fileContent[cursorY].erase(cursorX - 1, 1);
                    cursorX--;
                } else if (cursorY > 0) {
                    cursorX = fileContent[cursorY - 1].length();
                    fileContent[cursorY - 1] += fileContent[cursorY];
                    fileContent.erase(fileContent.begin() + cursorY);
                    cursorY--;
                    if (cursorY < scrollOffset) scrollOffset--;
                }
                break;
            case KEY_LEFT:
                    moveCursor(cursorX, cursorY, -1, 0, fileContent[cursorY].length(), fileContent.size() - 1, scrollOffset);
                    break;
            case KEY_RIGHT:
                    moveCursor(cursorX, cursorY, 1, 0, fileContent[cursorY].length(), fileContent.size() - 1, scrollOffset);
                    break;
            case KEY_DOWN:
                    moveCursor(cursorX, cursorY, 0, 1, fileContent[cursorY].length(), fileContent.size() - 1, scrollOffset);
                    break;
            case KEY_UP:
                    moveCursor(cursorX, cursorY, 0, -1, fileContent[cursorY].length(), fileContent.size() - 1, scrollOffset);
                    break;
            default: // Insert characters
                fileContent[cursorY].insert(cursorX, 1, ch);
                cursorX++;
                break;
        }
        drawScreen(); // Refresh the screen after every input
    }
}

// Handle COMMAND mode
void handleCommandMode() {
    echo();
    attron(COLOR_PAIR(4)); // Set COMMAND mode color
    mvprintw(LINES - 2, 0, ":");
    char command[256];
    getstr(command);
    noecho();
    attroff(COLOR_PAIR(4)); // Reset color

    string cmd(command);
    if (cmd.rfind("e ", 0) == 0) { // Check for :e <filename>
        string filename = cmd.substr(2);
        openFiles.push_back(filename); // Add to open files list
        allFileContents.push_back({}); // Initialize content for the new file
        currentFileIndex = openFiles.size() - 1;
        fileContent = allFileContents[currentFileIndex];
        loadFile(filename);
    } else if (cmd == "w") {
        saveFile(openFiles[currentFileIndex]);
    } else if (cmd == "q") {
        cleanupScreen();
        exit(0);
    } else if (cmd == "wq") {
        saveFile(openFiles[currentFileIndex]);
        cleanupScreen();
        exit(0);
    } else if (cmd.rfind("s/", 0) == 0) { // Search and replace
        size_t firstSlash = cmd.find("/", 2);
        size_t secondSlash = cmd.find("/", firstSlash + 1);
        string toFind = cmd.substr(2, firstSlash - 2);
        string toReplace = cmd.substr(firstSlash + 1, secondSlash - firstSlash - 1);
        pushToStack(undoStack, {fileContent, cursorX, cursorY, scrollOffset}); // Save state for undo before search and replace
        redoStack = stack<EditorState>();  // Correctly reinitialize redoStack// Clear redo stack
        searchAndReplace(toFind, toReplace);
    } else if (cmd.rfind("line ", 0) == 0) { // Jump to line
        int lineNumber = stoi(cmd.substr(5));
        jumpToLine(lineNumber);
    } else if (cmd.rfind("switch ", 0) == 0) { // Switch file
        int fileIndex = stoi(cmd.substr(7));
        switchFile(fileIndex);
    }

    mode = "NORMAL";
}

void enableColors() {
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLUE);   // Status bar color
        init_pair(2, COLOR_GREEN, COLOR_BLACK); // INSERT mode text color
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); // NORMAL mode text color
        init_pair(4, COLOR_CYAN, COLOR_BLACK);  // COMMAND mode text color
    }
}
