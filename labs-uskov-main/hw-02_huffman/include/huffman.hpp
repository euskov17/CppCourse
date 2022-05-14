#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <fstream>

using byte_t = uint8_t;
using codes_t = std::map<byte_t, std::string>;
class Archiver;

class HuffmanNode {
   private:
    byte_t bt_;
    HuffmanNode* left_ = nullptr;
    HuffmanNode* right_ = nullptr;
   public:
    friend std::ostream& operator<<(std::ostream& os, const HuffmanNode& hn);
    HuffmanNode(byte_t ch); 
    HuffmanNode(HuffmanNode* lhs, HuffmanNode* rhs);
    byte_t get_bt();
    bool is_last();
    HuffmanNode* get_left();
    HuffmanNode* get_right();
};

std::ostream& operator<<(std::ostream& os, const HuffmanNode& hn);

class HuffmanTree {
   private:
    HuffmanNode* root_;
    codes_t codes_table_;
    codes_t get_codes(HuffmanNode* root_);
   public:
    friend class Archiver;
    HuffmanTree(HuffmanNode* root, std::map<byte_t, std:: string> cd_table);
    HuffmanTree(std::vector<byte_t> symbs, std::vector<int> freqs);
    ~HuffmanTree();
    codes_t& get_codes_table();
    HuffmanNode* get_root();
};

std::ostream& operator<<(std::ostream& os, codes_t codes);




class Archiver{
    struct stat_t {
        int in_size = 0;
        int out_size = 0;
        int table_size = 0;
    };
    struct HuffmanCoder{    
        void encode(std::ifstream& in, std::ofstream& out);
        HuffmanCoder(std::ifstream& in);
        ~HuffmanCoder();
        HuffmanTree* tree_;
        codes_t codes_table_;
        int num_symbs = 0;
        int set_bits(byte_t& bt, std::string& str, int start_pos_b, int final_pos_b, int start_pos_s);
        int output_code(std::ofstream& out, std::string& code, int start_pos_b, byte_t& byte, stat_t& my_stat);
        void output_table(std::ofstream& out, stat_t& my_stat);
    };
    struct HuffmanDecoder{
        HuffmanDecoder(std:: ifstream& in, stat_t& my_stat);
        void decode(std::ifstream& in, std::ofstream& out, stat_t& my_stat);
        ~HuffmanDecoder();
        HuffmanTree* tree_;
        codes_t codes_table_;
        int num_symbs_ = 0;
        codes_t read_codes_table(std:: ifstream& in, stat_t& my_stat);
        HuffmanTree* build_tree(codes_t codes_table);
        void decode_byte(std::ofstream& out, byte_t& byte, HuffmanNode*& cur,int& counter);
    };    
    public:
        friend std::ostream& operator<<(std::ostream& os, Archiver::stat_t my_stat);
        void encode(std:: ifstream& in, std:: ofstream& out);
        void decode(std:: ifstream&in, std:: ofstream&out);  
};

std::ostream& operator<<(std::ostream& os, Archiver::stat_t my_stat);