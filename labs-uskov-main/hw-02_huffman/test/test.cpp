#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "huffman.hpp"
#include "exceptions.hpp"

#define ENCODE_STR( in_file ) {(char*)"./test", (char*)"-c", (char*)"-f", (char*)in_file, (char*)"-o", (char*)"samples/tmp_file"};
#define DECODE_STR(out_file) {(char*)"./test", (char*)"-u", (char*)"-f", (char*)"samples/tmp_file", (char*)"-o", (char*)out_file};

class request{
    private:
        bool action_is_encode;
        std:: string input_path;
        std:: string output_path;
    public:
        friend bool cmp_files(std:: ifstream& in1, std:: ifstream& in2);
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

int argcnt = 6;

bool cmp_files(std:: string old_file, std:: string new_file){
    std:: ifstream in1(old_file);
    std:: ifstream in2(new_file);
    if (not in1.is_open() and not in2.is_open())
        return true;
    char ch1, ch2;
    while (not(in1.eof()) and (not in2.eof())){
        in1.read(&ch1,1);
        in2.read(&ch2,1);
        if (ch1 != ch2)
            return false;
    };
    if ((in1.eof() and in2.eof()))
        return true; 
    return false;
}

TEST_CASE("Huffman_Node test"){
    HuffmanNode a('a');
    HuffmanNode b('b');
    HuffmanNode c = HuffmanNode(&a, &b);
    CHECK(a.is_last());
    CHECK(a.get_bt() == 'a');
    CHECK(not c.is_last());
    CHECK(c.get_left() == &a);
    CHECK(c.get_right() == &b);
    CHECK(c.get_bt() == 0);
}

TEST_CASE("Huffman_Tree test one letter"){
    HuffmanTree one_char_tree(std::vector<byte_t>({'a'}), std::vector<int>({17}));
    CHECK(one_char_tree.get_codes_table().at('a') == std::string("l"));
    CHECK(one_char_tree.get_root()->is_last());
    CHECK(one_char_tree.get_codes_table().size() == 1);
}

TEST_CASE("Huffman_Tree test easy tree"){
    std:: vector<byte_t> chars = {'a', 'b', 'c', 'd', 'e', 'f'};
    std:: vector<int> freqs = {1,10,100,1000,10000,100000};
    HuffmanTree one_char_tree(chars, freqs);
    CHECK(one_char_tree.get_codes_table().at('f') == "r");
    CHECK(one_char_tree.get_codes_table().at('e') == std::string("lr"));
    CHECK(one_char_tree.get_codes_table().at('d') == std::string("llr"));
    CHECK(one_char_tree.get_codes_table().at('c') == std::string("lllr"));
    CHECK(one_char_tree.get_codes_table().at('b') == std::string("llllr"));
    CHECK(one_char_tree.get_codes_table().at('a') == std::string("lllll"));
    CHECK(one_char_tree.get_codes_table().size() == 6);
    CHECK(not one_char_tree.get_root()->is_last());   
}


TEST_CASE("Huffman_Tree test not monot"){
    std:: vector<byte_t> chars = {'d', 'f', 'e', 'a', 'c', 'b'};
    std:: vector<int> freqs = {1000,100000,10000,1,100,10};
    HuffmanTree one_char_tree(chars, freqs);
    CHECK(one_char_tree.get_codes_table().at('f') == "r");
    CHECK(one_char_tree.get_codes_table().at('e') == std::string("lr"));
    CHECK(one_char_tree.get_codes_table().at('d') == std::string("llr"));
    CHECK(one_char_tree.get_codes_table().at('c') == std::string("lllr"));
    CHECK(one_char_tree.get_codes_table().at('b') == std::string("llllr"));
    CHECK(one_char_tree.get_codes_table().at('a') == std::string("lllll"));
    CHECK(one_char_tree.get_codes_table().size() == 6);
    CHECK(not one_char_tree.get_root()->is_last());   
}   

TEST_CASE("Huffman_Tree test same chars"){
    std:: vector<byte_t> chars = {'a', 'b', 'c', 'd', 'e', 'f','g','h'};
    std:: vector<int> freqs = {100,100,100,100,100,100,100,100};
    HuffmanTree one_char_tree(chars, freqs);
    CHECK(one_char_tree.get_codes_table().at('f').length() == 3);
    CHECK(one_char_tree.get_codes_table().at('e').length() == 3);
    CHECK(one_char_tree.get_codes_table().at('d').length() == 3);
    CHECK(one_char_tree.get_codes_table().at('c').length() == 3); 
    CHECK(one_char_tree.get_codes_table().at('b').length() == 3);
    CHECK(one_char_tree.get_codes_table().at('a').length() == 3);
    CHECK(one_char_tree.get_codes_table().size() == 8); 
    CHECK(not one_char_tree.get_root()->is_last());   
}

TEST_CASE("Huffman_Tree test strange"){
    std:: vector<byte_t> chars = {'a', 'b', 'c', 'd', 'e', 'f'};
    std:: vector<int> freqs = {15,25,35,45,55,65};
    HuffmanTree one_char_tree(chars, freqs);
    CHECK(one_char_tree.get_codes_table().at('f') == "rl");
    CHECK(one_char_tree.get_codes_table().at('e') == std::string("lr"));
    CHECK(one_char_tree.get_codes_table().at('d') == std::string("ll"));
    CHECK(one_char_tree.get_codes_table().at('c') == std::string("rrl"));
    CHECK(one_char_tree.get_codes_table().at('b') == std::string("rrrr"));
    CHECK(one_char_tree.get_codes_table().at('a') == std::string("rrrl"));
    CHECK(one_char_tree.get_codes_table().size() == 6);
    CHECK(not one_char_tree.get_root()->is_last());   
}


/// ENCODE -- DEODE TESTS

TEST_CASE("new_test"){
    char* argv[] =  ENCODE_STR("samples/input/empty");
    request Action(argcnt, argv);
    Action.do_action();
    CHECK(cmp_files("samples/input/empty","samples/tmp_file"));
}

TEST_CASE("empty_test decode"){
    char* argv[] = DECODE_STR("samples/decode/empty_new");
    request Action(argcnt, argv);
    Action.do_action();
    CHECK(cmp_files("samples/input/empty", "samples/decode/empty_new"));
}

TEST_CASE("one_letter_test encode"){
    char* argv[] = ENCODE_STR("samples/input/one_letter.txt");
    request Action(argcnt, argv);
    Action.do_action();
}


TEST_CASE("one_letter_test decode"){
    char* argv[] = DECODE_STR("samples/decode/one_letter_new.txt");
    request Action(argcnt, argv);
    Action.do_action();
    CHECK(cmp_files("samples/input/one_letter.txt", "samples/decode/one_letter_new.txt"));
}

TEST_CASE("small_test encode"){
    char* argv[] = ENCODE_STR("samples/input/small.txt");
    request Action(argcnt, argv);
    Action.do_action();
}

TEST_CASE("small_test decode"){
    char* argv[] = DECODE_STR("samples/decode/small_new.txt");
    request Action(argcnt, argv);
    Action.do_action();
    CHECK(cmp_files("samples/input/small.txt", "samples/decode/small_new.txt"));
}

TEST_CASE("text_test encode"){
    char* argv[] = ENCODE_STR("samples/input/text.txt");
    request Action(argcnt, argv);
    Action.do_action();
}

TEST_CASE("text_test decode"){
    char* argv[] = DECODE_STR("samples/decode/text_new.txt");
    request Action(argcnt, argv);
    Action.do_action();
    CHECK(cmp_files("samples/input/text.txt", "samples/decode/text_new.txt"));
}


TEST_CASE("Lena test encode"){
    char* argv[] = ENCODE_STR("samples/input/lena.bmp");
    request Action(argcnt, argv);
    Action.do_action();
}

TEST_CASE("Lena test decode"){
    char* argv[] = DECODE_STR("samples/decode/lena_new.bmp");
    request Action(argcnt, argv);
    Action.do_action();
    CHECK(cmp_files("samples/input/lena.bmp", "samples/decode/lena_new.bmp"));
}

TEST_CASE("Big_screen test encode"){
    char* argv[] = ENCODE_STR("samples/input/big_screen.png");
    request Action(argcnt, argv);
    Action.do_action();
}

TEST_CASE("Big screen test decode"){
    char* argv[] = DECODE_STR("samples/decode/big_screen_new.png");
    request Action(argcnt, argv);
    Action.do_action();
    CHECK(cmp_files("samples/input/big_screen.png", "samples/decode/big_screen_new.png"));
}