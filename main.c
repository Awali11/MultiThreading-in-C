#include "header.h"

/**
 * Write final balance to a single file.
 * The path name should be output/result.txt
 */
 
 

void writeBalanceToFiles(void) {
    // TODO: write balance for each customer 
    
    //lock
    pthread_mutex_lock(&b_lock);
    double total = 0;
    FILE *output = fopen("output/result.txt", "a+");
    
    // TODO: write total balance change
    

    for (int i = 0; i < acctsNum; i++){
    	fprintf(output,"%d\t%lf\n", i, balance[i]);	
    	total += balance[i];
   }
	

    
    fprintf(output, "All: \t%lf\n", total);
    fclose(output);
    pthread_mutex_unlock(&b_lock);
    //unlock


}
double balance[acctsNum];
int numConsumers;

 
int main(int argc, char *argv[]){
    
    //TODO: Argument check
    if( argc < 3){
    	printf("Not enought arguments");
    	exit(0);
    }
    
    numConsumers = atoi(argv[1]);
    
    char *inputFileName;
    inputFileName = argv[2];
    
    if (numConsumers < 1){
    	printf("Not enough consumers.");
    	exit(0);
    }
    
    logg = 0;
    
    if (argc >= 4){
    
    	if (strcmp(argv[3], "-p") == 0 )
    		logg = 1;
    }
	
	
    bookeepingCode();
    
    //TODO: Initialize global variables, like shared queue
        
    head = NULL;
    tail = NULL;
    
    //TODO: create producer and consumer threads
    
    pthread_t producer_thread;
    pthread_t consumer_thread[numConsumers];
    
    
    int arg_arr[numConsumers];
    
    for (int i = 0; i < numConsumers; i++) {
        arg_arr[i] = i;
    }

    if (logg == 1){
        logFile = fopen("output/log.txt", "a");
    }
    
    if(pthread_create(&producer_thread, NULL, producer,inputFileName) != 0){ 	
    	printf("Failed to create producer thread.");   
    }
   
    if (logg == 1){
    	fprintf(logFile, "producer\n");
        fflush(logFile);
    }
    
    for(int i = 0; i < numConsumers; i++){
    	if(pthread_create(&consumer_thread[i], NULL, consumer, (void*) & arg_arr[i]) != 0){ 	
    		printf("Failed to create consumer thread.");   
    	}	
    
	    if (logg == 1){
            fprintf(logFile, "consumer %d\n", arg_arr[i]);
            fflush(logFile);
	    }  
    } 

    //TODO: wait for all threads to complete execution
    
    pthread_join(producer_thread, NULL);
    
    for(int i = 0; i < numConsumers; i++) {
        pthread_join(consumer_thread[i], NULL);
    }


    //Write the final output
    writeBalanceToFiles();
    

    if (logg == 1){
        fclose(logFile);
    }



    return 0; 
}
