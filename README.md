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
- Upload to some sort of server
- Let them download