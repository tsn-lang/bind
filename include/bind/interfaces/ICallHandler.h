#pragma once

namespace bind {
    class Function;

    class ICallHandler {
        public:
            virtual ~ICallHandler();
            
            /**
             * @brief Calls a function
             * 
             * @param target Pointer to the `Function` to call
             * @param retDest Pointer to memory that will receive the return value, should
             * be `max(return type size, 8)` bytes. If the target does not return a value
             * then this parameter is ignored.
             * @param args Array of pointers to argument values. First element should be
             * 'this' pointer if this is a call to a type method
             */
            virtual void call(Function* target, void* retDest, void** args) = 0;
    };

    class HostCallHandler : public ICallHandler {
        public:
            virtual void call(Function* target, void* retDest, void** args);
            static HostCallHandler* get();
        
        private:
            static HostCallHandler instance;
    };

    class HostThisCallHandler : public ICallHandler {
        public:
            virtual void call(Function* target, void* retDest, void** args);
            static HostThisCallHandler* get();
        
        private:
            static HostThisCallHandler instance;
    };
};