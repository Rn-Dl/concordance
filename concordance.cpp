#include <ctype.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <getopt.h>

struct conLetters : std::ctype<char>
{
    conLetters() : std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const *get_table()
    {
        static std::vector<std::ctype_base::mask> rc(std::ctype<char>::table_size,
                                                     std::ctype_base::space);

        std::fill(&rc['a'], &rc['z' + 1], std::ctype_base::alpha);
        rc['\''] = std::ctype_base::alpha;
        rc['-'] = std::ctype_base::alpha;
        return &rc[0];
    }
};

struct Data
{
    unsigned count;
    std::vector<unsigned> locs;
};

void printCounter(std::map<std::string, Data> &C);
void fprintCounter(std::map<std::string, Data> &C);

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);

    opterr = 0;
    int c;
    bool fout = true;

    while ((c = getopt(argc, argv, "o")) != -1)
        switch (c)
        {
        case 'o':
            fout = false;
            break;
        case '?':
            if (isprint(optopt))
                std::cerr << "Unknown option `-" << optopt << "'.\n";
            else
                std::cerr << "Unknown option character `\\x" << optopt << "'.\n";
            return 1;
        default:
            abort();
        }

    clock_t begin = clock();

    const char *fn = (optind < argc) ? argv[optind] : "input.txt";

    std::string line;
    std::string word;
    size_t pos = 0;
    std::unordered_map<std::string, Data> Counter;

    std::ifstream ifp(fn, std::ios::in);
    if (ifp.is_open())
    {
        unsigned curLoc = 0;
        while (getline(ifp, line))
        {
            ++curLoc;

            transform(line.begin(), line.end(), line.begin(), ::tolower);

            std::istringstream ss(line);
            ss.imbue(std::locale(std::locale(), new conLetters()));
            while (ss >> word)
            {
                if (word[0] == '\'' || word[0] == '-')
                    word.erase(0, 1);
                if ((pos = word.find("--")) != std::string::npos)
                {
                    if (pos != 0)
                    {
                        ++Counter[word.substr(0, pos)].count;
                        Counter[word.substr(0, pos)].locs.push_back(curLoc);
                    }
                    word = word.substr(pos + 2);
                }
                if (word[0] == '\'' || word[0] == '-')
                    word.erase(0, 1);
                if (word == "\0")
                    continue;
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

    std::map<std::string, Data> orderedCounter(Counter.begin(), Counter.end());

    if (fout)
        fprintCounter(orderedCounter);
    else
        printCounter(orderedCounter);

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << "\nExecution time:\t" << time_spent << "\n";

    return 0;
}

void printCounter(std::map<std::string, Data> &C)
{
    for (std::pair<std::string, Data> w : C)
    {
        std::cout << w.first << " ";
        for (int location : w.second.locs)
            std::cout << location << " ";
        std::cout << "[" << w.second.count << "]"
                  << "\n";
    }
}

void fprintCounter(std::map<std::string, Data> &C)
{
    std::ofstream ofp("output.txt", std::ios::out);
    if (ofp.is_open())
    {
        for (std::pair<std::string, Data> w : C)
        {
            ofp << w.first << " ";
            for (int location : w.second.locs)
                ofp << location << " ";
            ofp << "[" << w.second.count << "]"
                << "\n";
        }
        ofp.close();
    }
    else
    {
        std::cerr << "Unable to write file\n";
        exit(1);
    }
}