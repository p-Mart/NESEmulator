import fileinput

def hexEnding(line):
    partition = line.partition("//")
    try:
        int(partition[-1])
    except ValueError:
        return ''

    hex_end = "0x{:02x}".format(int(partition[-1]))
    return " (" + hex_end + ")"

for line in fileinput.input("calltable.cpp", inplace=True):
    print(line.rstrip() + hexEnding(line))
