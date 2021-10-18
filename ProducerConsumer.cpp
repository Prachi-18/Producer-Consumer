#include <ProducerConsumer.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

// TODO: add BoundedBuffer, locks and any global variables here
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

BoundedBuffer *buffer;
ofstream myfile;
int count;
int psleep2;
int csleep2;
int items2;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items) {

  myfile.open("output.txt");
  count = 0;
  items2 = items;
  psleep2 = psleep;
  csleep2 = csleep;
  
  buffer = new BoundedBuffer(100);
  

  pthread_t prod_arr[p];
  pthread_t con_arr[c];
  
  // int cthreadID = 0;
  
  for(long i = 0; i < p ; i++){
    
    pthread_create(&prod_arr[i], NULL, producer, (void*)i);
    }
  
  for(int i = 0; i < p ; i++){
    
    pthread_join(prod_arr[i], NULL);
    }
  
  for(long i = 0; i < c ; i++){
  //   pthread_t thread;
     pthread_create(&con_arr[i], NULL, consumer, (void*)i);
  //   cthreadID++;
  //   void* val;
  //  pthread_join(thread, &val );
    }
    
  for(int i = 0; i < c ; i++){
    
    pthread_join(con_arr[i], NULL);
    }
  
  myfile.close();
 
}



void* producer(void* threadID) {
  // TODO: producer thread, see instruction for implementation
    
    
     while(true){
       
       
       //pthread_mutex_lock(&m);
       
       std::this_thread::sleep_for (std::chrono::seconds(psleep2));
      
    int num = rand();

    if(count >= items2){
      cout<< "if condition" << endl;
      
      pthread_exit(NULL);
    }
       
    (*buffer).append(num);
    
    pthread_mutex_lock(&m);
    count++;

    auto curr_time = std::chrono::system_clock::now();
    time_t curr = std::chrono::system_clock::to_time_t(curr_time);
    myfile << "Producer " << threadID << ", time = " << curr << ", producing data item " << count << ", item value=" << num << endl;
    pthread_mutex_unlock(&m);
   }
  
  
}

void* consumer(void* threadID) {
  // TODO: consumer thread, see instruction for implementation
  // int count = 0;

      while(true){
        
        std::this_thread::sleep_for (std::chrono::seconds(csleep2));
       if((*buffer).isEmpty()){
        
        pthread_exit(NULL);
       }
    pthread_mutex_lock(&m);
    int removed_item = (*buffer).remove();
    auto curr_time = std::chrono::system_clock::now();
    time_t curr = std::chrono::system_clock::to_time_t(curr_time);
    myfile << "Consumer " << threadID << ", time = "<< curr << ", consuming data item with value=" << removed_item << endl;
    pthread_mutex_unlock(&m);
  }
  
}