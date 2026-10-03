#pragma once

namespace autorun {

	bool CreateStartupShortcut();

	bool IsStartupShortcutPresent();
	bool RemoveStartupShortcut();

}

extern "C" bool CreateStartupShortcut_C();
extern "C" bool IsStartupShortcutPresent_C();
extern "C" bool RemoveStartupShortcut_C();
