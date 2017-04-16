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
#include <sys/stat.h>

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

/* output of result to screen */
void printCounter(std::map<std::string, std::vector<unsigned>> &C);

/* output of result to file */
void fprintCounter(std::map<std::string, std::vector<unsigned>> &C, const char *fout);

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);

    opterr = 0;
    int c;
    const char *fout = NULL;

    while ((c = getopt(argc, argv, "o:h")) != -1)
        switch (c)
        {
        case 'o':
            fout = optarg;
            break;
        case 'h':
            std::cout << "concordance <input file> -o <output file>\n";
            exit(0);
            break;
        case '?':
            if (optopt == 'o')
                std::cerr << "Option -" << char(optopt) << " requires an argument.\n";
            else if (isprint(optopt))
                std::cerr << "Unknown option `-" << optopt << "'.\n";
            else
                std::cerr << "Unknown option character `\\x" << optopt << "'.\n";
            exit(1);
        default:
            abort();
        }

    clock_t begin = clock();

    const char *fn = (optind < argc) ? argv[optind] : "input.txt";
    struct stat buf;
    if (stat(fn, &buf) != 0)
    {
        std::cerr << "Unable to open file: " << fn << "\n";
        exit(1);
    }
    std::ifstream ifp(fn, std::ios::in);

    std::string line;
    std::string word;
    std::unordered_map<std::string, std::vector<unsigned>> Counter;

    unsigned curLoc = 0;
    while (getline(ifp, line))
    {
        ++curLoc;

        transform(line.begin(), line.end(), line.begin(), ::tolower);

        size_t pos = 0;
        while ((pos = line.find("--", pos)) != std::string::npos)
        {
            line.replace(pos, 2, " ");
            pos += 1;
        }

        std::istringstream ss(line);
        ss.imbue(std::locale(std::locale(), new conLetters()));
        while (ss >> word)
        {
            if (std::ispunct(word[0]))
                word.erase(0, 1);
            if (word.size() == 0)
                continue;
            if (std::ispunct(word[word.size() - 1]))
                word.erase(word.size() - 1, 1);

            Counter[word].push_back(curLoc);
        }
    }
    ifp.close();

    std::map<std::string, std::vector<unsigned>> orderedCounter(Counter.begin(), Counter.end());

    if (fout)
        fprintCounter(orderedCounter, fout);
    else
        printCounter(orderedCounter);

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << "\nExecution time:\t" << time_spent << "\n";

    return 0;
}

void printCounter(std::map<std::string, std::vector<unsigned>> &C)
{
    for (std::pair<std::string, std::vector<unsigned>> w : C)
    {
        std::cout << w.first << " ";
        for (unsigned location : w.second)
            std::cout << location << " ";
        std::cout << "[" << w.second.size() << "]"
                  << "\n";
    }
}

void fprintCounter(std::map<std::string, std::vector<unsigned>> &C, const char *fout)
{
    std::ofstream ofp(fout, std::ios::out);
    if (ofp.is_open())
    {
        for (std::pair<std::string, std::vector<unsigned>> w : C)
        {
            ofp << w.first << " ";
            for (unsigned location : w.second)
                ofp << location << " ";
            ofp << "[" << w.second.size() << "]"
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