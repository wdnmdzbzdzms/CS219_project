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





