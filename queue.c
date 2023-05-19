#include "a2.h"

// return a pointer to a newly allocated queue
c_queue *setUp(void){
    c_queue *q = (c_queue *)calloc(1, sizeof(c_queue)); // allocate 1 memory of sizeof(c_queue)
    if(q) q->head = q->tail = NULL; // set head and tail to NULL upon successful allocation

    return q; // return pointer to queue
}


//enqueue a new node to the tail of the queue 'q' in O(1) time
//inialize the node's fields with the given parameters, 'pid', 'fd', 'fd_count', 'fname', 'inode'

void enqueue(c_queue *q, pid_t pid, int fd, int fd_count, char *fname, unsigned long inode){

    c_table *node = (c_table *)calloc(1, sizeof(c_table)); // allocate 1 memory of sizeof(c_table)

    if(node==NULL) return;  // if allocation fails, return early

    // initialize node's fields
    node->pid = pid;   
    node->fd = fd;
    node->fd_count = fd_count;
    strcpy(node->fname, fname);
    node->inode = inode;
    node->next = NULL;

    if(q->head == NULL){ // if queue is empty, set head and tail to new node
        q->head = node; 
        q->tail = node;
    }else{ // otherwise, set tail's next to new node and set tail to new node
        q->tail->next = node;
        q->tail = node;
    }
}

// dequeue a node from the head of the queue 'q' in O(1) time
void dequeue(c_queue *q){
    if(q->head == NULL || q->tail==NULL) return; // if queue is empty, return
    
    c_table *temp = q->head; // store the queue's head in temp

    if(q->head == q->tail){ // if there's only one node in the queue, set head and tail to NULL
        q->head = q->tail = NULL; 
    } else { // otherwise set head to next node
        q->head = q->head->next; 
    }
    free(temp); // free the head node
}

// free all nodes in the queue as well as the queue itself
c_queue *deleteAll(c_queue *q){
    if(q->head == NULL || q->tail==NULL) return NULL; // if queue is empty, return NULL
    
    c_table *temp = q->head; // store the queue's head in temp

    while(q->head != NULL){ // while queue is not empty
        q->head = q->head->next; // set head to next node
        free(temp); // free the previous head node
        temp = q->head; // set temp to new head
    }
    q->tail = NULL; // set tail to NULL
    
    free(q); // free the queue
    return NULL; // return NULL
}
