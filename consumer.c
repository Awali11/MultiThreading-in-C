#include "consumer.h"
#include <ctype.h>
#include <string.h>

/**
 * parse lines from the queue, calculate balance change
 * and update to global array
 */
 
void parse(char *line){
    
    // TODO: get customer id
    double total = 0;
    char *key;
    int customerID;
    
    char *data = line;
    
    key = strtok_r(data, ",", &data);
    customerID = atoi(key);
    
    // TODO: sum up transactions
    key = strtok_r(data, ",", &data);
    while (key != NULL) {
	double sum = strtod(key, NULL);
	total += sum;
	key = strtok_r(data, ",", &data);
    }

    // TODO: update the global array
    pthread_mutex_lock(&b_lock);
    balance[customerID] += total;
    pthread_mutex_unlock(&b_lock);



}


// consumer function
void *consumer(void *arg){
    
    //TODO: keep reading from queue and process the data
    // feel free to change
    

    int *consumerID = (int*) arg;

    while(1){
    	//lock
	pthread_mutex_lock(&q_lock);
    while(head == NULL){
 		pthread_cond_wait(&cond, &q_lock);
 	}

    	
    	linkedList_t *temp;
    	packet_t *info;
    	
    	temp = head;
    	head = head->next;
    	info = temp->packet;

		if (logg == 1){
			fprintf(logFile, "consumer %d: line %d\n", *consumerID, info->lineNum);
			fflush(logFile);

		}


    	if(info->lineNum == -1){
    		free(temp->packet);
    		free(temp);
    		pthread_mutex_unlock(&q_lock);
    		break;
    	}
    	else{

        	parse(info->fileLine);
        	free(temp->packet);
        	free(temp);
        	
        
        }
        //unlock 
	pthread_mutex_unlock(&q_lock);
    }
    

	
    return NULL; 
}

