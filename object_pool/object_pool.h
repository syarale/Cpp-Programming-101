#ifndef OBJECT_POOL_OBJECT_POOL_H_
#define OBJECT_POOL_OBJECT_POOL_H_

#include <cassert>
#include <functional>
#include <memory>
#include <utility>

namespace excercise {

template<typename T>
class ObjectPool : public std::enable_shared_from_this<ObjectPool<T>> {
 public:
  using Func = std::function<void(T*)>;

  template<typename... Args>
  explicit ObjectPool(uint32_t num, Args&&... args);

  template<typename... Args>
  explicit ObjectPool(uint32_t num, Func func, Args&&... args);

  ~ObjectPool();

  std::shared_ptr<T> GetObject();
  uint32_t GetFreeNums() { return free_nums_; }

 private:
  ObjectPool(const ObjectPool&) = delete;
  ObjectPool& operator=(const ObjectPool&) = delete;
  void ReleaseObj(T* obj_ptr);

  struct Node {
    T element;
    Node* next;
  };
  
  char* buffer_area_ = nullptr;
  Node* free_head_ = nullptr;
  uint32_t object_nums_ = 0;
  uint32_t free_nums_ = 0;
}; 


template<typename T>
template<typename... Args>
ObjectPool<T>::ObjectPool(uint32_t num, Args&&... args) : object_nums_(num), free_nums_(num) {
  assert(num != 0);
  buffer_area_ = static_cast<char*> (std::calloc(object_nums_, sizeof(Node)));
  if (!buffer_area_) {
    throw(std::bad_alloc());
  }

  for (uint32_t i = 0; i < object_nums_; i ++) {
    T* obj_ptr = new (buffer_area_ + i * sizeof(Node)) T(std::forward<Args>(args)...);
    reinterpret_cast<Node*>(obj_ptr)->next = free_head_;
    free_head_ = reinterpret_cast<Node*>(obj_ptr);
  }
}

template<typename T>
template<typename... Args>
ObjectPool<T>::ObjectPool(uint32_t num, Func func, Args&&... args) : object_nums_(num) {
  assert(num != 0);
  buffer_area_ = static_cast<char*> (std::calloc(object_nums_, sizeof(Node)));
  if (buffer_area_ == nullptr) {
    throw(std::bad_alloc());
  }

  for (uint32_t i = 0; i < object_nums_; i ++) {
    T* obj_ptr = new (buffer_area_ + i * sizeof(Node)) T(std::forward<Args>(args)...);
    func(obj_ptr);
    reinterpret_cast<Node*>(obj_ptr)->next = free_head_;
    free_head_ = reinterpret_cast<Node*>(obj_ptr);
  }
}

template<typename T>
ObjectPool<T>::~ObjectPool() {
  for (uint32_t i = 0; i < object_nums_; i ++) {
    T* obj_ptr = reinterpret_cast<T*>(buffer_area_ + i * sizeof(Node));
    obj_ptr->~T();
  }
  delete buffer_area_;
}

template<typename T>
std::shared_ptr<T> ObjectPool<T>::GetObject() {
  if (free_head_ == nullptr) {
    return nullptr;
  }

  std::shared_ptr<T> obj_ptr(&(free_head_->element), 
                             std::bind(&ObjectPool<T>::ReleaseObj, this, std::placeholders::_1));
  free_head_ = free_head_->next;
  free_nums_ --;

  assert(obj_ptr != nullptr);
  return obj_ptr;
}

template<typename T>
void ObjectPool<T>::ReleaseObj(T* obj_ptr) {
  assert(obj_ptr != nullptr);
  reinterpret_cast<Node*>(obj_ptr)->next = free_head_;
  free_head_ = reinterpret_cast<Node*>(obj_ptr);
  free_nums_ ++;
  return;
}

} // namespace excercise

#endif //OBJECT_POOL_OBJECT_POOL_H_