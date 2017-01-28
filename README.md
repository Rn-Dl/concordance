# Concordance

Write a C program that takes as input (via a command line argument) a sufficiently large text document (several are available online for testing; e.g. via Project Gutenberg), and produces as output (via stdout) an alphabetically-ordered listing of each unique word in the document (case insensitive and space separated, though be careful to consider hyphenated words), along with the lines from the input document that the word appears on. Each unique word (and the list of lines that it appears on) should be on a separate line in the output.

For example, taking the following text as input, in a file named "input.txt" in the current directory:

```This is
some kind OF text it
Is an example of text```

Supplied as an argument to an executable named "concordance" in the current directory:

```$ ./concordance ./input.txt```

The following would be the output:
```
an 3
example 3
is 1 3
it 2
kind 2
of 2 3
some 2
text 2 3
this 1
```

The program must:
  - Be written in standard C, and able to be compiled on Linux or MacOS. A Makefile should be supplied to enable easy compilation by the reviewer.
  - Not depend on any libraries excepting C standard libraries.
  - Utilize your own data structure code for all core data structures.
  - Output only the required output and exit upon completion.
  - Be well structured, cleanly implemented and well-commented.
  - Use a time (and space, but time is more important than space) efficient algorithm.
  - Be able to handle arbitrarily large files.

## Usage

Use `make all` to build tool in C.
And `make clean` to clean.