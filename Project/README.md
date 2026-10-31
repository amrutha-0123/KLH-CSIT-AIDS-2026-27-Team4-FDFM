# Linux File Descriptor and File I/O Management System

## Operating Systems and Systems Programming (25CS2104E)

A Linux-based command-line application that demonstrates **file descriptors, Linux system calls, file I/O operations, file offsets, permissions, metadata, and error handling** using C.

---

## Team

| Roll No.   | Name               |
| ---------- | ------------------ |
| 2520080066 | Amrutha Varshini J |
| 2520090125 | V. Hasith Varma    |
| 2520090132 | Keerthana G        |

**Section:** 12
**Team:** 10

---

## Project Overview

In Linux, files are accessed through **file descriptors**, which are integer values maintained by the operating system for open files.

This project provides a menu-driven terminal application that allows users to perform common file operations while directly demonstrating the underlying Linux system calls.

The project focuses on **Operating Systems and Systems Programming concepts** rather than GUI, web, database, or CRUD-based application development.

---

## Features

The application provides the following operations:

1. Create/Open a file
2. Read data from a file
3. Write data to a file
4. Append data to a file
5. Move the file pointer
6. Display file information
7. Close a file
8. Handle file-related errors
9. Exit the application

---

## Linux System Calls / APIs Used

| System Call / API | Purpose                           |
| ----------------- | --------------------------------- |
| `open()`          | Opens or creates a file           |
| `read()`          | Reads data from a file            |
| `write()`         | Writes data to a file             |
| `lseek()`         | Changes the file offset           |
| `close()`         | Closes an open file descriptor    |
| `stat()`          | Retrieves file metadata           |
| `errno`           | Helps identify system-call errors |

---

## Operating System Concepts Demonstrated

* File descriptors
* File tables and kernel-managed resources
* System calls
* User space and kernel space
* File offsets
* Sequential and random file access
* Linux file permissions
* File metadata
* Error handling
* Low-level file I/O

---

## Technologies Used

* **Language:** C
* **Operating System:** Linux / Ubuntu
* **Compiler:** GCC
* **Build Tool:** Make
* **Version Control:** Git / GitHub

---

## Project Structure

```text
Linux-File-IO-Management/
│
├── src/
│   └── main.c
│
├── Makefile
├── README.md
└── screenshots/
```

---

## Compilation and Execution

### 1. Clone the repository

```bash
git clone <YOUR_GITHUB_REPOSITORY_URL>
cd Linux-File-IO-Management
```

### 2. Compile using Makefile

```bash
make
```

### 3. Run the program

```bash
./fileio
```

### 4. Clean compiled files

```bash
make clean
```

---

## Example Output

```text
========================================
 Linux File Descriptor & File I/O System
========================================

1. Create/Open File
2. Read File
3. Write File
4. Append Data
5. Move File Pointer
6. Display File Information
7. Close File
8. Exit

Enter your choice: 1

Enter filename: sample.txt

File opened successfully.
File Descriptor: 3
```

Example read operation:

```text
Enter your choice: 2

Data read from file:
Hello Linux File I/O
```

Example file information:

```text
File Information
----------------
File Size       : 20 bytes
Permissions     : 644
File Descriptor : 3
```

---

## Testing

The system will be tested for:

* Creating a new file
* Opening an existing file
* Reading file contents
* Writing data
* Appending data
* Moving the file pointer
* Displaying file metadata
* Closing file descriptors
* Invalid file names
* Permission-related errors
* Invalid file operations

---

## Learning Outcomes

After completing this project, the team will understand:

* How Linux represents open files using file descriptors
* How user programs request file operations from the kernel
* How system calls perform low-level file I/O
* How file offsets affect read/write operations
* How Linux handles file permissions and metadata
* How errors from system calls are detected and handled

---

## Team Contributions

### Amrutha Varshini J

* Overall system design and integration
* File creation/opening
* File descriptor management

### V. Hasith Varma

* Read/write/append functionality
* File pointer operations
* File I/O testing

### Keerthana G

* File metadata and permissions
* Error handling
* Test cases and documentation

---

## Course

**Operating Systems and Systems Programming (25CS2104E)**
**Academic Year:** 2026–27, Term-I
**Section:** 12
**Team:** 10

---

## License

This project is developed for academic purposes as part of the Operating Systems and Systems Programming course.
