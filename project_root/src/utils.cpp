#include "utils.h"
#include "editor.h"
#include <ncurses.h>

void moveCursor(int& cursorX, int& cursorY, int directionX, int directionY, int maxX, int maxY, int& scrollOffset) {
    cursorX += directionX;
    cursorY += directionY;

    // 防止游标超出行/列的限制
    if (cursorX < 0) cursorX = 0;
    if (cursorX > maxX) cursorX = maxX;

    if (cursorY < 0) cursorY = 0;
    if (cursorY > maxY) cursorY = maxY;

    // 确保游标不超出当前行的最大列数
    cursorX = std::min(cursorX, (int)fileContent[cursorY].length());

    // 根据游标位置滚动屏幕
    if (cursorY >= scrollOffset + (LINES - 2)) {
        scrollOffset++;
    } else if (cursorY < scrollOffset) {
        scrollOffset--;
    }
}

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

void searchAndReplace(const string &toFind, const string &toReplace) {
    for (auto &line : fileContent) {
        size_t pos = 0;
        while ((pos = line.find(toFind, pos)) != string::npos) {
            line.replace(pos, toFind.length(), toReplace);
            pos += toReplace.length();
        }
    }
}

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