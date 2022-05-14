#include "huffman.hpp"
#include "exceptions.hpp"

//HuffmanNode:

HuffmanNode:: HuffmanNode(byte_t ch):bt_(ch) {};

HuffmanNode:: HuffmanNode(HuffmanNode* lhs, HuffmanNode* rhs):bt_(0), left_(lhs), right_(rhs){};

byte_t HuffmanNode:: get_bt() {
    return bt_;
};

bool HuffmanNode:: is_last() {
    return (left_ or right_) ? false : true;
};

HuffmanNode* HuffmanNode:: get_left() {
    return left_;
};

HuffmanNode* HuffmanNode:: get_right() {
    return right_;
};


std::ostream& operator<<(std::ostream& os, const HuffmanNode& hn) {
    return os << hn.bt_ << std::endl;
};

//HuffmanTree:

codes_t HuffmanTree:: get_codes(HuffmanNode* root_) {
    codes_t res;
    if (root_->is_last()){
        res.insert(std::pair<byte_t,std:: string>(root_->get_bt(), "l"));
        return res;
    };
    std::stack<std::pair<HuffmanNode*, std::string>> nodes_s;
    nodes_s.push(make_pair(root_, (std::string)("")));
    while (!nodes_s.empty()) {
        HuffmanNode* top = nodes_s.top().first;
        std::string cur_str = nodes_s.top().second;
        nodes_s.pop();
        if (top->is_last())
            res.insert(std::pair<byte_t, std::string>(top->get_bt(), cur_str));
        else {
            nodes_s.push(make_pair(top->get_left(), cur_str + 'l'));
            nodes_s.push(make_pair(top->get_right(), cur_str + 'r'));
        }
    };
    return res;
};

HuffmanTree:: HuffmanTree(std::vector<byte_t> symbs, std::vector<int> freqs) {
    std:: vector <std::pair<int, HuffmanNode*>> vect;
    int size = symbs.size();
    for (int i = 0; i < size; ++i)
        try{
            vect.push_back(std::make_pair(freqs[i], new HuffmanNode(symbs[i])));
        }catch(...){
            std:: for_each(vect.begin(), vect.end(), [](std::pair<int, HuffmanNode*> it){
                delete it.second;
            });
            throw HuffmanException::AllocateProblem();
        };
    while (vect.size() != 1) {
        std::sort(vect.begin(), vect.end(), [](auto p1, auto p2) {
            return p2.first < p1.first;
        });
        std:: pair<int, HuffmanNode*> last1 = vect.back();
        vect.pop_back();
        std:: pair<int, HuffmanNode*> last2 = vect.back();
        vect.pop_back();
        try{    
            vect.push_back(std::make_pair(last1.first + last2.first, new HuffmanNode(last1.second, last2.second)));
        }catch(...){
            std:: for_each(vect.begin(), vect.end(), [](std::pair<int, HuffmanNode*> it){
                delete it.second;
            });
            throw HuffmanException:: AllocateProblem();
        };
    };
    root_ = vect.back().second;
    codes_table_ = get_codes(root_);
};

HuffmanTree:: ~HuffmanTree() {
    std::stack<HuffmanNode*> nodes_s;
    nodes_s.push(root_);
    while (!nodes_s.empty()) {
        HuffmanNode* top = nodes_s.top();
        nodes_s.pop();
        if (!top->is_last()) {
            nodes_s.push(top->get_left());
            nodes_s.push(top->get_right());
        }
        delete top;
    };
};

HuffmanNode* HuffmanTree::get_root(){
    return root_;
};

codes_t& HuffmanTree:: get_codes_table() {
    return codes_table_;
};


std::ostream& operator<<(std::ostream& os, codes_t codes) {
    for_each(codes.begin(), codes.end(), [&os](std::pair<char, std::string> p1) {
        os << "char:  " << p1.first << "  code:  " << p1.second << std::endl;
    });
    return os;
};

HuffmanTree:: HuffmanTree(HuffmanNode* root, std:: map<byte_t, std:: string> cd_table):root_(root), codes_table_(cd_table){};


// Archiver:: HuffmanCoder

int Archiver:: HuffmanCoder:: set_bits(byte_t& bt, std::string& str, int start_pos_b, int final_pos_b, int start_pos_s) {
    int cur_pos = start_pos_b;
    for (; cur_pos <= final_pos_b; ++cur_pos)
        if (str[start_pos_s++] == 'l')
            bt |= 1 << (7 - cur_pos);
        else
            bt &= 255 - (1 << (7 - cur_pos));
    return start_pos_s;
};

int Archiver:: HuffmanCoder:: output_code(std::ofstream& out, std::string& code, int start_pos_b, byte_t& byte, stat_t& my_stat) {
    int length = code.size();
    int cur_pos_s = set_bits(byte, code, start_pos_b, std::min(7, length - 1 + start_pos_b), 0);
    if (length - 1 + start_pos_b < 7) {
        return length + start_pos_b;
    };
    try{    
        out.write((char*)&byte, 1);
    }catch(...){
        throw HuffmanException:: WriteFileProblem();    
    };
    my_stat.out_size++;
    while (length - cur_pos_s >= 8) {
        cur_pos_s = set_bits(byte, code, 0, 7, cur_pos_s);
        try{
            out.write((char*)&byte, 1);
        }catch(...){
            throw HuffmanException:: WriteFileProblem();
        }
        my_stat.out_size++;
    };
    cur_pos_s = set_bits(byte, code, 0, length - 1 - cur_pos_s, cur_pos_s);
    return (length + start_pos_b) % 8;  
};

void Archiver:: HuffmanCoder:: output_table(std::ofstream& out, stat_t& my_stat){
    char our_delimiter = '.';
    std:: string num = std:: to_string(num_symbs);
    try{    
        out << num;
        my_stat.table_size += num.size();
        out.write(&our_delimiter, 1);
    }catch(...){
        throw HuffmanException:: WriteFileProblem();
    };
    my_stat.table_size++;
    for (auto it: codes_table_){    
        byte_t bt = it.first;
        int length = it.second.size();
        try{
            out.write((char*)&bt, 1);
            out.write(it.second.c_str(), length);
            out.write(&our_delimiter, 1);
        }catch(...){
            throw HuffmanException:: WriteFileProblem();   
        }
        my_stat.table_size += length + 2;
    };
    try{
        out.write(&our_delimiter, 1);
        out.write(&our_delimiter, 1);
    }catch(...){
        throw HuffmanException:: WriteFileProblem();
    }
    my_stat.table_size += 2;        
};



void Archiver:: HuffmanCoder:: encode(std::ifstream& in, std::ofstream& out) {
    stat_t my_stat;
    if (num_symbs == 0){
        std:: cout << my_stat;
        return;
    }
    byte_t in_ch;
    byte_t out_ch = 0;
    std::string code;
    output_table(out, my_stat);
    int start_pos_b = 0;
    while (in.read((char*)&in_ch, 1)){
        my_stat.in_size++;
        code = codes_table_[in_ch];
        start_pos_b = output_code(out, code, start_pos_b, out_ch, my_stat);
    };
    if (start_pos_b != 0) {
        try{    
            out.write((char*)&out_ch, 1);
        }catch(...){
            throw HuffmanException:: WriteFileProblem();
        }
        my_stat.out_size++;
    };
    std:: cout << my_stat;
    return;
};

Archiver:: HuffmanCoder:: HuffmanCoder(std::ifstream& in) {
    std::vector<int> alphabet(256);
    byte_t cur_ch;
    std::vector<byte_t> chars;
    std::vector<int> freqs;
    if (in.read((char*)&cur_ch,1)){
        alphabet[cur_ch]++;
        while (in.read((char*)&cur_ch, 1))
            alphabet[cur_ch]++;
        for (int i = 0; i < 256; ++i) {
            if (alphabet[i]) {
                chars.push_back(i);
                freqs.push_back(alphabet[i]);
                num_symbs += alphabet[i];
            };
        };
        try{    
            tree_ = new HuffmanTree(chars, freqs);
        }catch(...){
            throw HuffmanException::AllocateProblem(); 
        }
        codes_table_ = tree_->get_codes_table();
        in.clear();
        in.seekg(0);
    }else
        tree_ = nullptr;
};

Archiver:: HuffmanCoder:: ~HuffmanCoder() {
    if (tree_)
        delete tree_;
};

//Archiver:: HuffmanDecoder

codes_t Archiver:: HuffmanDecoder:: read_codes_table(std:: ifstream& in, stat_t& my_stat){
    std:: map<byte_t, std:: string> ans;
    in >> num_symbs_;
    if (num_symbs_ == 0)
        return ans;
    my_stat.table_size += std::to_string(num_symbs_).size();
    char byte, cur_c;
    std:: string cur_str;
    if (in.eof())
        throw HuffmanException::BadArchive();
    in.read(&cur_c, 1);
    if (cur_c != '.')
        throw HuffmanException:: BadArchive();
    my_stat.table_size++;
    while (in.read(&byte, 1)){    
        in.read(&cur_c,1);
        my_stat.table_size += 2;
        if (cur_c == '.')
            break;
        while(cur_c != '.'){
            cur_str += cur_c;
            in.read(&cur_c,1);
            my_stat.table_size++;
        };
        if (cur_str.empty())
            throw HuffmanException::BadArchive();
        ans.insert(std:: pair<byte_t, std:: string>(byte, cur_str));
        cur_str.clear();
    };
    return ans;  

};

void Archiver::HuffmanDecoder:: decode_byte(std::ofstream& out, byte_t& byte, HuffmanNode*& cur, int& counter) {
    int cur_pos = 0;
    while (cur_pos < 8) {
        cur = (byte & (1 << (7 - cur_pos++))) ? cur->get_left() : cur->get_right();
        if (cur->is_last()) {
            byte_t output_b = cur->get_bt();
            try{    
                out.write((char*)&(output_b), 1);
            }catch(...){
                throw HuffmanException::WriteFileProblem();
            }
            if (!--counter)
                break;
            cur = tree_->get_root();
        };
    };
};

void Archiver:: HuffmanDecoder:: decode(std::ifstream& in, std::ofstream& out, stat_t& my_stat){
    if (num_symbs_ == 0){
        if (!in.eof())
            throw HuffmanException::BadArchive();
        std:: cout<< my_stat;
        return;   
    }
    byte_t cur_b;
    HuffmanNode* cur_node = tree_->get_root();
    int counter = num_symbs_;
    my_stat.out_size = num_symbs_;
    while (in.read((char*)&cur_b, 1)){
        decode_byte(out, cur_b, cur_node, counter);
        my_stat.in_size++;
    };
    std:: cout << my_stat;
    return;
};

Archiver:: HuffmanDecoder:: HuffmanDecoder(std:: ifstream& in, stat_t& my_stat){
    codes_table_ =  read_codes_table(in, my_stat);
    tree_ = (codes_table_.empty())? nullptr :build_tree(codes_table_);
};

Archiver:: HuffmanDecoder:: ~HuffmanDecoder(){
    if (tree_)
        delete tree_;
};


HuffmanTree* Archiver::HuffmanDecoder:: build_tree(codes_t codes_table){
    if (codes_table.size() == 1){
        HuffmanNode* node, *node2;
        try{
            node = new HuffmanNode(codes_table.begin()->first);
        }catch(...){
            throw HuffmanException::AllocateProblem();
        };
        try{
            node2 = new HuffmanNode(codes_table.begin()->first);
        }catch(...){
            delete node;
            throw HuffmanException:: AllocateProblem();
        };
        try{
            return new HuffmanTree(new HuffmanNode(node, node2), codes_table);
        }catch(...){
            delete node;
            delete node2;
            throw HuffmanException:: AllocateProblem();
        };
    }
    std:: vector<std:: pair<HuffmanNode*,std:: string>> vect;
    for (auto it: codes_table)
        try{
            vect.push_back(make_pair(new HuffmanNode(it.first), it.second));
        }catch(...){
            std:: for_each(vect.begin(), vect.end(), [](std::pair<HuffmanNode*, std:: string> it){
                delete it.first;
            });
            throw HuffmanException:: AllocateProblem();
        };
    while (vect.size() != 1){
        std:: sort(vect.begin(), vect.end(), [](auto it1, auto it2){
            return it1.second > it2.second;
        });
        auto it = vect.begin();
        while (it < vect.end() - 1){
            auto next = it + 1; 
            if (it->second.substr(0, it->second.size() - 1) == next->second.substr(0, next->second.size() -1)){
                HuffmanNode* left = (it->second[it->second.size() - 1] == 'l')?it->first:next->first;
                HuffmanNode* right =(it->second[it->second.size() - 1] == 'r')?it->first:next->first;
                std:: string sub_str = it->second.substr(0, it->second.size() -1);
                vect.erase(it);
                vect.erase(it);
                try{
                    vect.push_back(make_pair(new HuffmanNode(left, right), sub_str));
                }catch(...){
                    std:: for_each(vect.begin(), vect.end(), [](std::pair<HuffmanNode*, std:: string> it){
                        delete it.first;
                    });
                    throw HuffmanException:: AllocateProblem();
                };
            }else{  
                it = next;
            }
        };
    };

    try{    
        return new HuffmanTree(vect.back().first, codes_table);
    }catch(...){
        delete vect.back().first;
        throw HuffmanException::AllocateProblem();
    }
    return nullptr;
};

//  Archiver:

void Archiver::encode(std:: ifstream& in, std:: ofstream&out){
    HuffmanCoder huf_cod(in);
    huf_cod.encode(in, out); 
};

void Archiver:: decode(std:: ifstream&in, std:: ofstream& out){
    stat_t my_stat;
    HuffmanDecoder huf_dec(in, my_stat);
    huf_dec.decode(in, out, my_stat);    
    return;
};

std::ostream& operator<<(std::ostream& os, Archiver::stat_t my_stat) {
    return os << my_stat.in_size << std::endl
              << my_stat.out_size << std::endl
              << my_stat.table_size << std::endl;
};