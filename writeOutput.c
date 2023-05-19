#include "a2.h"

// Writes the contents of a c_queue* queue to a text file in composite Table format.
void write_txt(c_queue *q){

    FILE *t_file = fopen("compositeTable.txt", "w"); //opens file in write mode

    if(!t_file){ //checks if file was opened successfully
        perror("Error opening file");
        return;
    }

    int i=0; //index

    fprintf(t_file, "\t %s\t%s\t%s\t\t%s\n\t %s\n", "PID", "FD", "Filename", "Inode", "==============================================="); //write header

    for(c_table *tr = q->head; tr; tr=tr->next, i++){ //iterate through the queue
        fprintf(t_file, " %d\t%d\t%d\t%s\t%lu\n", i, tr->pid, tr->fd, tr->fname, tr->inode); //write each table entry (PID, FD, Filename, Inode) with its corresponding index
    }

    fprintf(t_file, "\t %s\n","==============================================="); //write footer
    fclose(t_file); //closes file
}

// Writes the contents of a c_queue* queue to a binary file in composite Table format.
void write_bin(c_queue *q){

    FILE *b_file = fopen("compositeTable.bin", "wb"); //opens file in write mode

    if(!b_file){ //checks if file was opened successfully
        perror("Error opening file");
        return;
    }
    
    int i=0; //index
    char header[] = "\t PID\tFD\tFilename\t\tInode\n\t ===============================================\n\0"; 
    fwrite(header, sizeof(char), strlen(header), b_file); //writes header to file
    

    for(c_table *tr = q->head; tr; tr=tr->next, i++){ //iterate through the queue
        fwrite(" ", sizeof(char), 1, b_file); //writes a space to file
        fwrite(&i, sizeof(int), 1, b_file); //writes index to file
        fwrite("\t", sizeof(char), 1, b_file); //writes a tab to file
        fwrite(&(tr->pid), sizeof(int), 1, b_file); //writes PID to file
        fwrite("\t", sizeof(char), 1, b_file); //writes a tab to file
        fwrite(&(tr->fd), sizeof(int), 1, b_file); //writes FD to file
        fwrite("\t", sizeof(char), 1, b_file); //writes a tab to file
        fwrite(tr->fname, sizeof(char), strlen(tr->fname), b_file); //writes Filename to file
        fwrite("\t", sizeof(char), 1, b_file); //writes a tab to file
        fwrite(&(tr->inode), sizeof(ino_t), 1, b_file); //writes Inode to file
        fwrite("\n", sizeof(char), 1, b_file); //writes a newline to file
    }

    char footer[] = "\t ===============================================\n\0"; 
    fwrite(footer, sizeof(char), strlen(footer), b_file); //writes footer to file

    fclose(b_file); //closes file
}