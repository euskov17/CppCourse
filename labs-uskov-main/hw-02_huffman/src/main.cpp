#include "huffman.hpp"
#include "exceptions.hpp"
#include <string>

class request{
    private:
        bool action_is_encode;
        std:: string input_path;
        std:: string output_path;
    public:
        request(int argc, char* argv[]){
            int cur_arg = 1;
            if (argc != 6)
                throw HuffmanException::ArgsProblem();
            while (cur_arg < argc){
                std:: string str(argv[cur_arg++]);
                if (str == "-c")
                    action_is_encode = true;
                else if (str == "-u")
                    action_is_encode = false;
                else if ((str == "-f" or str == "--file") and cur_arg <= argc)
                    input_path = argv[cur_arg++];
                else if ((str == "-o" or str == "output") and cur_arg <= argc)
                    output_path = argv[cur_arg++];
                else
                    throw HuffmanException:: ArgsProblem();
            };
        };
        void do_action(){
            std:: ifstream in(input_path);
            std:: ofstream out(output_path);
            if (!in.is_open())
                throw HuffmanException:: FileOpenFailed();
            if (!out.is_open())
                throw HuffmanException:: FileOpenFailed();
            Archiver huf_arch;
            (action_is_encode)? huf_arch.encode(in, out): huf_arch.decode(in, out);
        };
};  


int main(int argc, char* argv[]){
    request Action(argc, argv);
    Action.do_action();
    return 0;
};

