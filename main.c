#include "a2.h"

int main(int argc, char **argv){
    
    //flags to determine which options are passed to the program
    bool per_process = false, systemWide = false, v_nodes = false, composite = false, output_txt = false, output_binary = false;
    int threshold=-1, cmd, arg_pid=-1;

    //uses getopt_long to parse the command line options passed to the program
    struct option long_options[] = { //an array of 'struct option' objects, each line representing a single command line option
        {"per-process", no_argument, 0, 'p'}, //takes "per-process" with no argument, returns 'p' if option is present
        {"systemWide", no_argument, 0, 's'}, //takes "systemWide" with no argument, returns 's' if option is present
        {"Vnodes", no_argument, 0, 'v'}, //takes "Vnodes" with no argument, returns 'v' if option is present        
        {"composite", no_argument, 0, 'c'}, //takes "composite" with no argument, returns 'c' if option is present
        {"output_TXT", no_argument, 0, 'o'}, //takes "output_TXT" with no argument, returns 'o' if option is present
        {"output_binary", no_argument, 0, 'b'}, //takes "outout_binary" with no argument, returns 'b' if option is present
        {"threshold", required_argument, 0, 't'}, //takes "threshold" with required argument, returns 't' if option is present
        {0,0,0,0} //indicates the end of options
    };

    while ((cmd=getopt_long(argc, argv, "psvcobt:", long_options, NULL)) != -1){ 
    //the string "sugqn::t::" specifies that he options -s, -u, -g, -q, -n and -t are available. 
    //The (::) following the letters n and t indicate that an optional argument, which the user can specify by appending a value to the option on the command line
    
        switch (cmd) { //switch statment to determine action to take based on the option returned by getopt_long
            case 'p':
                per_process = true; //in case cmd is 'p', 'per_process' is set to true
                break;
            case 's':
                systemWide = true; //in case cmd is 's', 'systemWide' is set to true
                break;
            case 'v':
                v_nodes = true; //in case cmd is 'v', 'v_nodes' is set to 1
                break;
            case 'c':
                composite = true; //in case cmd is 'c', 'composite' is set to 1
                break;
            case 'o':
                output_txt = true; //in case cmd is 'o', 'output_txt' is set to 1
                break;
            case 'b':
                output_binary = true; //in case cmd is 'b', 'output_binary' is set to 1
                break;
            case 't':
                //in case cmd is 't', atoi converts the required argument from string to integer and updates the value of threshold 
                threshold = atoi(optarg);
                break;
            default:
                composite = true;
        }
    }

   //retrieve the positional argument for a specific pid
    if(argc > optind) arg_pid = atoi(argv[optind]);


    c_queue *q = setUp();
    c_queue *threshold_q = setUp();

    if(q==NULL || threshold_q==NULL){
        fprintf(stderr, "Calloc failed\n");
        return EXIT_FAILURE;
    }

    handleProcesses(q, arg_pid, threshold, threshold_q);
   
    printTable(q, arg_pid, argc, per_process, systemWide, v_nodes, composite);

    if(output_txt) write_txt(q);
    if(output_binary) write_bin(q);

    if(threshold >=0)
        printThreshold(threshold_q);

    q = deleteAll(q);
    threshold_q = deleteAll(threshold_q);

    return 0;

}