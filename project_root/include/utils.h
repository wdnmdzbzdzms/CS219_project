#ifndef UTILS_H
#define UTILS_H

#include <stack>
#include <vector>
#include <string>
#include "editor.h"

void moveCursor(int& cursorX, int& cursorY, int directionX, int directionY, int maxX, int maxY, int& scrollOffset);
void pushToStack(std::stack<EditorState>& stack, const EditorState& state);
void undo();
void redo();
void jumpToLine(int lineNumber);
void switchFile(int fileIndex);
void searchAndReplace(const string &toFind, const string &toReplace);

#endif // UTILS_H
