#ifndef _ui_h
#define _ui_h

#include <string>

namespace ui {
	enum BackupChoice {
		HOMEDIR = 1, TMP2 = 2
	};

	void banner();
	BackupChoice askBackupChoice();
	std::string askTmp2Directory();
}

#endif