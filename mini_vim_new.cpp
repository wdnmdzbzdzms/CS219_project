//edit by: zhd
#include <ncurses.h>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <unistd.h> 

using namespace std;

// Global Variables
struct EditorState {
    vector<string> content; // File content
    int cursorX, cursorY;   // Cursor position
    int scrollOffset;       // Scroll offset
};

vector<string> fileContent;               // Store file content line by line
int cursorX = 0, cursorY = 0;             // Cursor position
string mode = "NORMAL";                   // Current mode (NORMAL, INSERT, COMMAND)
stack<EditorState> undoStack;             // Undo stack
stack<EditorState> redoStack;             // Redo stack
int scrollOffset = 0;                     // Scroll offset for handling large files
string clipboard;                         // Clipboard for copy and paste
vector<string> openFiles;                 // List of open files
int currentFileIndex = 0;                 // Index of the current file
vector<vector<string>> allFileContents;   // Store content for all open files
const int MAX_HISTORY = 100;              // Limit undo/redo stack size

// Function Prototypes
void initializeScreen();
void cleanupScreen();
void handleNormalMode();
void handleInsertMode();
void handleCommandMode();
void drawScreen();
void drawStatusBar();
void saveFile(const string &filename);
void loadFile(const string &filename);
void undo();
void redo();
void enableColors();
void searchAndReplace(const string &toFind, const string &toReplace);
void jumpToLine(int lineNumber);
void switchFile(int fileIndex);
void pushToStack(stack<EditorState>& stack, const EditorState& state);

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

// Initialize ncurses
void initializeScreen() {
    initscr();            // Start ncurses mode
    raw();                // Disable line buffering
    keypad(stdscr, TRUE); // Enable function keys
    noecho();             // Disable character echoing
    fileContent.push_back("Welcome to Mini Vim!"); // Initialize with some content
    fileContent.push_back("This is a test line.");
}

// Enable colors in the editor
void enableColors() {
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLUE);   // Status bar color
        init_pair(2, COLOR_GREEN, COLOR_BLACK); // INSERT mode text color
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); // NORMAL mode text color
        init_pair(4, COLOR_CYAN, COLOR_BLACK);  // COMMAND mode text color
    }
}

// Cleanup ncurses
void cleanupScreen() {
    endwin();
}

// Push to stack with size limit
void pushToStack(stack<EditorState>& stack, const EditorState& state) {
    if (stack.size() >= MAX_HISTORY) {
        std::stack<EditorState> temp;
        while (stack.size() > 1) {
            temp.push(stack.top());
            stack.pop();
        }
        stack.pop(); // Remove the oldest state
        while (!temp.empty()) {
            stack.push(temp.top());
            temp.pop();
        }
    }
    stack.push(state);
}

// Handle NORMAL mode
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
                if (cursorX > 0) cursorX--;
                break;
            case 'l': case KEY_RIGHT:
                if (cursorX < fileContent[cursorY].length()) cursorX++;
                break;
            case 'j': case KEY_DOWN:
                if (cursorY < fileContent.size() - 1) {
                    cursorY++;
                    if (cursorY >= scrollOffset + (LINES - 2)) scrollOffset++;
                    cursorX = min(cursorX, (int)fileContent[cursorY].length());
                }
                break;
            case 'k': case KEY_UP:
                if (cursorY > 0) {
                    cursorY--;
                    if (cursorY < scrollOffset) scrollOffset--;
                    cursorX = min(cursorX, (int)fileContent[cursorY].length());
                }
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
              case KEY_LEFT: // Left arrow key
                if (cursorX > 0) {
                    cursorX--;
                }
                break;
            case KEY_RIGHT: // Right arrow key
                if (cursorX < fileContent[cursorY].length()) {
                    cursorX++;
                }
                break;
            case KEY_UP: // Up arrow key
                if (cursorY > 0) {
                    cursorY--;
                    if (cursorX > fileContent[cursorY].length()) cursorX = fileContent[cursorY].length(); // Adjust cursorX if necessary
                    if (cursorY < scrollOffset) scrollOffset--; // Scroll up if necessary
                }
                break;
            case KEY_DOWN: // Down arrow key
                if (cursorY < fileContent.size() - 1) {
                    cursorY++;
                    if (cursorX > fileContent[cursorY].length()) cursorX = fileContent[cursorY].length(); // Adjust cursorX if necessary
                    if (cursorY >= scrollOffset + (LINES - 2)) scrollOffset++; // Scroll down if necessary
                }
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

// Draw the screen
void drawScreen() {
    clear();
    int visibleLines = LINES - 2;
    for (int i = 0; i < visibleLines && (i + scrollOffset) < fileContent.size(); i++) {
        if (mode == "INSERT") {
            attron(COLOR_PAIR(2)); // Set INSERT mode text color
        } else if (mode == "NORMAL") {
            attron(COLOR_PAIR(3)); // Set NORMAL mode text color
        }
        mvprintw(i, 0, "%3d | %s", i + 1 + scrollOffset, fileContent[i + scrollOffset].c_str());
        if (mode == "INSERT") {
            attroff(COLOR_PAIR(2));
        } else if (mode == "NORMAL") {
            attroff(COLOR_PAIR(3));
        }
    }
    drawStatusBar();
    move(cursorY - scrollOffset, cursorX + 5); // Adjust cursor for line numbers
    refresh();
}

// Draw the status bar
void drawStatusBar() {
    attron(COLOR_PAIR(1));
    mvprintw(LINES - 1, 0, "-- %s MODE -- Cursor: (%d, %d) Lines: %d File: %s", 
             mode.c_str(), cursorY + 1, cursorX + 1, fileContent.size(), openFiles[currentFileIndex].c_str());
    attroff(COLOR_PAIR(1));
    clrtoeol();
}

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


// Search and replace in file
void searchAndReplace(const string &toFind, const string &toReplace) {
    for (auto &line : fileContent) {
        size_t pos = 0;
        while ((pos = line.find(toFind, pos)) != string::npos) {
            line.replace(pos, toFind.length(), toReplace);
            pos += toReplace.length();
        }
    }
}

// Jump to specific line
void jumpToLine(int lineNumber) {
    if (lineNumber > 0 && lineNumber <= fileContent.size()) {
        cursorY = lineNumber - 1;
        cursorX = 0;
        if (cursorY >= scrollOffset + (LINES - 2)) {
            scrollOffset = max(0, cursorY - (LINES - 2) + 1);
        } else if (cursorY < scrollOffset) {
            scrollOffset = cursorY;
        }
    }
}

// Switch between files
void switchFile(int fileIndex) {
    if (fileIndex >= 0 && fileIndex < openFiles.size()) {
        allFileContents[currentFileIndex] = fileContent; // Save current file content
        currentFileIndex = fileIndex;
        fileContent = allFileContents[currentFileIndex]; // Load selected file content
        cursorX = 0;
        cursorY = 0;
        scrollOffset = 0;
    }
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
}

// Undo operation
void undo() {
    if (!undoStack.empty()) {
        // 保存当前状态到 redoStack
        pushToStack(redoStack, {fileContent, cursorX, cursorY, scrollOffset});
        
        // 弹出 undoStack 中的一个状态，并恢复
        EditorState state = undoStack.top();
        undoStack.pop();
        
        // 恢复到 undo 状态
        fileContent = state.content;
        cursorX = state.cursorX;
        cursorY = state.cursorY;
        scrollOffset = state.scrollOffset;
        
    } else {
        mvprintw(LINES - 1, 0, "Nothing to undo!");
        clrtoeol();
        getch();
    }
}

// Redo operation
void redo() {
    if (!redoStack.empty()) {
        // 保存当前状态到 undoStack
        pushToStack(undoStack, {fileContent, cursorX, cursorY, scrollOffset});
        
        // 弹出 redoStack 中的一个状态，并恢复
        EditorState state = redoStack.top();
        redoStack.pop();
        
        // 恢复到 redo 状态
        fileContent = state.content;
        cursorX = state.cursorX;
        cursorY = state.cursorY;
        scrollOffset = state.scrollOffset;
    } else {
        mvprintw(LINES - 1, 0, "Nothing to redo!");
        clrtoeol();
        getch();
    }
}
