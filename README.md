# Workstation Backup Helper

This shell is used for compressing the data in the workstations.

## Installation

This project is Arch-only for now.
To install the program, run `make` and `sudo make install`.
To start the SSH servers, run the following commands:

```bash
sudo systemctl start sshd_fakeshell
sudo systemctl start sshd_sftpjail
```

To uninstall, run `sudo make uninstall`.

## Usage

```bash
ssh -p 2300 user@host
# Interactive program, do whatever it tells you to
sftp -P 2301 user@host
# download the compressed file
```


## Binaries

### FSH -- Fake Shell

Compresses the home directory or /tmp2/ directory of a user. Then, store it somewhere in /tmp2/

### Privlink -- Privieleged Linker

Executed at the end of FSH. Links the compressed file to a read-only directory in /tmp2/.

The sole purpose of this program is to bypass the [SFTP chroot restrictions](https://man.openbsd.org/sshd_config#ChrootDirectory). For security reasons, the path it chroots to cannot be writable by non-root users. However, since the only disk space we can use is /tmp2/, we have to somehow trick SSHD into opening a user-writable directory.

The solution is (not) quite straightforward.
First, we create a directory (the Storage from now on) with permission 1777 in /tmp2/ to store the compressed files.
Then, create another directory (the Archive from now on) with permission 755 in /tmp2/. Each time we create a new compressed file, make a hard link copy in the Archive.
At this point it is not possible for SSHD to chroot into the Archive. Despite its permissions being 755, the parent directory /tmp2/ is still 1777.
Thus, we bind mount the Acrhive to another directory (the Fake Root from now on) in "/". This way, sshd will assume that the Fake Root has 755 permission and is not writable. Users can then access the SFTP server and download the compressed data.