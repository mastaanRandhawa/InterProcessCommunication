# Inter-Process Communication and Global Variable Management in C

This project provides an architecture for handling inter-process communication (IPC) and managing global variables in a structured C program. It includes modular components for managing global state, facilitating IPC, and executing core program functionality, supported by a Makefile for streamlined compilation.

## Project Structure

- **`globalVars.c` and `globalVars.h`**: Manage global variables.
  - These files define and manage global variables used across the application, ensuring consistent state access for all components.

- **`ipc.c`**: Handles inter-process communication (IPC).
  - Implements functions for setting up and managing IPC, enabling different parts of the program or separate processes to communicate safely and efficiently.

- **`list.h`**: Defines list structures and prototypes.
  - Provides data structures and function prototypes related to list management, likely used across multiple files to maintain organized data handling.

- **`main.c`**: The main entry point for the program.
  - Orchestrates program execution, initializing global variables, handling IPC operations, and invoking list-related functionality.

- **`Makefile`**: Automates the build process, simplifying compilation and linking. Running `make` compiles all required files and creates an executable for the main program.

## Getting Started

### Prerequisites
- **C Compiler**: GCC or another compatible C compiler.
- **POSIX-compliant system**: Required for IPC functionality (if POSIX IPC mechanisms are used).

### Compilation
In the project directory, run:
```bash
make
```
This will:
  - Compile `globalVars.c`, `ipc.c`, `main.c`, and any list-related code.
  - Link all components to create the main executable.

### Running the Program
Once compiled, start the program by running:
```bash
./main
```

## Usage

This project serves as a template for applications that need centralized global state management and IPC capabilities. With `globalVars` for consistent variable access, `ipc` for process communication, and `list` for data management, it’s suited for systems that handle complex data structures and require efficient inter-process communication.

## Technologies Used

- **C Language**: For low-level control and memory management.
- **POSIX IPC** (if applicable): Supports safe communication across processes.
- **Makefile**: Manages the build process and dependencies.

## Example Use Cases

This project can be adapted for multi-process applications that require shared global variables and inter-process communication, such as distributed computing applications, simulations, or server-client architectures.
