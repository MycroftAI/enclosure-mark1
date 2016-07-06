#!/usr/bin/env python

import glob
import sys

import os.path

if len(sys.argv) < 2 or not os.path.isfile(sys.argv[1]):
    for i in glob.glob("*.dat"):
        print(i)
    file_name = raw_input("Enter a file name: ")
else:
    file_name = sys.argv[1]

MAT_SX, MAT_SY = 32, 8
mat = [[0 for x in range(MAT_SY)] for y in range(MAT_SX)]

x, y = 0, 0

with open(file_name, 'r') as f:
    while True:
        c = f.read(1)
        if not c or x == MAT_SX:
            break
        if c == '\n':
            y = 0
            x += 1
            continue
        f.read(1)  # Ignore space
        if c == '0':
            mat[x][y] = 0
        elif c == '1':
            mat[x][y] = 1
        y += 1

INDENT = ' ' * 4
name_no_ext = os.path.splitext(file_name)[0]
file_name = name_no_ext + '.cpp'
with open(file_name, 'w') as f:
    f.write('const PROGMEM char IMAGE[][16] = {\n')
    for begin_x in range(0, MAT_SX, 8):
        f.write(INDENT + '{\n')
        for x in range(begin_x, begin_x + 8):
            if x % 2 == 0:
                f.write(INDENT * 2)
            for begin_y in range(0, MAT_SY, 4):
                f.write('0b')
                for y in range(begin_y + 3, begin_y - 1, -1):
                    f.write(str(mat[x][y]))
                is_last = x == begin_x + 8 - 1 and begin_y == MAT_SY - 4
                if not is_last:
                    f.write(', ')
                else:
                    f.write('\n' + INDENT + '}')
                    is_final_last = begin_x == MAT_SX - 8
                    if not is_final_last:
                        f.write(',')
                    else:
                        f.write('\n};')
            if x % 2 == 1:
                f.write('\n')
print("File written to " + file_name)
