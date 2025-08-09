# Find My Flags

This challenge serves as a demo of various flag-obfuscation techniques commonly present in other enumeration and exploitation challenges. It consists of seven increasingly difficult stages, each one hiding a flag required to progress.

> [!IMPORTANT]
> If you don't have Ghidra installed, aren't sure how to use Ghidra, or don't know how to run the binaries, check out the **Getting Started With Ghidra** guide.

## Initial Observations

> [!NOTE]
> There are three binaries available - for Windows, macOS, and Linux. Download the one corresponding to your operating system. They should all run the same, but Ghidra might decompile them slightly differently. This writeup uses the Linux binary (`findmyflags-linux`), as NCL challenges typically use Linux binaries.

Let's start by running our binary and observing its behavior:

```
mini@mindeb:~$ ./findmyflags-linux
Input Flag 1:
```

It seems the program expects some kind of password in the form of a "flag". But how can we determine the correct value?

The sections below will guide you through reverse-engineering this binary to discover the values of those pesky flags.

## Ghidra Setup

Let's open up our binary in Ghidra and get ready to reverse-engineer!

The steps below are a condensed version of those in the **Getting Started With Ghidra** guide. If you're already familiar with how to do this, feel free to skip to the next section.

1. Start Ghidra by navigating to its folder and running `ghidraRun.bat` (Windows) or `ghidraRun` (Linux/MacOS)
2. Make a new project with `Ctrl+N`, or open an existing project if you have a suitable one
3. Import your `findmyflags-<os>` binary, either by dragging the binary itself into the Ghidra Active Project window or by pressing `I` while the window is in focus
4. Open the imported binary in the CodeBrowser by dragging its item from the project list to the icon of the dragon in the Tool Chest
5. Click `Yes` when it asks if you would like to analyze, then click `Analyze` in the window that pops up
6. Once it finishes analyzing (which should only take a couple of seconds), Ghidra should jump to the `main` function. If it doesn't, you can press `G` to open the navigator and manually enter in "main"

You should see a bunch of C code in the Decompile pane on the right half of the screen. The `main` function is where the program starts, and we are now ready to pick apart its internal workings.

## Reverse Engineering

We can see that this `main` function is full of blocks that look like:

```
__printf_chk(2,"Input Flag x: ");
get_user_input(user_input,100);
flag_x(user_input);
```

Furthermore, each block corresponds to a flag, replaced in this snippet with the value x.

We can therefore make some educated guesses about what the program is doing, based on what we saw earlier when we ran it. For every flag... (`//` denotes an annotation):
```
__printf_chk(2,"Input Flag x: "); // Text requesting that the user input the flag is printed to the terminal.
get_user_input(user_input,100); // User input is then retrieved...
flag_x(user_input); // ... and given to a function, which verifies that the correct flag was given
```

We'll now take a look at each `flag_x` function to see what the program expects as input.

> [!TIP]
> In the Decompile pane, double-click on any function name to jump to its contents. Use the back arrow in the top left to return to `main` once you're done.

## Flag 1

<details><summary>Click to expand</summary>

Opening the `flag_1` function should immediately reveal the flag. No obfuscation is used here. All the function does is compare the input with the hardcoded flag and run `fail()` if they do not match.

</details>

## Flag 2

<details><summary>Click to expand</summary>

Near the top of `flag_2`, we can see a function named `base64_decode` being used. Its input is a Base64-encoded string, which we can decode in [CyberChef](https://gchq.github.io/CyberChef/#recipe=From_Base64('A-Za-z0-9%2B/%3D',true,false)) to find the flag.

</details>

## Flag 3

<details><summary>Click to expand</summary>

This function looks more complicated, but most of it is fluff. Towards the end of `flag_3`, we can see that the flag has been broken into three pieces. Identify these pieces and combine them to get the flag.

</details>

## Flag 4

<details><summary>Click to expand</summary>

What an `if` statement! Thankfully, despite the clutter, it's simply a character-by-character comparison of the input. Extract the characters being checked to reconstruct the flag.

</details>

## Flag 5

<details><summary>Click to expand</summary>

The important thing to spot here is that this is all an XOR (eXclusive OR) operation on some data. If that sounds unfamiliar, don't worry - it's not as complicated as it sounds. All we need to do here is find the XOR key, which in this case is visible as the `0x1a` next to the `^` operator (C's XOR operator).

To get the flag, we can plug our found key and the contents of `to_xor` into CyberChef's XOR operation, [like so](https://gchq.github.io/CyberChef/#recipe=XOR(%7B'option':'Hex','string':'1a'%7D,'Standard',false)&input=WUlPN0lWX1Q3IywtLQ).

This way of hiding the flag is quite common in NCL challenges! I've therefore included a line-by-line annotation of the decompiler output below.

```
void flag_5(char *input)
{
	// Declaring some variables to use later
	int iVar1;
	char to_xor [14];

	// Copy 14 (0xe) bytes of data (YIO7IV_T7#,--) to to_xor
	// If you're wondering why it's 14 bytes when there are 13 characters, there's a null terminator
	builtin_strncpy(to_xor,"YIO7IV_T7#,--",0xe);

	// Classic for loop that loops through the 13 (0xd) characters in to_xor...
	for (iVar1 = 0; iVar1 < 0xd; iVar1 = iVar1 + 1) {

		// ... and XOR's each character by 0x1a
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

<details><summary>Click to expand</summary>

This one's just like `flag_5` - but now `to_xor` has been broken into 13 separate characters. Worse, some of the characters aren't even readable ASCII!

To get around this, simply hover over each character and copy down the number under the Decimal column. Once we have collected each number, we can plug them into CyberChef's Decimal converter to restore the [original data](https://gchq.github.io/CyberChef/#recipe=From_Decimal('Space',false)XOR(%7B'option':'Hex','string':''%7D,'Standard',false)&input=MCAxNiAyMiAxMTAgMTYgMTUgNiAxMyAxMTAgMTE5IDExOCAxMTcgMTEy).

Note that while the above recipe has all the steps you need to find the flag, the XOR operation lacks a key...

</details>

## Flag 7

<details><summary>Click to expand</summary>

NCL *loves* combining other operations with XOR to put a little spin on an admittedly easy-to-crack obfuscation method. `flag_7` uses the exact same XOR as `flag_5`, but with an extra addition operation at the end.

As a hint, CyberChef has an ADD operation, which you can use to add to each byte of some data.

</details>

## Flag 8

<details><summary>Click to expand</summary>

```
:)
```

</details>

## Final Notes

Hope you enjoyed! Here's a list of some good things to keep in mind when doing challenges like these.

- Knowing a programming language is quite helpful for these types of challenges, but you can absolutely figure out how things work through research. Even if you know programming, you'll still need to do a lot of research. Never be afraid to look things up, especially functions or sections of code you don't understand!
- Obfuscation and noise go hand in hand. Look for patterns and focus on what's important; see past the noise.
- CyberChef is full of awesome stuff to help with decoding things. And lots more.