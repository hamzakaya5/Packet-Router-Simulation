#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

	int port1_queue[10485][3]={0};				//initialized the data stack to the zero
	int port2_queue[10485][3]={0};
	int port3_queue[10485][3]={0};
	int port4_queue[10485][3]={0};

int enqueue(int packet[3], int port[][3])
{
		int i = 0;
		for(i; port[i][1] != 0; i++ );	//finding the last nonzero element in data
										//i is index of first zero element in port
		port[i][0] = packet[0];			
		port[i][1] = packet[1];			//incoming packet data is being replaced to the first zero index 
		port[i][2] = packet[2];
}


int drop(int a[][3],int packet[3], long int *dropped, long int *datasize){
	
	int i = 0;
	
	if(packet[2]==1)		//if incoming packet has high priority
	{
		for(i; a[i][0] != 0;i++);		//finding the last packet in the queue, 
		i--;							//i is going back to last nonzero element
		for(i; a[i][2] != 0 && i >=0 ; i--){	//finding the low priority packet beginning from the tail
		};
		
		if(i<0){			//this means there is no low priority packet in queue
			
			packet[0] = 0;	//incoming packet is being dropped
			packet[1] = 0;
			packet[2] = 0;
			(*dropped)++;
		}
		else{
			for(i; a[i][0] != 0;i++);	//ý am finding the last packet in the queue
			i--;						//index of last nonzero packet 
			(*datasize) -= a[i][1];		//length of dropped packet from queue is being subtracted from queue size
			
			a[i][0]=0;			//ý am setting the low priority packet in the port queue to the zero
			a[i][1]=0;
			a[i][2]=0;
		}
	}else if(packet[2]==0){		//incoming packet has low priority
	
		packet[0]=0;			//packet is being dropped
		packet[1]=0;
		packet[2]=0;
		(*dropped)++;
	}
		
		for(i; a[i+1][1] != 0; i++ ){	//when we deque a packet indexes of port queue  should be 
										//reduced 1 number.
			a[i][0] = a[i+1][0];
			a[i][1] = a[i+1][1];
			a[i][2] = a[i+1][2];
		}		
	
	
}


	
int dequeue(int a[][3], long long int *num_of_bytes, int *num_high_priority, int *num_low_priority, long int *size){
	
	*num_of_bytes += a[0][1];	//first,packet length of queue is added to num_of_bytes
								//because first element of queue is will be dequeue	
								
	(*size) -= a[0][1] ;			//when we dequeue the port, size of data in that port will be reduced
		
	(a[0][2] == 1)?((*num_high_priority) +=1):((*num_low_priority) +=1);	//counting the low-high priority packets
	
	a[0][0] = 0;		//first element is setted to zero
	a[0][1] = 0;
	a[0][2] = 0;
		
	int i = 0;
	for(i; a[i+1][1] != 0; i++ )
	{
		
		a[i][0] = a[i+1][0]; 	//index of every packet in the queue are going
		a[i][1] = a[i+1][1];	//1 step back
		a[i][2] = a[i+1][2];
		
	}
	
}




int main(){
	srand(time(NULL));		//for producing random number
	
	long int datasize_1 = 0;
	long int datasize_2 = 0;    //total data sizes of each port
	long int datasize_3 = 0;
	long int datasize_4 = 0; 
	long long int num_of_bytes = 0;	//bytes routed succesfully
	long int dropped = 0;			//number of dropped packets
	int num_high_priority = 0;	//number of high priority packets
	int num_low_priority = 0;	//number of low priority packets
	
	int count_enqueue = 0;
	int count_enqueue1 = 0, count_enqueue2 = 0, count_enqueue3 = 0, count_enqueue4 = 0;
	int dst_ip, len, priority;		//packet details..
	int packet[3];					//incoming packet
	int how_long; 		// how many milisecond code will run
	int ratio;
	int time1, time2;		//variables time1 and time2 are being used for print expected data	
	int sum =0;				//every 1 second.
	int finish;				//time to finish execution
	int start = clock();	//starting of execution 
	time1 = clock();			

	printf("\nHow many milisecond program should run?\n");
	scanf("%d",&how_long);	//we are taking the execution time of code.
	printf("What is percentage of congestion (it should be between 0 and 100)\n");
	scanf("%d",&ratio);	
	while(1)
	{	
		dst_ip = (rand()%4)+1;		//packet data are being produced
		len = rand()%1401+100;		
		priority = rand()%2;
		
		packet[0] = dst_ip;			// Data are being replaced to ip packet
		packet[1] = len;
		packet[2] = priority;
		
		sum += len;
	
		if(packet[0]==1){	
			datasize_1 += packet[1];	//length of produced packet is added to size of data in port1
						
			if(datasize_1>1024*1024){	//if datasize exceed 1mb=1024*1024 byte 
				datasize_1 -= packet[1];	//datasize is going back to its first value.
			
				drop(port1_queue,packet, &dropped, &datasize_1);	//calling drop function.
			}
			
			enqueue(packet, port1_queue); 
			count_enqueue1++;		//this is for counting the amount of the packet for this port
			count_enqueue++;		//this is for counting the total amount of enqueue
		
		}else if(packet[0]==2){
			datasize_2 += packet[1];		
					
			if(datasize_2>1024*1024){	
				datasize_2 -= packet[1];
			
				drop(port2_queue,packet, &dropped, &datasize_2);
			}
			enqueue(packet, port2_queue);
			count_enqueue2++;
			count_enqueue++;
				
		}else if(packet[0]==3){
			
			datasize_3 += packet[1];			
			
			if(datasize_3>1024*1024){
				datasize_3 -= packet[1];
			
				drop(port3_queue,packet, &dropped, &datasize_3);
			}
			enqueue(packet, port3_queue);
			count_enqueue3++;		
			count_enqueue++;		
			
		}else if(packet[0]==4){
			
			datasize_4 += packet[1];		
		
			
			if(datasize_4 > 1024*1024){
				datasize_4 -= packet[1];
			
				drop(port4_queue,packet, &dropped, &datasize_4);
			}
			enqueue(packet, port4_queue);	
			count_enqueue4++;		
			count_enqueue++;		
		}
		
		
		if(count_enqueue>=2500){	//ý started dequeue the ports after some amount of data is being enqueued.
									//ý used same congestion rate for all ports to dequeue.
			long int count_dequeue =0;
			for(count_dequeue = 0; 100*count_dequeue < count_enqueue1*(100-ratio)  ; count_dequeue++){	
				dequeue(port1_queue, &num_of_bytes, &num_high_priority, &num_low_priority, &datasize_1);
			}
			for(count_dequeue = 0; 100*count_dequeue < count_enqueue2*(100-ratio)  ; count_dequeue++){	
				dequeue(port2_queue, &num_of_bytes, &num_high_priority, &num_low_priority, &datasize_2);
			}			
			for(count_dequeue = 0; 100*count_dequeue < count_enqueue3*(100-ratio)  ; count_dequeue++){	
				dequeue(port3_queue, &num_of_bytes, &num_high_priority, &num_low_priority, &datasize_3);
			}			
			for(count_dequeue = 0; 100*count_dequeue < count_enqueue4*(100-ratio)  ; count_dequeue++){	
				dequeue(port4_queue, &num_of_bytes, &num_high_priority, &num_low_priority, &datasize_4);
			}							
			count_enqueue =0; count_enqueue1 =0;	 count_enqueue2 =0; count_enqueue3 =0;  count_enqueue4 =0;	
			
		}
		
		time2 = clock();
		
		if( time2-time1 == 1000 ){
			printf("%d\n",count_enqueue);
			printf("\nsize of queue in port1 = %ld", datasize_1); //total data size of
			printf("\nsize of queue in port2 = %ld", datasize_2); //each queue
			printf("\nsize of queue in port3 = %ld", datasize_3);
			printf("\nsize of queue in port4 = %ld", datasize_4);
			printf("\nNumber of dropped packet --> %ld",dropped);		//number of dropped packet
			printf("\nHigh priority packets routed succesfully  --> %d",num_high_priority);
			printf("\nNumber of low priority packet --> %d",num_low_priority);
			printf("\nNumber of bytes routed successfully --> %lld\n",num_of_bytes);
			
			dropped = 0;
			num_high_priority = 0; 	//in every 1 second, printed data should be refresh.
			num_low_priority = 0; 	//size of queue didnt set to 0, because there may be still data inside queue
			num_of_bytes=0; 
			count_enqueue =0;
			count_enqueue1=0; count_enqueue2=0; count_enqueue3=0; count_enqueue4=0;
			
			time1 = clock();	//we will recalculate the time difference between time2 and time1
		}	
		
		finish = clock();
		if((finish-start) >= how_long){
			printf("\nexecution time --> %d\n", finish-start);
			break;
		}
	}
}
