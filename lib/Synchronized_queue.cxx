#include "Utils.hh"
#include <iostream>

template<class T>
T Synchronized_queue<T>::get(){
  std::unique_lock<std::mutex> lock(_mutex);
  // D(std::cout << warning << "[Synchronized_queue::"
  //   << __func__<< "]. " << reset << "waiting..." << '\n';)
  _condition_variable.wait( lock, [this](){return _queue.size() > 0;} );
  // D(std::cout << warning << "[Synchronized_queue::"
  //             << __func__<< "]. " << reset << "message_arrived" << '\n';)
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
