## Function testing

```shell
sudo apt install screenkey
```

We use `screenkey` tool to show our key operations in realtime.

Some functions are **not** convenient to show in the report, but we will show them in the **presentation**.

### Basic function testing

#### 1. Normal mode

type `./vim` in terminal to execute the program in default situation.

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110155122557.png" alt="image-20250110155122557" style="zoom:67%;" />

Because no file is specified to be opened, `output.txt` is created by default.

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110165510218.png" alt="image-20250110165510218" style="zoom:67%;" />



If you open a specific file, such as `o.txt`, you will directly enter `Normal mode`. And the cursor is at the beginning of the line

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110170102509.png" alt="image-20250110170102509" style="zoom:67%;" />



#####  **Line Navigation**:

`$ `**for line  end**

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110170349631.png" alt="image-20250110170349631" style="zoom:67%;" />

`0 ` **for line start**

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110170445623.png" alt="image-20250110170445623" style="zoom:67%;" />

\-`gg` and `G` for **first and last line navigation**

Use the arrow keys to move the cursor to the middle of the paragraph.

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110170644836.png" alt="image-20250110170644836" style="zoom:67%;" />

`gg`

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110170723942.png" alt="image-20250110170723942" style="zoom:67%;" />

`G`

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110170754935.png" alt="image-20250110170754935" style="zoom:67%;" />

#####  **Basic Editing**:

**`dd` Delete current line**

delete the third line

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110170913838.png" alt="image-20250110170913838" style="zoom:67%;" />

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110171018484.png" alt="image-20250110171018484" style="zoom:67%;" />

**`yy` Copy current line and `p `Paste copied line**

Copy the second line and paste. The picture shows the third line becomes `222`.Move the cursor to the beginning of the third line

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110171108180.png" alt="image-20250110171108180" style="zoom:67%;" />

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110171137188.png" alt="image-20250110171137188" style="zoom:67%;" />

#### 2. **Insert Mode and Command Line Mode**

Type `i` to Insert mode. The text color becomes **green**.

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110171626308.png" alt="image-20250110171626308" style="zoom:67%;" />

**Real-time cursor updates during typing**

Type :`this is a test line` in 4th line.

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110172604189.png" alt="image-20250110172604189" style="zoom:67%;" />

**Cursor Movement with Typed Text**

Ability to change cursor position directly using arrow keys

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110172729009.png" alt="image-20250110172729009" style="zoom:67%;" />

Press `ESC` to exit insert mode. Use `:wq` to save directly to exit program `minivim`

<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110173130111.png" alt="image-20250110173130111" style="zoom:67%;" />

Open `o.txt` and find that the text has been completely saved



<img src="C:\Users\25979\AppData\Roaming\Typora\typora-user-images\image-20250110173234022.png" alt="image-20250110173234022" style="zoom:67%;" />