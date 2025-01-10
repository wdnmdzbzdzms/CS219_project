#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <vector>
#include <string>
#include <stack>
using namespace std;
// Global Variables
struct EditorState {
    vector<string> content; // File content
    int cursorX, cursorY;   // Cursor position
    int scrollOffset;       // Scroll offset
};


extern vector<string> fileContent;              // Store file content line by line
extern int cursorX, cursorY;                    // Cursor position
extern string mode;                             // Current mode (NORMAL, INSERT, COMMAND)
extern stack<EditorState> undoStack;            // Undo stack
extern stack<EditorState> redoStack;            // Redo stack
extern int scrollOffset;                        // Scroll offset for handling large files
extern string clipboard;                        // Clipboard for copy and paste
extern vector<string> openFiles;                // List of open files
extern int currentFileIndex;                    // Index of the current file
extern vector<vector<string>> allFileContents;  // Store content for all open files
extern const int MAX_HISTORY;                   // Limit undo/redo stack size


// Function Prototypes
void initializeScreen();
void cleanupScreen();
void handleNormalMode();
void handleInsertMode();
void handleCommandMode();
void drawScreen();
void drawStatusBar();
void enableColors();

#endif // EDITOR_H
