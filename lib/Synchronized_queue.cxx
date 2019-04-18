#include "Utils.hh"

template<class T>
T Synchronized_queue<T>::get(){
  std::unique_lock<std::mutex> lock(_mutex);
  _condition_variable(lock, [this](){_queue.size() > 0;});
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
