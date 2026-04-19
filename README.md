# CS2 Beginners External Hack

A simple external cheat for Counter-Strike 2, designed for beginners to learn game hacking concepts. This project does **not** include anti-cheat bypasses.

### Features
- **Aimbot**: Customizable FOV and target bone (Head, Chest, Stomach).
- **Visuals (ESP)**: Box ESP, Skeleton ESP, and Health bars.
- **Kernel Memory Access**: Includes a `KMDF Driver` to demonstrate reading/writing memory from the kernel.
- **Customizable FOV**: Adjust your in-game field of view.
- **Misc**: Spinbot (experimental) and developer debug views.

### Technical Overview
- **Language**: C++
- **GUI Framework**: Dear ImGui
- **Memory Management**: KMDF Driver (Kernel-mode)
- **Offsets**: Uses dynamic offsets which can be updated via tools like [cs2-dumper](https://github.com/a2x/cs2-dumper).

### Key Offsets (Example)
- Base Address: `cheat::clientAddress + 0x1836BB8`
- View Matrix: `0x1A33E30`
- Local Player Pawn: `0x1836BB8`

### How to use
1. Compile the solution using Visual Studio.
2. Ensure you have the `client.dll` offsets updated in the source.
3. Run the compiled executable as **Administrator**.
4. Press **INS (Insert)** to toggle the menu.

### Development Notes
To find player coordinates:
1. Find enemy health addresses (usually narrowing down to 4 addresses).
2. Look for the one with `0x344` offset.
3. Perform a pointer scan on this address.
4. Filter by value `100` (4 bytes) after restarting the match.
5. Common offsets: Offset 1: `0x10`, Offset 2: `0x0`, Offset 3: `0x1C8`, Offset 4: `0x344`.

### Console Commands
- `bot_kick`: Kick bots to test features more easily.

---
*Disclaimer: This is for educational purposes only. Modifying game memory can result in account bans.*
