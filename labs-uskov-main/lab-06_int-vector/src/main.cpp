#include "my_vector.hpp"

#include <string>
#include <string_view>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <algorithm>

struct Cmd {
    static constexpr size_t MAX_ARGS = 2;
    std::string name;
    size_t arg_nm;
    int args[MAX_ARGS];

    Cmd() {
        name.reserve(20);
        arg_nm = 0;
        for (int & arg : args)
            arg = 0;
    }

    void parse(std::string_view s) {
        arg_nm = 0;

        std::size_t pos = s.find(' ');
        name = std::string(s.substr(0, pos));
        if (pos == std::string_view::npos)
            return;
        s.remove_prefix(pos + 1);

        while (arg_nm < MAX_ARGS && !s.empty()) {
            pos = s.find(' ');
            args[arg_nm++] = static_cast<int>(std::strtol(s.data(), nullptr, 0));
            if (pos == std::string_view::npos)
                return;
            s.remove_prefix(pos + 1);
        }
    }
};

int main() {
    char buff[30];
    MyVector v;
    Cmd cmd;

#define CMD_ARGS_CHECK(CMD_NAME, ARGS_NM)                 \
    if (cmd.arg_nm != ARGS_NM) {                          \
        printf(#CMD_NAME" must take "#ARGS_NM" args.\n"); \
        continue;                                         \
    }

    while (true) {
        printf("> ");
        fflush(stdout);
        assert(fgets(buff, sizeof(buff), stdin));
        std::string_view buff_view = buff;
        if (!buff_view.empty() && buff_view.back() == '\n')
            buff_view.remove_suffix(1);
        cmd.parse(buff_view);

        if (cmd.name == "info") {
            CMD_ARGS_CHECK(Info, 0);
            printf("SZ: %zu; CAP: %zu;\n", v.size(), v.capacity());
            for (size_t i = 0; i < v.size(); ++i) {
                printf("%d ", v.get(i));
            }
            printf("\n");
        } else if (cmd.name == "set") {
            CMD_ARGS_CHECK(Set, 2);
            v.set(cmd.args[0], cmd.args[1]);
        } else if (cmd.name == "reserve") {
            CMD_ARGS_CHECK(Reserve, 1);
            v.reserve(cmd.args[0]);
        } else if (cmd.name == "resize") {
            CMD_ARGS_CHECK(Resize, 1);
            v.resize(cmd.args[0]);
        } else if (cmd.name == "push_back") {
            CMD_ARGS_CHECK(Push_back, 1);
            v.push_back(cmd.args[0]);
        } else if (cmd.name == "insert") {
            CMD_ARGS_CHECK(Insert, 2);
            v.insert(cmd.args[0], cmd.args[1]);
        } else if (cmd.name == "erase") {
            CMD_ARGS_CHECK(Erase, 1);
            v.erase(cmd.args[0]);
        } else if (cmd.name == "exit") {
            CMD_ARGS_CHECK(Exit, 0);
            break;
        } else {
            printf("Unknown command\n");
        }
    }

#undef CMD_ARGS_CHECK

}
