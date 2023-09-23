# Workstation Backup Helper

This shell is used for compressing the data in the workstations.

## Design

- Print a welcome message
- Ask for the directory to backup
	- home directory
	- tmp2
		- Ask the name of the folder
- Ask for the compression method (zip, zstd)
- Compress the file
	- Only include files owned by the user!
- Let them download

## Installation

This project is Arch-only for now.
To install the program, run `make` and `make install`.
If you changed your mind, run `make uninstall`.