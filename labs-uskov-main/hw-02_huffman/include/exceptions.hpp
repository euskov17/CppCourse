#pragma once

#include <exception>

namespace HuffmanException{
    class MyException: public std:: exception{};

    class FileOpenFailed : public MyException{
        public:
            const char* what() const noexcept override{
                return "File open failed";
            };
    };

    class ArgsProblem: public MyException{
        public:
            const char* what() const noexcept override{
                return "Bad argument list";
            };
    };

    class AllocateProblem: public MyException{
        public:
            const char* what() const noexcept override{
                return "Memory Allocation problem";
            };
    };

    class BadArchive: public MyException{
        public:
            const char* what() const noexcept override{
                return "Archive not decodable";
            };  
    };
    
    class WriteFileProblem: public MyException{
        public:
            const char* what() const noexcept override{
                return "Write file problem";
            };
    };
};