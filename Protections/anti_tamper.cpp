#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>
#include <fstream>
#include <filesystem>

#include "Headers/anti_tamper.h"

namespace security
{
	// Flagged file 
	const std::string lock_path = "C:\\example.dat";
	
	bool permanently_lock_system() {
		std::ofstream file(lock_path);
		if (file.is_open()) {
			file << "error_disallowed_software"; // You can put any data here
			file.close();
			return true;
		}
		return false;
	}

	// Call this at the very start of your program
	bool is_system_locked() {
		return std::filesystem::exists(lock_path);
	}

	bool is_debugger_present()
	{
		// Standard checks
		if (IsDebuggerPresent()) return true;

		// Remote debugger check (used by external tools)
		BOOL remote_debug = FALSE;
		CheckRemoteDebuggerPresent(GetCurrentProcess(), &remote_debug);
		return remote_debug;
	}

	bool detect_forbidden_processes()
	{
		// List of disallowed process names
		std::vector<std::wstring> forbidden = { L"ollydbg.exe", L"x64dbg.exe", L"x32dbg.exe"};

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE) return false;

		PROCESSENTRY32W pe;
		pe.dwSize = sizeof(pe);

		if (Process32FirstW(hSnapshot, &pe))
		{
			do
			{
				for (const auto& name : forbidden) {
					if (name == pe.szExeFile) {
						CloseHandle(hSnapshot);
						return true;
					}
				}
			} while (Process32NextW(hSnapshot, &pe));
		}
	}

	bool run_security_checks()
	{
		if (is_debugger_present()) return false;
		if (detect_forbidden_processes()) return false;

		return true;
	}
}