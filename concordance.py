#!/usr/bin/python3
import getopt
import re
import string
import time
from os.path import isfile
from sys import argv


def usage():
    print("concordance -i <input file> -o <output file>\n")


def main():
    try:
        opts, args = getopt.getopt(
            argv[1:], "i:o:h", ["input", "output", "help"])
    except getopt.GetoptError as err:
        print(err)
        usage()
        exit(2)
    fout = None
    fin = None
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            exit()
        elif o in ("-o", "--output"):
            fout = a
        elif o in ("-i", "--input"):
            fin = a
        else:
            assert False, "unhandled option"

    if not fin:
        if isfile("input.txt"):
            fin = "input.txt"
        else:
            print("Unable to open input file.")
            usage()
            exit(1)

    f = open(fin, 'r')
    text = f.read().lower()
    f.close()

    Counter = {}
    pattern = re.compile(r'\b([a-z\']+(?:-[a-z\']+)*)\b')

    for i, line in enumerate(text.split('\n')):
        match_pattern = re.findall(pattern, line)
        for word in match_pattern:
            if word not in Counter:
                Counter[word] = []
            Counter[word].append(i + 1)

    if fout:
        f = open(fout, 'w')
        for key in sorted(Counter):
            f.write(key + " ")
            for loc in Counter[key]:
                f.write(str(loc) + " ")
            f.write("[" + str(len(Counter[key])) + "]\n")
        f.close()
    else:
        for key in sorted(Counter):
            print(key, end=" ")
            for loc in Counter[key]:
                print(loc, end=" ")
            print("[" + str(len(Counter[key])) + "]")


if __name__ == "__main__":
    start_time = time.time()
    main()
    print("\n%s seconds" % (time.time() - start_time))
