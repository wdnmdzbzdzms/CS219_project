## Function testing

```shell
sudo apt install screenkey
```

We use `screenkey` tool to show our key operations in real-time.

Some functions are **not** convenient to show in the report, but we will show them in the **presentation**.

### Basic function testing

#### 1. Normal mode

type `./vim` in terminal to execute the program in default situation.

<img src="https://s2.loli.net/2025/01/10/DveXNlbp5kzC9Ay.png" alt="image-20250110222912543"  />

Because no file is specified to be opened, `output.txt` is created by default.

![image-20250110222955018](https://s2.loli.net/2025/01/10/qrgFXGSRVtOWQP3.png)



If you open a specific file, such as `o.txt`, you will directly enter `Normal mode`. And the cursor is at the beginning of the line

![image-20250110223018998](https://s2.loli.net/2025/01/10/zAiZXhbT3c2trkj.png)



#####  **Line Navigation**:

`$ `**for line  end**

![image-20250110223029757](https://s2.loli.net/2025/01/10/LCEeM9A5UBy6b1Y.png)

`0 ` **for line start**

![image-20250110223050301](https://s2.loli.net/2025/01/10/hjsQntmqz7aYVZU.png)

\-`gg` and `G` for **first and last line navigation**

Use the arrow keys to move the cursor to the middle of the paragraph.

![image-20250110223100664](https://s2.loli.net/2025/01/10/GmTdX7P8BfJpR2j.png)

`gg`

![image-20250110223110964](https://s2.loli.net/2025/01/10/IRNxtlDQqUBPkLp.png)

`G`

![image-20250110223138870](https://s2.loli.net/2025/01/10/y6fTtWQbrJGSFsX.png)

#####  **Basic Editing**:

**`dd` Delete current line**

delete the third line

![image-20250110223151601](https://s2.loli.net/2025/01/10/bWUZj2hxnoq57KG.png)

![image-20250110223159626](https://s2.loli.net/2025/01/10/YOoEPLZbeWM2qSC.png)

**`yy` Copy current line and `p `Paste copied line**

Copy the second line and paste. The picture shows the third line becomes `222`.Move the cursor to the beginning of the third line

![image-20250110223210317](https://s2.loli.net/2025/01/10/ySHsqL9AEx2oTw3.png)

![image-20250110223223245](https://s2.loli.net/2025/01/10/xDB7j1JC594nsKa.png)

#### 2. **Insert Mode and Command Line Mode**

Type `i` to Insert mode. The text color becomes **green**.

![image-20250110223238105](https://s2.loli.net/2025/01/10/rBWwFkOMp2T8od9.png)

**Real-time cursor updates during typing**

Type :`this is a test line` in 4th line.

![image-20250110223248905](https://s2.loli.net/2025/01/10/BzSLqMtAKuw9Yhn.png)

**Cursor Movement with Typed Text**

Ability to change cursor position directly using arrow keys

![image-20250110223258090](https://s2.loli.net/2025/01/10/sL5JXHBnt3Q9U4b.png)

Press `ESC` to exit insert mode. Use `:wq` to save directly to exit program `minivim`

![image-20250110223346473](https://s2.loli.net/2025/01/10/DeLmCo7adZRIkqn.png)

Open `o.txt` and find that the text has been completely saved



![image-20250110223359126](https://s2.loli.net/2025/01/10/qSVmtpwGTWCf6xn.png)





### Advanced Features

* **Search and Replace**:

  We can implement the function to find and replace with a command- `:s/old/new/g`

  ***Example:***

  Before:

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110211734198.png" alt="image-20250110211734198" style="zoom:92%;" />

  Then, we can input `:s/is/IS not/g` to find all "is" in the content, and then replace them with "IS not". The result is as below,

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110211326586.png" alt="image-20250110211326586" style="zoom:80%;" />

* **Undo and Redo:**

  We can input `u` to implement undo. 

  ***Example:***

  in the above case, we input `u`, then the content is changed to the original content

  ![image-20250110212759287](https://gitee.com/ullrhd/typora-image/raw/master/image-20250110212759287.png)

  Then, if we continue to input `u`, the it would display "Nothing to undo!".

  If we input `R/Ctrl+R`, we can implement redo. The content would be changed to the latest content:

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110213446068.png" alt="image-20250110213446068" style="zoom:80%;" />

  If we continue to input `R/Ctrl+R`, then it will display "Nothing to redo!", as there is no content to redo.

* **Line Numbers and Jumping:**

  As shown above, the line numbers can be shown in this content. Moreover, we can use `:line #` to jump to the #-th line.

   ***Example:***

  we can input `:line 12` to jump to 12-th line.

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110214911874.png" alt="image-20250110214911874" style="zoom:80%;" />

* **Multi-File Editing:**

  `:e <filename>` to open an another file. If this file doesn't exist, then it will create a new file. When the user input ":w" or ":wq", this file will be saved to the local file folder. 

  ***Example:***

  when we input `:e test1.txt`

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110221712288.png" alt="image-20250110221712288" style="zoom:80%;" />

  then it will open test1.txt, 

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110222039940.png" alt="image-20250110222039940" style="zoom:80%;" />

  If this a new file. Then, it will create a new file.

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110222221736.png" alt="image-20250110222221736" style="zoom:80%;" />

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110222309976.png" alt="image-20250110222309976" style="zoom:80%;" />

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110222338788.png" alt="image-20250110222338788" style="zoom:80%;" />

* **UI Improvements:**

  we use different color to distinguish different mode, i.e., yellow for normal mode, green for insert mode, and white for command mode. Moreover, we use the sentence with background of blue at the bottom to present different mode.

  <img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110222556482.png" alt="image-20250110222556482" style="zoom:80%;" />

<img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110222941903.png" alt="image-20250110222941903" style="zoom:80%;" />

<img src="https://gitee.com/ullrhd/typora-image/raw/master/image-20250110222635163.png" alt="image-20250110222635163" style="zoom: 70%;" />

## Code realization

### Brief Introduction

This program mainly depends on the following libraries and some basic logic. We will give a detailed description in this section

**`ncurses` Library**:

- Powers the terminal-based UI, including screen rendering, cursor movement, and input handling.
- Enables features like color themes (`COLOR_PAIR`), multi-line display, and status bar rendering.

**C++ Standard Libraries**:

- **`vector`**: Used extensively for dynamic storage of file content, line-by-line management, and multi-file handling.
- **`stack`**: Provides undo and redo functionality through a stack-based history mechanism.
- **`string`**: Facilitates operations like parsing commands, managing text content, and search/replace functionalities.

### **Core Data Structures and Variables**

- **`fileContent`**: Stores the content of the currently opened file, split into lines of text.
- **`cursorX` and `cursorY`**: Represent the cursor's column and row position, indicating the user's current editing location.
- **`scrollOffset`**: Manages screen scrolling to display larger files effectively.
- **`undoStack` and `redoStack`**: Preserve editing history, enabling undo and redo functionality.
- **`openFiles` and `allFileContents`**: Support opening and switching between multiple files.

### Implementation

1. **Cursor Movement and Screen Scrolling**
   Cursor movement is implemented in the `moveCursor` function. This function updates the global variables `cursorX` and `cursorY` while ensuring they stay within valid bounds, including the current line's length. If the cursor moves out of the visible screen range, the `scrollOffset` is adjusted to keep the cursor visible on the screen. The `fileContent` vector stores each line of the file, and `ncurses` functions such as `move` handle the cursor's terminal position. The screen height is determined using `LINES`, allowing the editor to adapt to terminal size dynamically.

------

2. **Mode Switching**
   The current mode (`NORMAL`, `INSERT`, or `COMMAND`) is tracked using the global string variable `mode`. Mode-specific behaviors are implemented in `handleNormalMode`, `handleInsertMode`, and `handleCommandMode` functions. User inputs like `i` for INSERT mode or `:` for COMMAND mode are parsed and trigger appropriate mode-switching logic. The `string` library is used to store and manipulate the mode name and commands.

------

3. **File Operations**
   File operations are managed with `fileContent`, which stores the content line by line in a `vector<string>`. For loading files, the `loadFile` function uses a `FILE` pointer with `fopen` to open the file and `fgets` to read its contents line by line. Each line is stored in the vector for easy manipulation. The `saveFile` function writes the contents back to the disk using `fprintf`. If the file doesn’t exist, it is created automatically. The combination of the C Standard I/O library and `vector` provides efficient file handling and content storage.

------

4. **Undo and Redo**
   Undo and redo functionality is implemented using two stacks: `undoStack` and `redoStack`. Each stack stores snapshots of the editor's state, including `fileContent`, `cursorX`, `cursorY`, and `scrollOffset`. Before every edit, the current state is pushed onto `undoStack` using the `pushToStack` function. During an undo, the top state from `undoStack` is popped and restored, while the current state is saved to `redoStack`. Redo performs the reverse operation. The `stack` library handles the history mechanism, while `fileContent` snapshots are managed using `vector<string>`.

------

5. **Search and Replace**
   The `searchAndReplace` function iterates through `fileContent` and performs search-and-replace operations line by line. It uses `string`'s `find` method to locate the target string within each line and `replace` to substitute it with the desired string. The combination of `find` and `replace` simplifies the process of locating and modifying text, while the `vector<string>` structure efficiently organizes the file’s content during this operation.

------

6. **Multi-File Management**
   Multi-file management is achieved using two vectors: `openFiles` stores the file paths of all opened files, and `allFileContents` holds their corresponding content as a `vector<string>`. The `switchFile` function facilitates file switching by saving the current file’s state and loading the content of the target file. This design ensures that each file's content is independently managed and can be switched seamlessly.

------

7. **Screen Rendering**
   The `drawScreen` function refreshes the terminal display to show visible portions of the file content, including line numbers. It iterates through the lines stored in `fileContent`, adjusting based on the `scrollOffset`. The `drawStatusBar` function renders a status bar at the bottom, showing the current mode, cursor position, and file information. `ncurses` functions such as `clear` clear the screen, `mvprintw` prints text at specific coordinates, and `refresh` ensures updates are displayed. Color themes for different modes and the status bar are managed using `COLOR_PAIR`.

------











