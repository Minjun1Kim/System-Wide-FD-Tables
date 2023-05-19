#include "a2.h"

// displays composite table
void printComposite(c_queue *q){
    int i=0;
    printf("\t %s\t%s\t%s\t\t%s\n\t %s\n", "PID", "FD", "Filename", "Inode", "==============================================="); //print header

    for(c_table *tr = q->head; tr; tr=tr->next, i++){ //iterate through the queue
        printf(" %d\t%d\t%d\t%s\t%lu\n", i, tr->pid, tr->fd, tr->fname, tr->inode); //print each table entry (PID, FD, Filename, Inode) with its corresponding index
    }

    printf("\t%s\n\n","==============================================="); //print footer
}


// displays per-process table
void printPerProcess(c_queue *q){

    printf("\t %s\t%s\n\t%s\n", "PID", "FD", "============"); //print header

    for(c_table *tr = q->head; tr; tr=tr->next) //iterate through the queue
        printf("\t %-8d%-8d\n", tr->pid, tr->fd); //print each table entry (PID, FD)
    
    printf("\t============\n\n"); //print footer
}


// displays system-wide table
void printSystemWide(c_queue *q){

    printf("\t %s\t%s\t%s\n\t%s\n", "PID", "FD", "Filename", "========================================"); //print header

    for(c_table *tr = q->head; tr; tr=tr->next) //iterate through the queue
        printf("\t %d\t%d\t%s\n", tr->pid, tr->fd, tr->fname); //print each table entry (PID, FD, Filename)
    
    printf("\t========================================\n\n"); //print footer
}


// displays Vnodes table
void printVNodes(c_queue *q){

    printf("\t  %s\t\t%s\n\t%s\n", "FD", "Inode", "========================================"); //print header

    for(c_table *tr = q->head; tr; tr=tr->next) //iterate through the queue
        printf("\t  %d\t\t%lu\n", tr->fd, tr->inode); //print each table entry (FD, Inode)
    
    printf("\t========================================\n\n"); //print footer
}

// displays the offending processes
void printThreshold(c_queue *threshold_q){

    printf("## Offending processes:\n");

    if(threshold_q->head == NULL || threshold_q->tail == NULL){
        return;
    }

    for(c_table *temp = threshold_q->head; temp != NULL; temp = temp->next){
        if(temp->next != NULL)
            printf("%d (%d), ", temp->pid, temp->fd_count);
        else
            printf("%d (%d)\n", temp->pid, temp->fd_count);
    }
}

// displays the table based on the arguments given
void printTable(c_queue *q, int arg_pid, int argc, bool per_process, bool systemWide, bool v_nodes, bool composite){
    int i=0;
    
    // If the target PID is given as a program argument, print it as the "Target PID"
    if(arg_pid>0){
        printf(">>> Target PID: %d\n", arg_pid);
        !composite && argc==2? composite=true : composite; //if only one argument is given (PID), print composite table for the Target PID
    }

    if (per_process) //if --per-process table is an argument, print the process FD table
        printPerProcess(q);
    if(systemWide) //if --systemWide table is an argument, display the system-wide FD table
        printSystemWide(q);
    if(v_nodes) //if --Vnodes is an argument, display Vnodes FD table
        printVNodes(q);
    if(composite || argc==1) //if --composite is an argument or no arguments are given, print composite table
        printComposite(q);
    
}