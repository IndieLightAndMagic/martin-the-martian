#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <vector>
#include <map>

// A signal object may call multiple slots with the
// same signature. You can connect functions to the signal
// which will be called when the emit() method on the
// signal object is invoked. Any argument passed to emit()
// will be passed to the given functions.
namespace GTech{

    static int signalId{0};
    template <typename... Args>
    class Signal {

    public:

        Signal() : current_id_(0) {}

        // copy creates new signal
        Signal(Signal const& other) = default;

        // connects a member function to this Signal
        template <typename T>
        int connect_member(T *inst, void (T::*func)(Args...)) {
          return connect([=](Args... args) {
              (inst->*func)(args...);
          });
        }

        // connects a const member function to this Signal
        template <typename T>
        int connect_member(T *inst, void (T::*func)(Args...) const) {
          return connect([=](Args... args) {
              (inst->*func)(args...);
          });
        }

        // connects a void func(p0, p1, ... pn) to the signal
        int connect_function(void (*func)(Args...)){
            return connect([=](Args... args){
                (*func)(args...);
            });
        }

        // connects a std::function to the signal. The returned
        // value can be used to disconnect the function again
        int connect(std::function<void(Args...)> const& slot) const {
          slots_.insert(std::make_pair(++current_id_, slot));
          return current_id_;
        }

        // connects another signal
        int connect_signal(Signal const& other) {

            return connect([=](){
                other.emit();
            });

        }


        // disconnects a previously connected function
        void disconnect(int id) const {
          slots_.erase(id);
        }

        // disconnects all previously connected functions
        void disconnect_all() const {
          slots_.clear();
        }

        // calls all connected functions
        void emit(Args... p) const{
          for(auto id_slot : slots_) {
            id_slot.second(p...);
          }
        }

        // assignment creates new Signal
        Signal& operator=(Signal const& other) {
          disconnect_all();
        }



    private:
        std::map<int, std::function<void(Args...)>>& GetIdSlots() const{
            return slots_;
        };
        mutable std::map<int, std::function<void(Args...)>> slots_;
        mutable int current_id_;
    };



}

#endif /* SIGNAL_HPP */