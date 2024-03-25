#ifndef MESSAGE_BUS_MESSAGE_BUS_H_
#define MESSAGE_BUS_MESSAGE_BUS_H_

#include <any>
#include <functional>
#include <map>
#include <string>
#include <typeinfo>

namespace excercise {

class MessageBus {
 using TopicType = std::string;
 using KeyType = std::string;
 public:
  MessageBus() = default;
  ~MessageBus() = default;
  
  template<typename F>
  void Register(F&& func, TopicType topic = "") {
    KeyType key = GenerateKey(std::forward<F>(func), topic);
    map_.insert({key, func});
  }
  
  template<typename F>
  uint32_t Unregister(F&& func, TopicType topic = "") {
    KeyType key = GenerateKey(std::forward<F>(func), topic);
    
    uint32_t count = 0;
    auto it = map_.find(key);
    while (it != map_.end() && it->first == key) {
      it = map_.erase(it);
      count ++;
    }
    return count;
  }
  
  template<typename RetType>
  void SendMsg(TopicType topic = "") {
    KeyType key = GenerateKey(std::function<RetType()>(), topic);
    auto range = map_.equal_range(key);
    for (auto it = range.first; it != range.second; it ++) {
      auto func = std::any_cast<std::function<RetType()>>(it->second);
      func();
    }
  }

  template<typename RetType, typename... Args>
  void SendMsg(Args... args, TopicType topic = "") {
    KeyType key = GenerateKey(std::function<RetType(Args...)>(), topic);
    auto range = map_.equal_range(key);
    for (auto it = range.first; it != range.second; it ++) {
      auto func = std::any_cast<std::function<RetType(Args...)>>(it->second);
      func(std::forward<Args>(args)...);
    }
  }

 private:
  MessageBus(const MessageBus&) = delete;
  MessageBus& operator= (const MessageBus&) = delete;
  
  template<typename F>
  KeyType GenerateKey(F&& f, TopicType topic = "") {
    auto func = static_cast<std::function<F>>(std::forward<F>(f));
    KeyType key = topic + typeid(func).name();
    return key;
  }

  std::multimap<KeyType, std::any> map_;
};

} // namespace excercise

#endif // MESSAGE_BUS_MESSAGE_BUS_H_