#!/usr/bin/env python

import curses
import sys

import os.path


def start_curses():
    curses.noecho()
    curses.cbreak()
    curses.curs_set(0)


def create_screen():
    scrn = curses.initscr()
    scrn.keypad(1)
    return scrn


def read_char():
    c = screen.getch()
    try:
        return chr(c)
    except ValueError:
        return None


def get_char(val, on_cursor):
    if val == 1:
        return '=' if on_cursor else 'O'
    else:
        return '+' if on_cursor else '.'


def print_mat(mat, cx, cy):
    for j in range(MAT_SY):
        line = ""
        for i in range(MAT_SX):
            if should_group_cells and i % 8 == 0:
                line += ' '
            on_cursor = cx == i and cy == j
            line += get_char(mat[i][j], on_cursor) + " "
        screen.addstr(PADDING + j, PADDING + 1, line)
    sys.stdout.flush()


def try_exit(mat):
    x = MAX_HELP_WIDTH + PADDING * 2
    y = MAT_SY + PADDING * 2
    max_width = 43
    screen.addstr(y + 0, x, "Quit and export to file?")
    screen.addstr(y + 1, x, "Press Y to confirm or shift + D to discard.")
    sys.stdout.flush()
    char = read_char()

    if char == 'y' or char == 'Y':
        file_num = 1

        def file_name(num):
            return "image-" + str(num) + ".dat"

        while os.path.isfile(file_name(file_num)):
            file_num += 1

        f = open(file_name(file_num), 'w')
        for line in mat:
            for num in line:
                f.write("%d " % num)
            f.write('\n')
        f.close()
        screen.addstr(y + 3, x, "File written to " + file_name(file_num) + ".")
        screen.addstr(y + 4, x, "Press any key to exit the application...")
        sys.stdout.flush()
        read_char()
        return True
    elif char == 'D':
        return True
    else:
        screen.addstr(y + 0, x, ' ' * max_width)
        screen.addstr(y + 1, x, ' ' * max_width)
        return False


def check_bounds(cx, cy):
    if cx >= MAT_SX:
        cx -= MAT_SX
    if cx < 0:
        cx += MAT_SX
    if cy >= MAT_SY:
        cy -= MAT_SY
    if cy < 0:
        cy += MAT_SY
    return cx, cy


def parse_input(char):
    cx, cy = 0, 0
    if char == 'w':
        cy = -1
    if char == 's':
        cy = +1
    if char == 'a':
        cx = -1
    if char == 'd':
        cx = +1
    return cx, cy


MAX_HELP_WIDTH = 20
NUM_HELP_LINES = 4


def draw_help_screen():
    y = MAT_SY + 2 * PADDING
    x = 1 + PADDING
    screen.addstr(y + 0, x, "WSAD to move")
    screen.addstr(y + 1, x, "E/J to set pixel")
    screen.addstr(y + 2, x, "Q to export + quit")
    screen.addstr(y + 3, x, "H to hide help")


def clear_help_screen():
    y = MAT_SY + 2 * PADDING
    x = 1 + PADDING
    for j in range(y, y + NUM_HELP_LINES):
        screen.addstr(j, x, ' ' * MAX_HELP_WIDTH)


def run():
    cx, cy = 0, 0
    prev_x, prev_y = 0, 0
    mat = [[0 for x in range(MAT_SY)] for y in range(MAT_SX)]
    print_mat(mat, 0, 0)
    show_help = True
    draw_help_screen()

    while True:
        char = read_char()
        ax, ay = parse_input(char)
        cx += ax
        cy += ay

        if prev_x != cx or prev_y != cy:
            cx, cy = check_bounds(cx, cy)
            print_mat(mat, cx, cy)

        if char == 'q':
            if try_exit(mat):
                break

        if char == 'h':
            show_help = not show_help
            if show_help:
                draw_help_screen()
            else:
                clear_help_screen()

        if char == 'e' or char == 'j':
            mat[cx][cy] = 1 if mat[cx][cy] == 0 else 0
            print_mat(mat, cx, cy)

        prev_x, prev_y = cx, cy


def check_parameters():
    group_cells_cmd = '--group-cells'
    if len(sys.argv) > 1:
        if sys.argv[1] == group_cells_cmd:
            return True
        else:
            print("Usage: image-editor.py [" + group_cells_cmd + "]")
            exit(1)
    return False

PADDING = 1
MAT_SX, MAT_SY = 32, 8
should_group_cells = check_parameters()
screen = create_screen()
start_curses()

try:
    run()
finally:
    curses.nocbreak();
    screen.keypad(0);
    curses.echo()
    curses.endwin()
