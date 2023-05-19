#ifndef __A2_header
#define __A2_header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <getopt.h>
#include <stdint.h>

#define MAX_LEN 1024
#define NOTHING -1

extern struct option long_options[];

/**
 *  @brief Represents a table entry in the File Descriptor tables.
 *  stores information about a process's PID, file descriptors, file descriptor count, file name, and inode number.
**/
typedef struct table{
    pid_t pid; // process id
    int fd; // file descriptor
    int fd_count;  // number of file descriptors associated with the process
    char fname[MAX_LEN];    // symbolic link path name or canonical path name
    unsigned long inode;    // inode number
    struct table *next; // pointer to next node
} c_table;


/**
 *  @brief Represents a queue of c_table nodes.
 *  stores pointers to the head and tail of the queue.
**/
typedef struct queue {
    c_table *head;  // pointer to head of queue
    c_table *tail;  // pointer to tail of queue
} c_queue;


/**
 *  @brief Allocate and initialize a new queue.
 *  @param None
 *  @return c_queue* a pointer to the new queue.
**/
c_queue *setUp(void);


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


/**
 *  @brief Dequeue a c_table node at the head of the c_queue 'q' in O(1) time
 *  @param q a pointer to the c_queue 'q'
 *  @return None
**/
void dequeue(c_queue *q);


/**
 *  @brief Free all nodes in the queue as well as the queue itself
 *  @param q a pointer to the c_queue 'q'
 *  @return c_queue*
**/
c_queue *deleteAll(c_queue *q);


/**
 *  @brief Display the contents of a composite table from the queue 'q' to the console. 
 *  It first prints a header with column labels, then iterates through the queue and prints each table entry (PID, FD, Filename, Inode) with its corresponding index.
 *  Finally, it prints a footer consisting of a horizontal line to separate the output from other console output.
 * 
 *  @param q a pointer to a c_queue struct, representing the queue to be printed
 *  @return void
**/
void printComposite(c_queue *q);


/**
 *  @brief Display the contents of a per-process table from the queue 'q' to the console. 
 *  It first prints a header with column labels, then iterates through the queue and prints each table entry (PID, FD).
 *  Finally, it prints a footer consisting of a horizontal line to separate the output from other console output.
 * 
 *  @param q a pointer to a c_queue struct, representing the queue to be printed
 *  @return void
**/
void printPerProcess(c_queue *q);


/**
 *  @brief Display the contents of a systemWide table from the queue 'q' to the console. 
 *  It first prints a header with column labels, then iterates through the queue and prints each table entry (PID, FD, Filename).
 *  Finally, it prints a footer consisting of a horizontal line to separate the output from other console output.
 * 
 *  @param q a pointer to a c_queue struct, representing the queue to be printed
 *  @return void
**/
void printSystemWide(c_queue *q);


/**
 *  @brief Display the contents of a Vnodes table from the queue 'q' to the console.
 *  It first prints a header with column labels, then iterates through the queue and prints each table entry (FD, Inode).
 *  Finally, it prints a footer consisting of a horizontal line to separate the output from other console output.
 *
 *  @param q a pointer to a c_queue struct, representing the queue to be printed
 *  @return void
**/
void printVNodes(c_queue *q);

/**
 *  @brief Display processes which have a number of FD assigned larger than X in the output.
 *  will list the PID and number of assigned FDs, e.g. PID (FD)  
 *  @param threshold_q a pointer to a c_queue struct, representing the queue to be printed
 *  @return void
**/
void printThreshold(c_queue *threshold_q);

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

/**
 *  @brief get the inode number of the symbolic link 'pathLink'
 *  @param pathLink a pointer to a char array, representing the symbolic link path name
 *  @return ino_t the inode number of the symbolic link
**/
ino_t getInode(char *pathLink);


/**
 * @brief return the user ID of a given process ID
 * takes a process ID 'pid' as input and returns the user ID of the corresponding process.
 * 
 * @param pid the process ID to look up
 * @return int the user ID of the process, or -1 if there was an error opening the file or finding the UID line
**/
int getProcessUid(pid_t pid);

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


/**
 *  @brief Writes the contents of a c_queue* queue to a text file in composite Table format.
 *  The text format includes a header and a footer, with each row of the table containing fields (PID, FD, Filename, Inode) with its corresponding index.
 * 
 *  @param q A pointer to a c_queue* queue.
 *  @return void.
**/
void write_txt(c_queue *q);


/**
 *  @brief Writes a binary representation of the composite table to a file named "compositeTable.bin".
 *  The binary format includes a header and a footer, with each row of the table containing fields (PID, FD, Filename, Inode) with its corresponding index.
 * 
 *  @param q A pointer to the c_queue struct that holds the composite table to be written to file.
 *  @return void
**/
void write_bin(c_queue *q);


#endif