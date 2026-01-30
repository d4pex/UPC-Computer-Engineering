# Linux System Administration & Scripting 🐧

## ⚙️ Module Overview
Practical training on managing **Linux Enterprise environments** (Debian/Arch). The focus is on automation, service management, and understanding the boot process. Unlike standard user usage, this module covers the **DevOps** side of Linux: automated backups, compiling software from source, and writing maintenance scripts.

**Red Team Relevance:**
* **Persistence:** Mastering **Systemd Services & Timers** is the standard technique for maintaining access to a compromised machine after a reboot.
* **Privilege Escalation:** Deep understanding of **Sudoers** configuration, file permissions (SUID/SGID), and user groups.
* **Living off the Land:** Using native tools like `rsync`, `tar`, and `Bash` to move data and execute logic without introducing external binaries.

## 📂 Projects & Labs

### 1. 🐍 Automation & Scripting
* **File**: `Lab-Automation-Scripting-Bash-Python.pdf`
* **Tech**: Python 3, Bash.
* **Description**:
    * Developing scripts to detect inactive users and manage disk space.
    * Automating log rotation and system monitoring tasks.

### 2. ⏱️ Service Management & Persistence
* **File**: `Lab-Systemd-Services-Persistence.pdf`
* **Tech**: Systemd, Journalctl.
* **Skills**:
    * Creating custom **Systemd Units** (`.service`) to run background daemons.
    * Configuring **Systemd Timers** (`.timer`) as a modern alternative to Cron jobs for periodic execution.

### 3. 🛠️ Source Compilation & Package Management (Arch Linux)
* **File**: `Project-ArchLinux-Source-Compilation.pdf`
* **Scenario**: Setting up an Arch Linux environment and installing software from source.
* **Skills**:
    * **Compiling from Source**: Using `make`, `configure`, and `gcc` to build binaries (`vitetris`, `nudoku`) directly on the target machine.
    * **Dependencies**: resolving library dependencies manually.

### 4. 🛡️ System Hardening & Maintenance
* **Files**: `Lab-User-Management-Sudoers.pdf`, `Lab-Filesystem-Backup-Rsync.pdf`.
* **Skills**:
    * **Sudoers**: Configuring granular administrative privileges.
    * **Backups**: Implementing incremental snapshots using hard links and `rsync`.

---
*Tech Stack: Debian, Arch Linux, Bash, Python, Systemd, Rsync, GCC.*