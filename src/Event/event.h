#ifndef __EVENT_H__
#define __EVENT_H__

#include <cxxabi.h>
#include <string>

namespace GTech {
    class Tech2DEvent {
    public:
        virtual std::string GetType() {
            auto name = typeid(*this).name();
            auto status = 4;
            std::unique_ptr<char, void (*)(void *)> res{
                    abi::__cxa_demangle(name, nullptr, nullptr, &status),
                    std::free
            };
            return (status == 0 ? res.get() : name);
        }
        static const std::string m_type;
        virtual ~Tech2DEvent(){}
    };
}


#endif /* __EVENT_H__ */