#include <BoundedBuffer.h>

BoundedBuffer::BoundedBuffer(int N) {
  // TODO: constructor to initiliaze all the varibales declared in
  buffer = new int[N];
  buffer_size = N;
  buffer_cnt = 0;
  buffer_first = -1;
  buffer_last = -1;
  
  

  buffer_lock = PTHREAD_MUTEX_INITIALIZER;      // lock
  buffer_not_full = PTHREAD_COND_INITIALIZER;   // Condition indicating buffer is not full
  buffer_not_empty = PTHREAD_COND_INITIALIZER;  // Condition indicating buffer is not empty

}



BoundedBuffer::~BoundedBuffer() {
  // TODO: destructor to clean up anything necessary
  //  for( int i = 0; i < buffer_size; i++){
  //   buffer[i] = 0;
  // }
  // buffer_cnt = 0;
  // buffer_first = 0;
  // buffer_last = 0;
  pthread_mutex_destroy(&buffer_lock);
  pthread_cond_destroy(&buffer_not_full);
  pthread_cond_destroy(&buffer_not_empty);
}

void BoundedBuffer::append(int data) {
  // TODO: append a data item to the circular buffer
  pthread_mutex_lock(&buffer_lock);
  while(buffer_cnt == buffer_size){
    pthread_cond_wait(&buffer_not_full, &buffer_lock);
  }
  if(buffer_last == (buffer_size -1)){
    buffer_last = 0;
    buffer[buffer_last] = data;
  }
  else if(buffer_first == -1){
    buffer_first = buffer_last = 0;
    buffer[buffer_last] = data;
  }
  else {
    buffer_last++;
    buffer[buffer_last] = data;
  }

  buffer_cnt++;
  pthread_cond_signal(&buffer_not_empty);
  pthread_mutex_unlock(&buffer_lock);
}

int BoundedBuffer::remove() {
  // TODO: remove and return a data item from the circular buffer
    pthread_mutex_lock(&buffer_lock);
    while(buffer_cnt == 0){
    pthread_cond_wait(&buffer_not_empty, &buffer_lock);
  }
    int data = buffer[buffer_first];
    buffer_first++;
    
    if (buffer_first == buffer_size)
        buffer_first = 0;
    
 
  
  buffer_cnt--;
  pthread_cond_signal(&buffer_not_full);
  pthread_mutex_unlock(&buffer_lock);
  return data;
}

bool BoundedBuffer::isEmpty() {
  // TODO: check is the buffer is empty
  if(buffer_cnt == 0){
    return true;
  }
  else{
    return false;
  }
  
}