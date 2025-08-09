# Find My Flags

This challenge serves as a demo of various flag-obfuscation techniques commonly present in other enumeration and exploitation challenges. It consists of 7 flags which we must find sequentially.

> [!IMPORTANT]
> If you don't have Ghidra installed, aren't sure how to use Ghidra, or don't know how to run the binaries, check out the **Getting Started With Ghidra** guide.

## Cursory Examination

> [!NOTE]
> There are three binaries for Windows, MacOS, and Linux respectively. Download the one corresponding to your operating system. They should all run the same, but there may be slight differences in how Ghidra decompiles them. I'll be using the Linux binary throughout this writeup (`findmyflags-linux`) since most NCL challenges use Linux binaries.

Let's start by running our binary and seeing what it does. You should see something similar to:

```
mini@mindeb:~$ ./findmyflags-linux
Input Flag 1:
```

It seems the program wants some kind of password in the form of a "flag". But how can we go about finding it?

The next sections will guide you through reverse-engineering this binary to discover the values of those pesky flags.

## Ghidra Setup

Let's open up our binary in Ghidra and get ready to reverse-engineer!

The steps below are a shortened version of those in the **Getting Started With Ghidra** guide. If you're already familiar with how to do this, feel free to skip to the next section.

1. Start Ghidra by navigating to its folder and running `ghidraRun.bat` (Windows) or `ghidraRun` (Linux/MacOS)
2. Make a new project with `Ctrl+N`, or open an existing project if you have a suitable one
3. Import your `findmyflags-<os>` binary, either by dragging the binary itself into the Ghidra Active Project window or by pressing `I` while the window is in focus
4. Open the imported binary in the CodeBrowser by dragging its item from the project list to the icon of the dragon in the Tool Chest
5. Click `Yes` when it asks if you would like to analyze, then click `Analyze` in the window that pops up
6. Ghidra should jump to the `main` function once it's finished analyzing (which should only take a couple seconds), but if it doesn't, you can press `G` to open the navigator and enter in `main` manually

You should now see a bunch of C code in the Decompile pane on the right half of the screen that corresponds to the `main` function of the program. This is where the program starts, and we can now see its internal workings.

## Reverse-Engineering

We can see that this `main` function is full of structures that look like:

```
__printf_chk(2,"Input Flag x: ");
get_user_input(user_input,100);
flag_x(user_input);
```

Furthermore, this structure repeats once for each flag number - replaced in this snippet with the value x.

We can therefore make some educated guesses about what the program is doing, based on what we saw earlier when we ran it. For every flag (`//` denotes an annotation):
```
__printf_chk(2,"Input Flag x: "); // Text requesting that the user input the flag is printed to the terminal.
get_user_input(user_input,100); // User input is then retrieved...
flag_x(user_input); // ... and given to a function, which verifies that the correct flag was given
```

Now we should take a look at what each `flag_x` function does to see what we need to input for each flag.

> [!TIP]
> Double-click on a function in the Decompile pane to jump to its contents. You can then use the back arrow in the top left of the screen to return to `main` once you're done.

## Flag 1

<details>
<summary>Click to expand</summary>

Opening the `flag_1` function should immediately reveal the flag. This flag has nothing done to it to make it harder to find. All it does is compare the input with the flag and run the `fail` function if they do not match.

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

The important thing to spot here is that this is all a XOR (eXclusive OR) operation on some data. If that sounds unfamiliar, don't worry - it's not as complicated as it sounds. All we need to do is find the XOR key, which in this case is plainly visible as the `0x1a` next to the `^` operator (which is C's XOR operator) on line 10 (line number may differ between binaries).

After that, we can plug the contents of `to_xor` into CyberChef with a XOR operation and our found key to get the flag, [like so](https://gchq.github.io/CyberChef/#recipe=XOR(%7B'option':'Hex','string':'1a'%7D,'Standard',false)&input=WUlPN0lWX1Q3IywtLQ).

This way of hiding the flag is quite common in NCL challenges! Because of this, I've included a line-by-line annotation of the decompiler output below.

```
void flag_5(char *input)
{
	// declaring some variables to use later
	int iVar1;
	char to_xor [14];

	// Copy 14 (0xe) bytes of data (YIO7IV_T7#,--) to to_xor
	// If you're wondering why it's 14 bytes when there are 13 characters, there's a null terminator
	builtin_strncpy(to_xor,"YIO7IV_T7#,--",0xe);

	// Classic for loop that loops through the 13 (0xd) characters in to_xor...
	for (iVar1 = 0; iVar1 < 0xd; iVar1 = iVar1 + 1) {

		// ... and XOR's each one by 0x1a
		to_xor[iVar1] = to_xor[iVar1] ^ 0x1a;

	}

	// Finally, the XOR'd result is compared with the user's input
	iVar1 = memcmp(input,to_xor,0xd);

	// If they match, return to main and continue the program
	if (iVar1 == 0) {
		return;
	}

	// Otherwise, fail (which exits the program)
	fail();
}
```

</details>

## Flag 6

<details>
<summary>Click to expand</summary>

This one's just like `flag_5` - but now `to_xor` has now been broken into 13 separate characters. Worse, some of the character's aren't even readable ASCII!

To get around this, simply hover over each character and copy down the number under the Decimal column. Once we have collected each number, we can plug them into CyberChef's Decimal converter to restore the original data in a format that's [easier for processing](https://gchq.github.io/CyberChef/#recipe=From_Decimal('Space',false)XOR(%7B'option':'Hex','string':''%7D,'Standard',false)&input=MCAxNiAyMiAxMTAgMTYgMTUgNiAxMyAxMTAgMTE5IDExOCAxMTcgMTEy).

Note that the XOR operation in the above recipe is missing the key. Can you find it based on how we solved `flag_5`?

</details>

## Flag 7

<details>
<summary>Click to expand</summary>

NCL *loves* combining other operations with XOR to put a little spin on an admittedly easy-to-crack obfuscation method. `flag_7` uses the exact same XOR as `flag_5`, but with an extra addition operation.

As a hint, CyberChef has an ADD operation which you can use to add to each byte of some data.

</details>

## Flag 8

<details>
<summary>Click to expand</summary>

```
:)
```

</details>