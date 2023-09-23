#ifndef _ui_h
#define _ui_h

#include <string>

namespace ui {
	enum BackupType {
		HOMEDIR = 1, TMP2 = 2
	};

	enum CompressMethod {
		ZSTD = 1, ZIP = 2
	};

	void banner();
	BackupType askBackupType();
	CompressMethod askCompressionMethod();
	std::string askTmp2Directory();
}

#endif