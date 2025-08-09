# Find My Flags

This challenge serves as a demo of various flag-hiding techniques commonly present in other E&E challenges. It consists of 7 flags which we must find sequentially.

> [!IMPORTANT]
> If you don't have Ghidra installed, aren't sure how to use Ghidra, or don't know how to run the binaries on your system, check out my **Getting Started With Ghidra** guide.

> [!NOTE]
> The Ghidra decompiler output may look slightly different between binaries meant for different operating systems. I will be using the Linux binary throughout this writeup, though the output shouldn't look *too* different where it matters.

First, start Ghidra and open a new or existing project. Then, import your `findmyflags-<os>` binary. As a reminder, you can do this either by dragging the binary itself from File Explorer to the Ghidra Active Project window or by pressing `I` on your keyboard while the window is in focus.

After you have done this, open the imported binary in the CodeBrowser by dragging its item from the project list to the icon of the dragon in the Tool Chest. It will ask you if you would like to analyze; click Yes and then Analyze. It may then ask you if you would like to go to the `main` symbol; click Yes if it does.

Now let us turn our attention to the Decompile pane (the one containing C code). We will mostly be paying attention to this pane as we reverse-engineer the program. Currently, it should be displaying the `main` function. Most programs run starting from the top of `main` going down, exiting when `main` reaches its end. A cursory examination shows us that this `main` function is full of structures such as:

```
__printf_chk(2,"Input Flag 1: ");
get_user_input(user_input,100);
flag_1(user_input);
```

Furthermore, this structure repeats once for each flag!

We can therefore make some educated guesses about what's going on here, based on how the program behaves when we run it. For every flag:
- `__printf_chk(2,"Input Flag <n>: ");` most likely prints to the terminal, requesting user input for the specified flag.
- `get_user_input(user_input,100);` most likely waits for user input.
- `flag_<n>(user_input);` most likely takes user input does something to verify it matches the specified flag.

Now we should take a look at what each `flag_<n>` function does to see what we need to input for each flag.

> [!TIP]
> Double-click on a function in the Decompile pane to jump to its contents. You can use the back arrow in the top left of the screen to return to `main` once you're done.

## Flag 1

<details>
<summary>Click to expand</summary>

Glancing at the `flag_1` function should immediately reveal the flag. This flag has nothing done to it to make it harder to find. All it does is compare the input with the flag and run the `fail` function if they do not match.

</details>

## Flag 2

<details>
<summary>Click to expand</summary>

Near the top of `flag_2`, we can see a function named `base64_decode` being used. Its input is a Base64 string, which we can decode in [CyberChef](https://gchq.github.io/CyberChef/) to find the flag.

</details>

## Flag 3

<details>
<summary>Click to expand</summary>

This one looks a little more complicated, but we need not pay too much attention to the fluff. Towards the end of `flag_3`, we can see that the flag has been broken into 3 pieces. Identifying and putting these 3 pieces together will get us the flag.

</details>

## Flag 4

<details>
<summary>Click to expand</summary>

What an `if` statement! Thankfully, while things might initially look unclear, realizing that it's only comparing the flag character-by-character and extracting each character from the statement is all you need to do.

</details>

## Flag 5

<details>
<summary>Click to expand</summary>

A freebie before we get into some harder ones :\)

</details>

## Flag 6

<details>
<summary>Click to expand</summary>

The most important thing to spot is that this is all a XOR (eXclusive OR) operation on some data. If that sounds unfamiliar, don't worry - it's not as complicated as it sounds. All we need to do is find the XOR key, which in this case is plainly visible as the `0x56` next to the `^` operator (which is C's XOR operator) on line 11 (line number may differ between binaries).

After that, we can plug the contents of `to_xor` into CyberChef with a XOR operation and our found key to get the flag, [like so](https://gchq.github.io/CyberChef/#recipe=Unescape_string()XOR(%7B'option':'Hex','string':'56'%7D,'Standard',false)&input=XHgxNVx4MDVceDAze1x4MDVceDFhXHgxM1x4MTh7YmNgZVY).

This way of hiding the flag is quite common in NCL challenges! Because of this, I've included a line-by-line annotation of the decompiler output below.

```
void flag_6(char *input)
{
	// declaring some variables to use later
	uint uVar1;
	int iVar2;
	char to_xor [14];

	// copy 14 (0xe) bytes of data (in this case represented as an escaped string) to to_xor
	builtin_strncpy(to_xor,"\x15\x05\x03{\x05\x1a\x13\x18{bc`eV",0xe);

	// classic for loop that loops through the 14 (0xe) bytes of data in to_xor...
	for (uVar1 = 0; uVar1 < 0xe; uVar1 = uVar1 + 1) {

		// ... and XOR's each byte by 0x56
		to_xor[(int)uVar1] = to_xor[(int)uVar1] ^ 0x56;

	}

	// finally, the XOR'd result is compared with the user's input
	iVar2 = strcmp(input,to_xor);

	// if they match, return to main and continue the program
	if (iVar2 == 0) {
		return;
	}

	// otherwise, fail and exit
	fail();
}
```

</details>

## Flag 7

<details>
<summary>Click to expand</summary>

This one's the same as `flag_6`, but with a little twist - each character has the number 55 (0x37) subtracted from it before being XOR'd by 0x21.

</details>