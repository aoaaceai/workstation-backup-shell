#include <iostream>
#include "ui.h"
#include "customException.h"
using namespace std;

void backupHomedir() {

}

void backupTmp2() {
	
}

int main() {
	try {
		ui::banner();
		ui::BackupChoice choice = ui::askBackupChoice();
		switch (choice) {
		case ui::HOMEDIR:
			backupHomedir();
			break;
		case ui::TMP2:
			backupTmp2();
			break;
		}
		if (choice == ui::BackupChoice::TMP2) {
			ui::askTmp2Directory();
		}
	}
	catch (std::exception &e) {
		cout << "Exception caught: " << e.what() << endl;
	}
}