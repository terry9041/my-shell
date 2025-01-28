# Custom Shell Project  

This project demonstrates a custom shell built in C, showcasing my knowledge of system programming, process management, and command-line interfaces. Initially created as a learning project, the shell implements foreground and background process handling, internal commands, and a command history feature.

---

## Key Features  

### **1. Program Execution**
- **Foreground Mode:** Runs a program in the foreground, waiting for it to finish before accepting another command.  
- **Background Mode:** Runs a program in the background, allowing the shell to continue accepting new commands. Background processes are managed to prevent zombie processes.

### **2. Shell Prompt**
- The shell displays the current working directory as part of the prompt.
  ```bash
  /home/username$
  ```
- Uses the `getcwd()` function to fetch the current directory with proper error handling.

### **3. Built-in Commands**
- `exit`: Terminates the shell.  
- `pwd`: Displays the current working directory.  
- `cd`: Changes the current working directory with the following features:  
  - `~` for the home directory  
  - `-` for switching to the previous directory  
  - Error handling for invalid arguments.  
- `help`: Shows help information for internal commands.  

### **4. Command History**
- Records and displays the 10 most recent commands entered.  
- Implements the `history` command to show these recent commands.  
- Supports these special commands for re-execution:  
  - `!!` to run the last command.  
  - `!n` to execute the nth command from the history list.  

### **5. Signal Handling**
- The shell intercepts `SIGINT` (Ctrl+C) to prevent termination of the shell, displays the help information, and re-displays the prompt.

---

## Sample Usage  

```bash
/home/user$ pwd
/home/user

/home/user$ cd ~/projects
/home/user/projects$

/home/user/projects$ ls &
[Background process started]

/home/user/projects$ history
3  history
2  ls &
1  cd ~/projects
0  pwd

/home/user/projects$ !!  
ls

/home/user/projects$ !1  
cd ~/projects
```

---

## Installation  

1. Clone the repository:  
   ```bash
   git clone https://github.com/terry9041/my-shell.git
   cd my-shell
   ```  

2. Cd into the `src` folder:  
   ```bash
   cd src
   ``` 

3. Compile the project using `make`:  
   ```bash
   make
   ```  
4. Run the shell:  
   ```bash
   ./myshell
   ```  

---

## Project Structure  

- **`myshell.c`**: Core logic for the shell, including parsing, process management, and command execution.  
- **`history.c`**: Implements the history feature and related commands.  
- **`signals.c`**: Signal handling logic for `SIGINT`.  
- **`internal_commands.c`**: Implements the built-in commands like `cd`, `pwd`, and `help`.  
- **`msgs.h`**: Macros for consistent error and help message formatting.  
- **`Makefile`**: Automates the build process.  

---

## Key Concepts Demonstrated  

1. **Process Management**  
   - Use of `fork()`, `exec()`, and `waitpid()` for process handling.  

2. **Signal Handling**  
   - Custom handling for `SIGINT` to prevent shell termination.  

3. **Command Parsing**  
   - Splits user input into commands and arguments.  

4. **Memory Safety**  
   - Proper use of dynamic memory allocation with no memory leaks.  

5. **Error Handling**  
   - Handles invalid inputs, system call failures, and unexpected scenarios gracefully.  

---

## Future Improvements  

- Add support for redirection (`>` and `<`) and piping (`|`).  
- Implement auto-completion for commands and file paths.  
- Add support for scripting and execution of shell scripts.  
- Enhance the prompt with user and hostname information (e.g., `user@hostname:/path$`).  

---

## License  

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Contact  

Feel free to reach out if you have questions, suggestions, or feedback!  



<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
