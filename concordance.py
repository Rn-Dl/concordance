#!/usr/bin/python3
from sys import argv
import re
import string
import time


def main():
    fout = True
    file = None
    Counter = {}

    if(len(argv) > 1):
        for a in argv[1:]:
            if a == "-o":
                fout = False
            else:
                file = open(a, 'r')

    text = file.read().lower()
    file.close()

    pattern = re.compile(r'\b([a-z\']+(?:-[a-z\']+)*)\b')

    for i, line in enumerate(text.split('\n')):
        match_pattern = re.findall(pattern, line)
        for word in match_pattern:
            if word not in Counter:
                Counter[word] = []
            Counter[word].append(i + 1)

    if fout:
        of = open("output.txt", 'w')
        for key in sorted(Counter):
            of.write(key + " ")
            for loc in Counter[key]:
                of.write(str(loc) + " ")
            of.write("[" + str(len(Counter[key])) + "]\n")
        of.close()
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
