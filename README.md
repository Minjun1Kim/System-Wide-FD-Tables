# <span style="color:#ADD8E6">Assignment 2: Recreating the System-Wide FD Tables
</span>


<div align="right"> Created by Minjun Kim </div>


## <span style="color:#ADD8E6">Table of Contents </span> 
- [Description](#description)
- [Requirements](#requirements)
- [Usage](#usage)
- [Functions](#functions)
- [Problem Solving](#problemsolving)
- [Notes](#notes)

<br />

<a id="description"></a>
## <span style="color:#ADD8E6">Description </span> 

- This program displays the tables used by the OS to keep track of open files, assignation of File Descriptors (FD) and processess. 
- The FD Tables include process FD table, system-wide FD table, Vnodes FD table, composed table for all the currently processes belong to the current user executing the program. 
- Allows the user to enter a threshold value to indicate the display of the selected processes with the number of file descriptors greater than the threshold in the format PID (FD) below any FD tables indicated by the user.
- Allows the user to enter a specific process id number (PID), and displays tables including the target process.


<br />

<a id="requirements"></a>
## <span style="color:#ADD8E6">Requirements </span>

The program requires the following libraries to be installed: <br />
- `<stdio.h>`<br />
- `<stdlib.h>`<br />
- `<string.h>`<br />
- `<stdbool.h>` <br />
- `<sys/types.h>` <br />
- `<sys/stat.h>` <br />
- `<dirent.h>`<br />
- `<unistd.h>`<br />
- `<errno.h>`<br />
- `<ctype.h>`<br />
- `<getopt.h>`<br />
- `<stdint.h>`<br />

and the header file: <br />

- `"a2.h'`

as well as the implementation files: <br />

- `main.c`
- `queue.c`
- `printTables.c`
- `a2.c`
- `writeOutput.c`

<br />

also includes:
- `Makefile`

<a id="usage"></a>
## <span style="color:#ADD8E6">Usage</span> 

To run the program, compile the code using the following Makefile command:

```console
$ make
```

And then run the executable (with additional arguments if necessary):

```console
$ ./prog
```

The program also accepts several command line arguments such as: <br />

  `--per-process`
<details>
    <summary>Click to expand</summary>

```console
$ ./prog --per-process
```

* to indicate that only the process FD table will be displayed
</details>

<br />

`--systemWide`
<details>
    <summary>Click to expand</summary>

```console
$ ./prog --systemWide
```

* to indicate that only the system-wide FD table will be displayed
</details>

<br />
  
`--Vnodes`

<details>
  <summary>Click to expand</summary>

```console
$ ./prog --Vnodes
```

  * to indicate that the Vnodes FD table will be displayed

</details>

<br />

`--threshold=X`

<details>
  <summary>Click to expand</summary>

```console
$ ./prog --threshold=X
```

  * to indicate that processes which have a number of FD assigned larger than X should be flagged in the output.
</details>

<br />


<details>
  <summary>No argument</summary>

```console
$ ./prog
```

  * if no arguments given, the program will display the composite table, i.e. same effect as having used the `--composite` flag

</details>

<br />


<details>
  <summary>Multiple Arguments</summary>

```console
$ ./prog --composite --Vnodes 19803
 
```
 * displays all the tables indicated
 * prints the composite table of PID=19803 and the Vnodes table of PID=19803
<br />

</details>



<br />

Extra CDTs Used
-   `c_table` 
    <br />

    ```c
    typedef struct table{
        pid_t pid; // process id
        int fd; // file descriptor
        int fd_count;  // number of file descriptors associated with the process
        char fname[MAX_LEN];    // symbolic link path name or canonical path name
        unsigned long inode;    // inode number
        struct table *next; // pointer to next node
    } c_table;
    ```
- `c_queue`
    <br />

    ```c
    typedef struct queue {
        c_table *head;  // pointer to head of queue
        c_table *tail;  // pointer to tail of queue
    } c_queue;
    ```


<a id="functions"></a>
## <span style="color:#ADD8E6">Functions</span>

    
<br />

- defined in `queue.c`

    -   ```c

        /**
        *  @brief return a pointer to a newly dynamically allocated queue and sets the head and tail to NULL.
        *  @param None
        *  @return c_queue* a pointer to the new queue.
        **/
        c_queue *setUp(void);

        ```


    -   ```c
        /**
        *  @brief Enqueue a newly allocated and initialized c_table node to the tail of the c_queue 'q' in O(1) time
        *  inialize the node's fields with the given parameters, 'pid', 'fd', 'fd_count', 'fname', 'inode'
        * 
        *  @param q a pointer to the queue
        *  @param pid process id
        *  @param fd file descriptor
        *  @param fd_count number of file descriptors associated with the process
        *  @param fname symbolic link path name or canonical path name
        *  @param inode inode number
        * 
        *  @return None
        **/
        void enqueue(c_queue *q, pid_t pid, int fd, int fd_count, char *fname, unsigned long inode);
        ```

    -   ```c
        /**
        *  @brief Dequeue a c_table node at the head of the c_queue 'q' in O(1) time
        *  @param q a pointer to the c_queue 'q'
        *  @return None
        **/
        void dequeue(c_queue *q);
        ```

    -   ```c
        /**
        *  @brief Free all nodes in the queue as well as the queue itself
        *  @param q a pointer to the c_queue 'q'
        *  @return c_queue*
        **/
        c_queue *deleteAll(c_queue *q);
        ```
 
 <br />
 <br />

- Implemented/defined in `printTables.c`

    -   ```c

        /**
        *  @brief Display the contents of a composite table from the queue 'q' to the console. 
        *  It first prints a header with column labels, then iterates through the queue and prints each table entry (PID, FD, Filename, Inode) with its corresponding index.
        *  Finally, it prints a footer consisting of a horizontal line to separate the output from other console output.
        * 
        *  @param q a pointer to a c_queue struct, representing the queue to be printed
        *  @return void
        **/
        void printComposite(c_queue *q);
        ```
    

    -   ```c
        /**
        *  @brief Display the contents of a per-process table from the queue 'q' to the console. 
        *  It first prints a header with column labels, then iterates through the queue and prints each table entry (PID, FD).
        *  Finally, it prints a footer consisting of a horizontal line to separate the output from other console output.
        * 
        *  @param q a pointer to a c_queue struct, representing the queue to be printed
        *  @return void
        **/
        void printPerProcess(c_queue *q);
        ```

    -   ```c
        /**
        *  @brief Display the contents of a systemWide table from the queue 'q' to the console. 
        *  It first prints a header with column labels, then iterates through the queue and prints each table entry (PID, FD, Filename).
        *  Finally, it prints a footer consisting of a horizontal line to separate the output from other console output.
        * 
        *  @param q a pointer to a c_queue struct, representing the queue to be printed
        *  @return void
        **/
        void printSystemWide(c_queue *q);
        ```

    -   ```c
        /**
        *  @brief Display the contents of a Vnodes table from the queue 'q' to the console.
        *  It first prints a header with column labels, then iterates through the queue and prints each table entry (FD, Inode).
        *  Finally, it prints a footer consisting of a horizontal line to separate the output from other console output.
        *
        *  @param q a pointer to a c_queue struct, representing the queue to be printed
        *  @return void
        **/
        void printVNodes(c_queue *q);
        ```

    -   ```c
        /**
        *  @brief Display processes which have a number of FD assigned larger than X in the output.
        *  will list the PID and number of assigned FDs, e.g. PID (FD)  
        *  @param threshold_q a pointer to a c_queue struct, representing the queue to be printed
        *  @return void
        **/
        void printThreshold(c_queue *threshold_q);
        ```

    -   ```c
        /**
        *  @brief Print information about the contents of a c_queue, depending on the input arguments.
        * 
        *  @param q a pointer to a c_queue struct, representing the queue to be printed
        *  @param arg_pid an integer representing the target PID (if any)
        *  @param argc an integer representing the number of command line arguments
        *  @param per_process a boolean variable indicating whether to print the per-process table
        *  @param systemWide a boolean variable indicating whether to print the system-wide table
        *  @param v_nodes a boolean variable indicating whether to print the virtual node table
        *  @param composite a boolean variable indicating whether to print the composite table
        *  @param q a pointer to a c_queue struct, representing the queue to be printed
        *  @param arg_pid an integer representing the target PID (if any)
        *
        *  @return void
        **/
        void printTable(c_queue *q, int arg_pid, int argc, bool per_process, bool systemWide, bool v_nodes, bool composite);
        ```

<br />

- Implemented/defined `a2.c`

    -   ```c
        /**
        *  @brief Return the determined length of a path string based on flags and input values.
        *  takes in a process ID 'pid' and a directory name and returns the length of the corresponding path string.
        * 
        *  @param pid the process ID to include in the path string
        *  @param d_name the directory name to include in the path string
        *  @param pid_flag a flag indicating whether the pid should be included in the string
        *  @param dname_flag a flag indicating whether the directory name should be included in the string
        *
        *  @return the length of the path string, or -1 if there was an error with snprintf
        **/
        int checkPathLength(pid_t pid, char *d_name, bool pid_flag, bool dname_flag);
        ```
    

    -   ```c
        /**
        *  @brief get the inode number of the symbolic link 'pathLink'
        *  @param pathLink a pointer to a char array, representing the symbolic link path name
        *  @return ino_t the inode number of the symbolic link
        **/
        ino_t getInode(char *pathLink);
        ```

    -   ```c
        /**
        * @brief return the user ID of a given process ID
        * takes a process ID 'pid' as input and returns the user ID of the corresponding process.
        * 
        * @param pid the process ID to look up
        * @return int the user ID of the process, or -1 if there was an error opening the file or finding the UID line
        **/
        int getProcessUid(pid_t pid);
        ```

    -   ```c
        /**
        *   @brief Handles the processes in the "/proc" directory with the specified parameters.
        * 
        *   opens the "/proc" directory, iterates over each process and handles it according to the specified parameters.
        *   checks the user id of each process and sets the values for the process's file descriptors (fds).
        *   If an argument pid is provided, it only handles the process with that pid.
        *   If a threshold is provided, it enqueues processes with a number of fds greater than the threshold in a separate queue.
        * 
        *   @param q A pointer to a c_queue struct representing the main queue for handling processes.
        *   @param arg_pid An integer representing the pid to handle (if provided).
        *   @param threshold An integer representing the threshold for the number of fds a process must have to be enqueued in the threshold queue (if provided).
        *   @param threshold_q A pointer to a c_queue struct representing the queue for processes with a number of fds greater than the threshold.
        *   
        * @return void.
        **/
        void handleProcesses(c_queue *q, int arg_pid, int threshold, c_queue *threshold_q);
        ```

    -   ```c
        /**
        *  @brief Return the number of file descriptors for a given process ID 
        *  This function opens the /proc/<pid>/fd directory and retrieves the file descriptors for the specified
        *  process pid. It then enqueues the relevant information to the given c_queue pointer q.
        * 
        *  @param q A pointer to the c_queue where the file descriptors will be enqueued.
        *  @param pid The process ID whose file descriptors will be retrieved.
        *  @return the number of file descriptors enqueued to q. If an error occurs, returns -1.
        **/
        int handleFDs(c_queue *q, pid_t pid);
        ```

<br />

- Implemented/defined in `writeOutput.c`


    -   ```c
        /**
        *  @brief Writes the contents of a c_queue* queue to a text file in composite Table format.
        *  The text format includes a header and a footer, with each row of the table containing fields (PID, FD, Filename, Inode) with its corresponding index.
        * 
        *  @param q A pointer to a c_queue* queue.
        *  @return void.
        **/
        void write_txt(c_queue *q);
        ```

    -   ```c
        /**
        *  @brief Writes a binary representation of the composite table to a file named "compositeTable.bin".
        *  The binary format includes a header and a footer, with each row of the table containing fields (PID, FD, Filename, Inode) with its corresponding index.
        * 
        *  @param q A pointer to the c_queue struct that holds the composite table to be written to file.
        *  @return void
        **/
        void write_bin(c_queue *q);
        ```

<br />

<a id="problemsolving"></a>
## <span style="color:#ADD8E6">Problem Solving</span>
- How did I solve the problem(s)?

    - The first of many problems I had over the course of the assignment was finding the right functions from the libraries to use. Every office hours I had attended, I found out how to retrieve specific data part of the PID table. Although it was not a quick process, I was able to make sense of all the man pages and use the required functions in my program to retrieve process information to display in my output.
    
    - Secondly, I was slightly stuck on how to efficiently store the process table data. Then I came up with the idea of using the heap with O(1) insertion to the tail of the queue. I recalled my memory of implementing it in the course CSCA48 to successfully come up with the algorithm.

    - Additionally, I was a bit stuck on how to approach the problem of parsing the command line arguments and postional arguments. Fortunately, my TA suggested the idea of using getopt_long which eliminated all the problems I'd had with the command line arguments.
    
    - Most importantly, I approached most problems I'd encountered by analyzing the behaviour of the program provided in Marcelo's video and brainstorming about how to solve the problem in my head before starting coding. For example, for the threshold argument I came up with the solution of storing the number of file descriptors in the c_table node.

<a id="notes"></a>
## <span style="color:#ADD8E6">Notes</span>

- The utility only works on Linux systems.
- Note please read the pdf file "a2Bonus.pdf" for the bonus component.
- The program uses the files `/proc/`, `/proc/[PID]/`, and `/proc/[PID]/fd` to obtain information about the system including currently running processes, specified process, and file descriptors. The files are constantly updated by the system, so the information displayed may change over time.
- The code assumes that the --threshold=X command option requires an optional argument X, which is of type int.

<a id="contact"></a>
## <span style="color:#ADD8E6">Contact</span>
- creator: Minjun Kim
- email address: minjunn.kim@mail.utoronto.ca

