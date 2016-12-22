#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

int main(int argc, char **argv)
{
    const char *fn = argc > 1 ? argv[1] : "input.txt";

    std::string word;
    std::unordered_map<std::string, int> dict;

    std::ifstream ifp(fn, std::ios::in);
    if (ifp.is_open())
    {
        while (ifp)
        {
            ifp >> word;
            std::cout << word << "\n";
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