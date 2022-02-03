#include "producer.h"

/**
 *
 * Producer thread will read from the file and write data to 
 * the end of the shared queue
 */
     	 


void *producer(void *arg){


	char *inputFileName = (char*) arg;
	FILE *inputFile;
	
	
	if(( inputFile = getFilePointer(inputFileName)) == NULL){
		printf("Failed to get file pinter.");
		exit(0); 
    	}
    	
    	
    	
    	int lineNumber = 0;
    	char *line = (char*) malloc(sizeof(char) * chunkSize);
    	ssize_t max = chunkSize;
    	
    	

    
    	while(getLineFromFile(inputFile, line, max) != -1){
    		pthread_mutex_lock(&q_lock);	
    		if (head == NULL || tail == NULL ){ //create first node
    			linkedList_t *newNode = (linkedList_t*) malloc (sizeof(linkedList_t));
    			packet_t *packet = (packet_t*) malloc (sizeof(packet_t)); 
    			char *dup = (char*) malloc(sizeof(char) * chunkSize);
				strncpy(dup,line, max);
    			packet->fileLine = dup;
    			packet->lineNum = lineNumber;
    			newNode->packet = packet;
    			newNode->next = NULL;
    			head = newNode;
    			tail = newNode;
    			lineNumber++;
    	
    	}
    		else{// add Node to end of list
    			linkedList_t *newNode = (linkedList_t*) malloc (sizeof(linkedList_t));
    			packet_t *packet = (packet_t*) malloc (sizeof(packet_t));
				char *dup = (char*) malloc(sizeof(char) * chunkSize);
				strncpy(dup,line,max);
    			packet->fileLine = dup;
    			packet->lineNum = lineNumber;
    			newNode->packet = packet;
    			newNode->next = NULL;
    			tail -> next = newNode;
    			tail = tail -> next;
    			lineNumber++;	
    			
    	}
    		if(logg == 1){
				fprintf(logFile,  "producer: line %d\n" , lineNumber);
				fflush(logFile);

			}
			
			pthread_cond_signal(&cond);
    	    pthread_mutex_unlock(&q_lock);
    	    	
    	}

    	
    //TODO: open the file and read its content line by line
    //Send data to the shared queue
    //When reaching the end of the file, send EOF message
	lineNumber = -1;
    for(int i = 0; i< numConsumers; i++){  
    	pthread_mutex_lock(&q_lock);	
   		if (head == NULL || tail == NULL ){ //create first node
			linkedList_t *newNode = (linkedList_t*) malloc (sizeof(linkedList_t));
    		packet_t *packetEOF = (packet_t*) malloc (sizeof(packet_t));
    		packetEOF->lineNum = lineNumber;
    		newNode->packet = packetEOF;	
    		newNode->next = NULL;
    		head = newNode;
    		tail = newNode;
    	
    	}
    	else{
    		linkedList_t *newNode = (linkedList_t*) malloc (sizeof(linkedList_t));
    		packet_t *packetEOF = (packet_t*) malloc (sizeof(packet_t));
    		packetEOF->lineNum = lineNumber;
    		newNode->packet = packetEOF;	
    		newNode->next = NULL;
    		tail->next = newNode;
    		tail = tail -> next;
    	}
    	
		if(logg == 1){
			fprintf(logFile,  "producer: line %d\n" , lineNumber);
			fflush(logFile);
		}
    	pthread_cond_signal(&cond);
    	pthread_mutex_unlock(&q_lock);
    	
    }
   
    // cleanup and exit
    fclose(inputFile);
    free(line);

    return NULL; 
}
