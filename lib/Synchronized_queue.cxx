#include "Utils.hh"
#include <iostream>

template<class T>
T Synchronized_queue<T>::get(){
  std::unique_lock<std::mutex> lock(_mutex);
  std::cout<<"waiting...\n";
  _condition_variable.wait( lock, [this](){return _queue.size() > 0;} );
  std::cout<<__func__<<": message_arrived "<<std::endl;
  T result = _queue.front();
  _queue.pop();
  return result;
}

template<class T>
void Synchronized_queue<T>::push(const T &data){
  std::unique_lock<std::mutex> lock(_mutex);
  _queue.push(data);
  _condition_variable.notify_one();
}
