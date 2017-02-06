#include <ctype.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

struct conLetters : std::ctype<char>
{
    conLetters() : std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const *get_table()
    {
        static std::vector<std::ctype_base::mask> rc(std::ctype<char>::table_size,
                                                     std::ctype_base::space);

        std::fill(&rc['A'], &rc['z' + 1], std::ctype_base::alpha);
        return &rc[0];
    }
};

struct Data
{
    unsigned count;
    std::vector<unsigned> locs;
};

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);

    const char *fn = argc > 1 ? argv[1] : "input.txt";

    std::string line;
    std::string word;
    std::unordered_map<std::string, Data> Counter;

    std::ifstream ifp(fn, std::ios::in);
    ifp.imbue(std::locale(std::locale(), new conLetters()));
    if (ifp.is_open())
    {
        unsigned curLoc = 0;
        while (getline(ifp, line))
        {
            ++curLoc;
            std::istringstream ss(line);
            while (ss >> word)
            {
                ++Counter[word].count;
                Counter[word].locs.push_back(curLoc);
            }
        }
        ifp.close();
    }
    else
    {
        std::cerr << "Unable to open file\n";
        exit(1);
    }

    return 0;
}