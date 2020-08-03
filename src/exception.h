#ifndef __ETHER_EXCEPTION_H__
#define __ETHER_EXCEPTION_H__

namespace ether {

    class Exception : public std::exception {
    private:
        int m_errcode;
        std::string m_message;

    public:
        Exception();
        Exception(int code, const char* msg)  : m_errcode(code), m_message(msg) {}
        Exception(int code, const std::string& msg) : m_errcode(code), m_message(msg) {}
        Exception(const Exception& other)     : std::exception(other), m_errcode(other.m_errcode), m_message(other.what()) {}

        operator const char* () const { return what(); }
        operator int() const { return m_errcode; }

        //const char* what() const noexcept { return m_message.c_str(); }
    };

    class SystemError : public Exception {
    public:
        SystemError(const std::string& msg) : Exception(0, msg) {}
    };

    class MathError : public Exception {
        public:
            MathError(const std::string& msg) : Exception(0, msg) {}
    };

    class ConversionError : public Exception {
    public:
        ConversionError(const std::string& msg) : Exception(0, msg) {}
    };

    class EngineError : public Exception {
    public:
        EngineError(const std::string& msg) : Exception(0, msg) {}
    };

    class UserCancel : public Exception {
    public:
        UserCancel(int code, const std::string& msg) : Exception(code, msg) {}
    };

    class NotFound : public Exception {
    public:
        NotFound(const std::string& objName);
    };

    class AlreadyExists : public Exception {
    public:
        AlreadyExists(const std::string& objName);
    };

}

#endif // __ETHER_EXCEPTION_H__
