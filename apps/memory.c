//#include <stdlib.h>
#include <mem.h>
#include <paging.h>



node * first_node=NULL;

unsigned int check_deadbeef(node * n1){
	int * casted_addr=(int *)n1->data_addr;
	int * base=casted_addr-8;
	int * front = base+4;
	int * back = base+8+((n1->size)/sizeof(int));
	int i =0;
	int beef=0;
	for(i=0;i<4;i++){
		if(*front==buffer && *back==buffer){
			beef+=1;	
		}
		front+=1;
		back+=1;	
	}
	if(beef==4){return 1;}
	return 0;
}
void check_data(void * addr){


}
node * create_node(node * n1,int bytes){
		//base of the original 
		int * orig_base = (int *)n1;
		//end of the original nodes data
		int * eod=orig_base+8+(n1->size/sizeof(int));
		//the start of the new node
		int * new_base= eod-((bytes+32)/sizeof(int));
		//start of the new nodes data
		int * new_data_addr=new_base+8;
		//start of the new nodes begining dead beef
		int * start=new_base+4;
		int i;
	
		//add begining deadbeef
		for(i=0;i<4;i++){
			*start=buffer;
			start+=1;
	
		}
		//void * check = (void *) new_node;
		//change the stuff in the old node to reflect the addition of the new node
//		printf("N1 SIZE %d\n",n1->size);
		n1->size=n1->size-(bytes+48);
		int * new_end = orig_base+8+n1->size/sizeof(int);
		for(i=0;i<4;i++){
			*new_end=buffer;
			new_end+=1;	
		}	
		node * new_node = (node *) new_base;
		new_node->size=bytes;
		new_node->data_addr=(void * )new_data_addr;
//		printf("FIRST NODE %d\n",first_node->size);
		node * cur_node = first_node;
		node * first_holder=first_node;
		if(n1->size>bytes){
			while(cur_node->next!=NULL){
				if(cur_node->size<new_node->size){
						new_node->next=cur_node->next;
						new_node->prev=cur_node;
						cur_node->next=new_node;
						new_node->next->prev=new_node;
						break;
					}
					cur_node=cur_node->next;
				}
				if(cur_node->next==NULL){
					first_holder->prev=new_node;
					new_node->next=first_holder;
					first_node=new_node;	
				}

		}else if(n1->size<bytes){
			//if theres a null in next or prev
			if(n1->next==NULL || n1->prev==NULL){
				printf("NULLS\n");
				//remove the old node from the list
				if(n1->next!=NULL){
				printf("n1->next %d\n",n1->next->prev->size);
				n1->next->prev=n1->prev;
				}
				if(n1->prev!=NULL){
			//	printf("n1->prev %d\n",n1->prev->next->size);
				n1->prev->next=n1->next;
				}
				//place the new node in the list in the proper place
				cur_node=first_holder;
				while(cur_node->next!=NULL){
					if(cur_node->size > new_node-> size){
						cur_node->prev->next=new_node;
						cur_node->prev=new_node;	
						new_node->next=cur_node;
						break;	
					}
					cur_node=cur_node->next;
				}
				//if its the biggest thing in the list just put it at the end
				if(cur_node->next==NULL){
					new_node->prev=cur_node;
					cur_node->next=new_node;
				}
				cur_node=first_holder;
				while(cur_node->next!=NULL){
					if(cur_node->size > n1->size){
						if(cur_node->prev==NULL){
							n1->next=cur_node;
							n1->prev=NULL;
							cur_node->prev=n1;
							first_node=n1;
							break;	
						}else{
							cur_node->prev->next=n1;
							n1->prev=cur_node->prev;
							cur_node->prev=n1;
							n1->next=cur_node;
							
							break;
						}			
					}
					cur_node=cur_node->next;
				}
				//if its the last node put the old node in front of it

				if(cur_node->next==NULL){
					cur_node->prev->next=n1;
					n1->next=cur_node;
					cur_node->prev=n1;

			}
		//otherwise if there are no nulls
		}else{
		//	printf("NO NULLS\n");
			n1->prev->next=n1->next;
			n1->next->prev=n1->prev;
			cur_node=first_holder;			
			while(cur_node->next!=NULL){
				if(cur_node->size > new_node->size){
					cur_node->next->prev=new_node;
					cur_node->prev->next=new_node;
					new_node->next=cur_node->next;
					new_node->prev=cur_node->prev;
					break;	

				}
				cur_node=cur_node->next;
			}
			if(cur_node->next==NULL){
				new_node->prev=cur_node->prev;
				cur_node->prev->next=new_node;
				new_node->next=cur_node;
				cur_node->prev=new_node;
				
			}
			cur_node=first_holder;
			
			while(cur_node->next!=NULL){
				
				if(cur_node->size > n1->size){
				//	printf("N1 LOOP N1<BYTES\n");
					if(cur_node->prev==NULL){
						n1->prev=cur_node->prev;
						cur_node->prev=n1;
						n1->next=cur_node;
						first_node=n1;
						break;	
					}else
				
					n1->prev=cur_node->prev;
					n1->next=cur_node;
					if(n1->prev!=NULL){
					n1->prev->next=n1;
					}
					cur_node->prev=n1;	
					break;
					
				}
				cur_node=cur_node->next;
			}	
			
		}
	}
		
	return new_node;	
}
void * m_malloc(unsigned int bytes){
	
	//things for when i actually get setting up the first node working lol	
	node * cur_node;
	cur_node=first_node;	
	//if the first node doesnt exist allocate a new page for it and writes its junk into mem (initialize list)	
	node * new_node=NULL;
	if(bytes<4096){				
		if(first_node==NULL){
			kprintf("dicks");
			node * base =(node *)get_mem(4096);
			
			//node that first_node will point to
			int i;
			//allocate a page for the first node
			//Set the previous and next in memory as well as in the node to NULL
			base->prev=base->next=NULL;
			//set the size
			base->size=4048;
			//cast the memory back to void * so we can store the addr in the proper format
			void * temp=(void *) base;
			base->data_addr=temp+32;
			// sets the front and back of the area where we want to insert the buffer		
			int * base_intaddr= (int *)base;		
			//cast the front and back addresses
			int * front = base_intaddr+4;
			int * back = base_intaddr+8+base->size/sizeof(int);
			//move through memory 4 bytes at a time assigning buffer each 4 bytes
			for(i=0;i<4;i++){
				*front=buffer;
				*back=buffer;
				front+=1;
				back+=1;
			}
			cur_node=base;
			first_node=cur_node;
			new_node=create_node(cur_node,bytes);
			first_node=new_node;
			cur_node=first_node;
		/**
			do{	
				printf("\n\n");
				printf("Node %p\n",cur_node);
				printf("Prev %p\n",cur_node->prev);
				printf("Next %p\n",cur_node->next);
				printf("Size %d\n",cur_node->size);
				printf("Data %p\n",cur_node->data_addr);
				
			cur_node=cur_node->next;
	
			}while(cur_node!=NULL);
		**/
			return new_node->data_addr;	
		}else{
				
			do{
				if(cur_node->size>bytes){
					if(cur_node->size-bytes>=128){
						new_node=create_node(cur_node,bytes);
						break;
					}	
				}
				cur_node=cur_node->next;	
			}while(cur_node!=NULL);
			printf("\n\n");
				
			//printf("Prev size %d\n",cur_node->prev->size);
			/**
			printf("Node %p\n",cur_node);
			printf("Prev %p\n",cur_node->prev);
			printf("Next %p\n",cur_node->next);
			printf("Size %d\n",cur_node->size);
			printf("Data %p\n",cur_node->data_addr);
			**/	
			cur_node=first_node;	
		/**
			do{	
				printf("\n\n");
				printf("Node %p\n",cur_node);
				printf("Prev %p\n",cur_node->prev);
				printf("Next %p\n",cur_node->next);
				printf("Size %d\n",cur_node->size);
				printf("Data %p\n",cur_node->data_addr);
				
			cur_node=cur_node->next;
	
				
			}while(cur_node!=NULL);
		**/
		if(new_node!=NULL){
		return new_node->data_addr;
		}
		}
	  } else if(bytes>=4096){
		//figure out how many pages we will need
		int pages=(bytes/4096);
		int i;
		int over=bytes-(pages*4096);
		node * first_page;
		//not sure how to make them contigious without a PAT/MMU so I am just making memory 		
		node * new_node=NULL;	
		if(first_node==NULL){
			node * base = (node*)get_mem(bytes + ((pages) *48));
			i=0;
			//node that first_node will point to
			//allocate a page for the first node
			// sets the front and back of the area where we want to insert the buffer		
			int * base_intaddr= (int *)base;
			base->size=bytes;
			void * temp=(void *) base;
			base->data_addr=temp+32;
			//cast the front and back addresses
			int * front = base_intaddr+4;
			int * back = base_intaddr+8+base->size/sizeof(int);
			//move through memory 4 bytes at a time assigning buffer each 4 bytes
			for(i=0;i<4;i++){
				*front=buffer;
				*back=buffer;
				front+=1;
				}
				
			first_node=base;	
		
			i=0;
			
			if(pages>1){
				
				node * first_holder=first_node;
				while(i!=pages){
					create_node(first_holder,4048);	
					i++;
				}
			if(first_node->size==0){
				first_node=first_node->next;
				first_node->next->prev=NULL;
				

				}
				
			}
			
		/**	
			cur_node=first_node;	
			do{	
			printf("\n\n");
			printf("Node %p\n",cur_node);
			printf("Prev %p\n",cur_node->prev);
			printf("Next %p\n",cur_node->next);
			printf("Size %d\n",cur_node->size);
			printf("Data %p\n",cur_node->data_addr);		
			cur_node=cur_node->next;	
			}while(cur_node!=NULL);
		**/
			}else{
			node * base = (node*)get_mem(bytes + ((pages) *48));
			i=0;
			//node that first_node will point to
			//allocate a page for the first node
			// sets the front and back of the area where we want to insert the buffer		
			int * base_intaddr= (int *)base;
			base->size=bytes;
			void * temp=(void *) base;
			base->data_addr=temp+32;
			//cast the front and back addresses
			int * front = base_intaddr+4;
			int * back = base_intaddr+8+base->size/sizeof(int);
			//move through memory 4 bytes at a time assigning buffer each 4 bytes
			for(i=0;i<4;i++){
				*front=buffer;
				*back=buffer;
				front+=1;
				}
			cur_node=first_node;
			while(cur_node->next!=NULL){
				if(cur_node->size>=bytes){
					if(cur_node->prev!=NULL && cur_node->next!=NULL){
						cur_node->prev->next=base;
						base->prev=cur_node->prev->next;
						cur_node->prev=base;
						base->next=cur_node;	
					}else if(cur_node->prev==NULL){
						cur_node->prev=base;
						base->next=cur_node;

					}
				}
				cur_node=cur_node->next;	
			}
			cur_node->next=base;
			base->prev=cur_node;
			cur_node=first_node;
			i=0;
			if(pages>1){
				bytes=bytes-over;	
				node * first_holder=base;
				do{		
					do{
					if(cur_node->size>bytes){
						if(cur_node->size-bytes>=128){
							new_node=create_node(cur_node,bytes);
							break;
							}	
						}
					cur_node=cur_node->next;	
					}while(cur_node!=NULL);
					cur_node=first_node;
					bytes=bytes-4096;
					i++;
				}while(i!=pages);
				cur_node=first_node;
				if(over>0){
					do{
						if(cur_node->size>over){
							if(cur_node->size-over>=128){
								new_node=create_node(cur_node,over);
								break;
							}
						}
						
						cur_node=cur_node->next;
				}while(cur_node!=NULL);	
			}				
		}	
			if(first_node->size==0){
				first_node=first_node->next;
				first_node->next->prev=NULL;
				

				}
			cur_node=first_node;
			/**
			while(cur_node->next!=NULL){
			printf("\n\n");
			printf("Node %p\n",cur_node);
			printf("Prev %p\n",cur_node->prev);
			printf("Next %p\n",cur_node->next);
			printf("Size %d\n",cur_node->size);
			printf("Data %p\n",cur_node->data_addr);		
			cur_node=cur_node->next;
				
			}
			**/
			return cur_node->data_addr;

		}			
	}
}
unsigned int is_contigious(node * n1, node * n2){

		
		int * int_n1=(int *)n1;
		int * int_n2=(int*)n2;
		int * data;
		if(n1->next!=NULL){
			
			data=int_n1-4-(n2->size/sizeof(int))-8;
		
			if((node *)data==n2){
				return 1;	
			}

		}else if(n1->next==NULL){
			data=int_n1+8+(n1->size/sizeof(int))+4;
			if((node *)data==n2){
				return 1;	
			}
		

		}
		


		return 0;

}
node * merge(node * n1, node * n2){
	node * cur_node=NULL;
	if(n1->prev!=NULL && n1->next!=NULL){
		n1->prev->next=n1->next;
		n1->next->prev=n1->prev;
	}else if(n1->prev==NULL){
		n1->next->prev=NULL;
		first_node=n1->next;	

	}else if(n1->next==NULL){
		n1->prev->next=NULL;
	}
	if(n2->prev!=NULL && n2->next!=NULL){
		n2->prev->next=n2->next;
		n2->next->prev=n2->prev;
	}else if(n2->prev==NULL){
		n2->next->prev=NULL;
		first_node=n2->next;	

	}else if(n2->next==NULL){
		n2->prev->next=NULL;
	}

	cur_node=first_node;	
	/**	
	do{
		printf("\n\n");
		printf("Node %p\n",cur_node);
		printf("Prev %p\n",cur_node->prev);
		printf("Next %p\n",cur_node->next);
		printf("Size %d\n",cur_node->size);
		printf("Data %p\n",cur_node->data_addr);		
		cur_node=cur_node->next;	
	}while(cur_node!=NULL);
	**/
	cur_node=first_node;
	printf("n1 size %d\n",n1->size);
	printf("n2 size %d\n",n2->size);
	if(n1->size < n2->size){
		n2->size=n1->size+n2->size+48;
		do{
			if(cur_node->size>n2->size){
				cur_node->prev->next=n2;	
				cur_node->prev=n2;
				n2->next=cur_node;			
	
	
			}	
			cur_node=cur_node->next;

		}while(cur_node->next!=NULL);
		if(cur_node->next==NULL){
			cur_node->prev->next=n2;
			n2->prev=cur_node->prev->next;
			cur_node->prev=n2;
			n2->next=cur_node;
		
		}
		
	}else if(n1->size>n2->size) {
		n1->size=n1->size+n2->size+48;
		do{
			if(cur_node->size>n1->size){
				cur_node->prev->next=n1;	
				cur_node->prev=n1;
				n1->next=cur_node;			
	
	
			}	
			cur_node=cur_node->next;

		}while(cur_node->next!=NULL);
		if(cur_node->next==NULL){
			cur_node->prev->next=n1;
			n1->prev=cur_node->prev->next;
			cur_node->prev=n1;
			n1->next=cur_node;
		
		}

	}

}
void m_free(void * addr){
	node * cur_node;
	node * chosen;
	if(first_node!=NULL){
		cur_node=first_node;
		do{
			if(cur_node->data_addr==addr){
				chosen=cur_node;
				break;
			}
			cur_node=cur_node->next;
		}while(cur_node!=NULL);
		cur_node=first_node;	
		do{
		if(is_contigious(chosen,cur_node)){
			//printf("got here");
			//printf("chosen %d\n",chosen->size);
			//printf("cur_node %d\n",cur_node->size);		
			merge(chosen,cur_node);
			break;
		}
		cur_node=cur_node->next;		

		}while(cur_node!=NULL);
	
	}
		
	
			cur_node=first_node;	
		/**	
			do{
			printf("\n\n");
			printf("Node %p\n",cur_node);
			printf("Prev %p\n",cur_node->prev);
			printf("Next %p\n",cur_node->next);
			printf("Size %d\n",cur_node->size);
			printf("Data %p\n",cur_node->data_addr);		
			cur_node=cur_node->next;	
			}while(cur_node!=NULL);
	**/
	
}

	
