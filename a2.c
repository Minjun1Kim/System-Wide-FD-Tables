#include "a2.h"


// return the determined length of a path string based on flags and input values. 
int checkPathLength(pid_t pid, char *d_name, bool pid_flag, bool dname_flag){

    int len = -1;
    //uses snprintf to construct the path name string based on whether the pid_flag and dname_flag are set or not.
    //stores the length of the path name string in len
    if(pid_flag && dname_flag){
        len = snprintf(NULL, 0, "/proc/%d/fd/%s", pid, d_name); 
    } else if(pid_flag && !dname_flag){
        len = snprintf(NULL, 0, "/proc/%d/fd", pid);
    } else if(!pid_flag && dname_flag){
        len = snprintf(NULL, 0, "/proc/fd/%s", d_name);
    } else if(!pid_flag && !dname_flag){
        len = snprintf(NULL, 0, "/proc/fd"); 
    }

    if(len < 0){ //returns -1 if there was an error with snprintf
        //perror("Error with snprintf");
        return -1;
    }
    return len + 1; //returns length of path name string + 1 (for NULL terminator)
}


// return the inode of the file pointed to by the symbolic link 'pathLink'
ino_t getInode(char *pathLink){
    struct stat info; //stat struct

    if(stat(pathLink, &info) == -1){ //stat returns -1 on error
        //perror("Error with stat");
        return UINTMAX_MAX; //returns max value of unsigned int
    }
    return info.st_ino; //returns inode number
}


// return the user ID of a given process ID
int getProcessUid(pid_t pid){
    char pathName[MAX_LEN] = {'\0'}; //initializes all elements of pathName to NULL
    snprintf(pathName, sizeof(pathName), "/proc/%d/status", pid); //writes formatted output /proc/[pid]/status to pathName
    FILE *file = fopen(pathName, "r"); //opens file specified by pathName

    if(!file){ //return -1 if unable ot open file
        //perror("Error opening file");
        return -1;
    }

    memset(pathName, 0, sizeof(pathName)); //clears pathName (sets all elements to NULL
    uid_t uid = (uid_t)-1; //initializes uid to -1

    while(fgets(pathName, MAX_LEN, file)){ //reads line from file and stores in pathName
        if(strncmp(pathName, "Uid:", 4)==0){ //if line starts with "Uid:"
            uid = (uid_t)atoi((pathName+5)); //converts string to int and stores in uid
            break;
        }
    }

    fclose(file); //closes file
    return uid; //returns uid
}


// handles the processes in the "/proc" directory with the specified parameters.
void handleProcesses(c_queue *q, int arg_pid, int threshold, c_queue *threshold_q){
    DIR *directory = opendir("/proc"); //opens the /proc directory
    int num_fds=0; //number of file descriptors

    if(!directory){ //return early if unable to open directory
        perror("Error opening directory: \"proc\""); //prints error message
        return; 
    }

    uid_t myUserID = getuid(); //get current user id
    char d_name[MAX_LEN]={'\0'}; //initializes all elements of d_name to NULL

    for(struct dirent *entry=NULL; (entry = readdir(directory)) != NULL;){ //go through each process and find only the processes run by the current user
        strcpy(d_name, entry->d_name); //copies d_name to entry->d_name

        if(!isdigit(*(d_name)) || strcmp(d_name, ".") == 0 || strcmp(d_name, "..") == 0) //skips the (irrelevant) current and parent directories and non-processes.
            continue;
        
        int pid = atoi(d_name); //converts d_name string to int and stores in pid
        int process_uid = getProcessUid(pid); //gets the user id of the process

        if(process_uid == -1) //if process_uid is -1, skip to next process
            continue;
        if(myUserID == process_uid){ //if the process is run by the current user, add it to the queue
            if(arg_pid>0 && arg_pid != pid) //if the optional argument Target PID was given and the pid does not match the argument, skip to next process
                continue;

            num_fds=handleFDs(q, pid); //sets the values of the queue

            if(threshold > 0 && num_fds > threshold){ //if the optional argument Threshold was given and the number of file descriptors is greater than the threshold, add it to the threshold queue
                char tempStr[1]="\0"; //initializes all elements of tempStr to NULL
                enqueue(threshold_q, pid, NOTHING, num_fds, tempStr, 0); //adds the process to the threshold queue
            }
        }
    }

    closedir(directory); //closes the directory
}


// return the number of file descriptors for a given process ID and handle the file descriptors in the "/proc/[pid]/fd" directory with the specified parameters. 
int handleFDs(c_queue *q, pid_t pid){
    
    int len=-1, count=0; //len is the length of the path name string, count is the number of file descriptors
    char d_name[MAX_LEN]={'\0'}; //initializes all elements of d_name to NULL
    
    len = checkPathLength(pid, NULL, true, false); //checks the length of the path name string
    if(len<0) return -1; //returns -1 if there was an error with checkPathLength

    char pathName[len]; //a string variable to store path of a file directory

    memset(pathName, 0, sizeof(pathName)); //sets buf to all 0 before reading
    snprintf(pathName, len, "/proc/%d/fd", pid); //formats the path and stores in pathName
    
    DIR *directory = opendir(pathName); //opens the directory

    if(!directory){ //return early if unable to open directory
        return -1;
    }

    //go through each process and find only the processes run by the current user

    for(struct dirent *entry=NULL; (entry = readdir(directory)) != NULL;){
        strcpy(d_name, entry->d_name); 
        if(strcmp(d_name, ".") == 0 || strcmp(d_name, "..") == 0) 
            continue; //skips the (irrelevant) current and parent directories.
        
        len = checkPathLength(pid, d_name, true, true); //checks the length of the path name string
        if(len<0) continue; //returns -1 if there was an error with checkPathLength
        
        char path_d[len]; //a string variable to store path of a file directory
        memset(path_d, 0, sizeof(path_d)); //sets buf to all 0 before reading a link to prevent existence of leftover data
        snprintf(path_d, len, "/proc/%d/fd/%s", pid, d_name); //formats the path and stores in path_d

        ino_t inode = getInode(path_d); //gets the inode number of the file

        if(inode == UINTMAX_MAX) //if stat() failed and inode is max value of unsigned int, skip to next process
            continue;
        
        char link[MAX_LEN] = {'\0'}; //initializes all elements of link to NULL
        memset(link, 0, sizeof(link)); //sets buf to all 0 before reading a link to prevent existence of leftover data
        
        if (readlink(path_d, link, sizeof(link)) == -1) { //reads the link and stores in link
            //perror("Error: Failed to read link");
            continue;
        }
        
        enqueue(q, pid, atoi(d_name), NOTHING, link, inode); //adds the process to the queue
        count++; //increments count
            
    }

    closedir(directory); //closes the directory
    return count; //returns the number of file descriptors
}


