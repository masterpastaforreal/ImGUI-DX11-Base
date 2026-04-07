#pragma once

namespace security
{
	// Runs secuirty checks - returns true if everything is safe
	bool run_security_checks();

	// Specific checks for debuggers
	bool is_debugger_present();

	// Checks for common third party disallowed programs
	bool detect_forbidden_processes();

	bool permanently_lock_system();
	bool is_system_locked();
}