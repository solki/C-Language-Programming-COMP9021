/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: The input is expected to consist of y_dim lines of x_dim       *
 *              members of {0, 1, 2, 3}, where x_dim and y_dim are at least    *
 *              equal to 2 and at most equal to MAX_X_DIM and MAX_Y_DIM,       *
 *              respectively, with possibly lines consisting of spaces only    *
 *              that will be ignored and with possibly spaces anywhere on the  *
 *              lines with digits.                                             *
 *              The xth digit n of the yth line, with 0 <= x < x_dim and       *
 *              0 <= y < y_dim, is to be associated with a point situated      *
 *              x * 0.5 cm to the right and y * 0.5 cm below an origin, is to  *
 *              be connected to the point 0.5 cm to its right                  *
 *              if n = 1 or n = 3, and is to be connected to the point         *
 *              0.5 cm below itself if n = 2 or n = 3.                         *
 *              The last digit on every line cannot be equal to 1 or 3, and    *
 *              the digits on the last line cannot be equal to 2 or 3, which   *
 *              ensures that the input encodes a maze, that is, a grid of      *
 *              width (x_dim - 1) * 0.5 cm and of height (y_dim - 1) * 0.5 cm, *
 *              with possibly gaps on the sides and inside. A point not        *
 *              connected to any of its neighbours is thought of as a pillar;  *
 *              a point connected to at least one of its neighbours is thought *
 *              of as part of a wall.                                          *
 *              We talk about "inner point" to refer to a point that lies      *
 *              (x + 0.5) * 0.5 cm to the right of and (y + 0.5) * 0.5 cm      *
 *              below the origin with 0 <= x < x_dim - 1 and                   *
 *              0 <= y < y_dim - 1.                                            *
 *                                                                             *
 *              Practically, the input will be stored in a file and its        *
 *              contents redirected to standard input. The program will be run *
 *              with either co command-line argument or with "print" as unique *
 *              command line argument; otherwise it will exit. The program     *
 *              will also exit immediately if the input is not as expected.    *
 *                                                                             *
 *              When provided with no command-line argument, the program does  *
 *              the following.                                                 *
 *              - It outputs the number of gates, that is, the number of       *
 *              consecutive points on one of the four sides of the maze   *
 *              that are not connected.                                        *
 *              - It outputs the number of sets of connected walls.            *
 *              - It outputs the number of inner points that cannot be         *
 *                accessed from any gate, starting from the point in the       *
 *                middle of a gate and going from inner points to neighbouring *
 *                inner points.                                                *
 *              - It outputs the number of maximal areas that can be accessed  *
 *                from at least one gate.                                      *
 *              - It outputs the number of accessible cul-de-sacs.             *
 *              - It outputs the number of paths consisting of inner points    *
 *                that go from a gate to another gate and such that the        *
 *                resulting path is choice-free, that is, such that leaving    *
 *                the path, at any inner point where that is possible,         *
 *                immediately leads into a cul-de-sac.                         *
 *                                                                             *
 *              When provided with "print" as unique command-line argument,    *
 *              the program outputs some .tex code to depict the maze          *
 *              as a tiz picture.                                              *
 *              - Walls are drawn in blue. There is a command for every        *
 *                longest segment that is part of a wall. Horizontal segments  *
 *                are drawn starting with the topmost leftmost segment and     *
 *                finishing with the bottommost rightmost segment. Then        *
 *                vertical segments are drawn starting with the topmost        *
 *                leftmost segment and finishing with the bottommost rightmost *
 *                segment.                                                     *
 *              - Pillars are drawn as green circles.                          *
 *              - Inner points in accessible cul-de-sacs are drawn as red      *
 *                crosses.                                                     *
 *              - The choice-free paths are drawn as dashed yellow lines.      *
 *                There is a command for every longest segment on such a       *
 *                path. Horizontal segments are drawn starting with the        *
 *                topmost leftmost segment and finishing with the bottommmost  *
 *                rightmost segment, with those segments that end at a gate    *
 *                sticking out by 0.25 cm. Then vertical segments are drawn    *
 *                starting with the topmost leftmost segment and finishing     *
 *                with the bottommmost rightmost segment, with those segments  *
 *                that end at a gate sticking out by 0.25 cm.                  *
 *                                                                             *
 * Written by Zhang Xi (3472528) for COMP9021                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_X_DIM 31
#define MAX_Y_DIM 41

int size_of_line = 0;
int size_of_column = 0;
int number[MAX_X_DIM * MAX_Y_DIM] = {-1};//store the original input into an array
int grid[MAX_Y_DIM][MAX_X_DIM]; //rewrite the original input from array to matrix
int grid_transformed[MAX_Y_DIM - 1][MAX_X_DIM - 1]; //to transform the original matrix

/*Here I declare my functions which are used in the programe.*/
bool get_input(void);
void rebuild_grid(void); //to rebuild the original matrix when needed
int looking_for_gates(void);
int looking_for_walls(void);
void the_wall(int i, int j);
void trans_grid(void);  // to build a transformed grid indicates the number of paths
void adjust_grid(void); // to adjust the modified transformed grid into a proper form
int get_element(int a, int b, int c);
void find_route(int i, int j);
int looking_for_inner_points(void);
int looking_for_access_areas(void);
int looking_for_dead_ends(void);
void mark_ends(int i, int j);
void count_ends(int i, int j);
int looking_for_one_way_paths(void);
void find_path(int i, int j);
void count_path(int i, int j);
void draw_horizontal_lines(void);
void draw_vertical_lines(void);
void draw_pillars(void);
void draw_cross(void);
void draw_horizontal_dash(void);
void draw_vertical_dash(void);
bool is_gate(int i, int j, int dir);

int main(int argc, char **argv) {
    if (argc > 2 || (argc == 2 && strcmp(argv[1], "print"))) {
        printf("I expect no command line argument or \"print\" as unique command line argument.\n");
        return EXIT_FAILURE;
    }
    if (!get_input()) {
        printf("Incorrect input.\n");
        return EXIT_FAILURE;
    }
    if (argc == 2) {
        printf("\\documentclass[10pt]{article}\n");
        printf("\\usepackage{tikz}\n");
        printf("\\usetikzlibrary{shapes.misc}\n");
        printf("\\usepackage[margin=0cm]{geometry}\n");
        printf("\\pagestyle{empty}\n");
        printf("\\tikzstyle{every node}=[cross out, draw, red]\n\n");
        printf("\\begin{document}\n\n");
        printf("\\vspace*{\\fill}\n");
        printf("\\begin{center}\n");
        printf("\\begin{tikzpicture}[x=0.5cm, y=-0.5cm, ultra thick, blue]\n");
        rebuild_grid();
        printf("%% Walls\n");
        draw_horizontal_lines();
        draw_vertical_lines();
        rebuild_grid();
        draw_pillars();
        rebuild_grid();
        trans_grid();
        looking_for_access_areas();
        looking_for_inner_points();
        looking_for_dead_ends();
        printf("%% Inner points in accessible cul-de-sacs\n");
        draw_cross();
        looking_for_one_way_paths();
        adjust_grid();
        printf("%% Entry-exit paths without intersections\n");
        draw_horizontal_dash();
        draw_vertical_dash();
        printf("\\end{tikzpicture}\n");
        printf("\\end{center}\n");
        printf("\\vspace*{\\fill}\n\n");
        printf("\\end{document}\n");
        return EXIT_SUCCESS;
    }
    rebuild_grid();
    int num_of_gates = looking_for_gates();
    if (num_of_gates == 0)
        printf("The maze has no gate.\n");
    if (num_of_gates == 1)
        printf("The maze has a single gate.\n");
    if (num_of_gates > 1)
        printf("The maze has %d gates.\n", num_of_gates);
    rebuild_grid();
    int num_of_walls = looking_for_walls();
    if (num_of_walls == 0)
        printf("The maze has no wall.\n");
    if (num_of_walls == 1)
        printf("The maze has walls that are all connected.\n");
    if (num_of_walls > 1)
        printf("The maze has %d sets of walls that are all connected.\n", num_of_walls);
    rebuild_grid();
    trans_grid();
    int num_of_accessible_areas = looking_for_access_areas();
    int num_of_inner_points = looking_for_inner_points();
    if (num_of_inner_points == 0)
        printf("The maze has no inaccessible inner point.\n");
    if (num_of_inner_points == 1)
        printf("The maze has a unique inaccessible inner point.\n");
    if (num_of_inner_points > 1)
        printf("The maze has %d inaccessible inner points.\n", num_of_inner_points);
    if (num_of_accessible_areas == 0)
        printf("The maze has no accessible area.\n");
    if (num_of_accessible_areas == 1)
        printf("The maze has a unique accessible area.\n");
    if (num_of_accessible_areas > 1)
        printf("The maze has %d accessible areas.\n", num_of_accessible_areas);
    int num_of_dead_ends = looking_for_dead_ends();
    if (num_of_dead_ends == 0)
        printf("The maze has no accessible cul-de-sac.\n");
    if (num_of_dead_ends == 1)
        printf("The maze has accessible cul-de-sacs that are all connected.\n");
    if (num_of_dead_ends > 1)
        printf("The maze has %d sets of accessible cul-de-sacs that are all connected.\n", num_of_dead_ends);
    int num_of_paths = looking_for_one_way_paths();
    if (num_of_paths == 0)
        printf("The maze has no entry-exit path with no intersection not to cul-de-sacs.\n");
    if (num_of_paths == 1)
        printf("The maze has a unique entry-exit path with no intersection not to cul-de-sacs.\n");
    if (num_of_paths > 1)
        printf("The maze has %d entry-exit paths with no intersections not to cul-de-sacs.\n", num_of_paths);    
}
/*Read the original input array. If satisfied, return true, otherwise return false. Then trun
 * the inputs into an executable form.*/
bool get_input(void) {
    int ch;
    bool indicator = true;
    int count_of_one_line = 0;
    int tmp;
    int k = 0;
    while((ch = getc(stdin)) != EOF) {
        if ((ch == '\n' || ch == ' ') && !count_of_one_line)
            continue;
        if (ch < '4' && ch >= '0') {
            count_of_one_line++;
            if (count_of_one_line > MAX_X_DIM) {
                indicator = false;
                break;
            }
            tmp = ch;
            number[k] = ch - '0';
            k++;
        }
        if ((ch >= '4' || ch < '0') && ch != ' ' && ch != '\n') {
            indicator = false;
            break;
        }
        if (ch == '\n') {
            if (size_of_column == 0 && (tmp == '1' || tmp == '3')) {
                indicator = false;
                break;
            }
            if (size_of_column == 0 && tmp != '1' && tmp != '3')
                size_of_line = count_of_one_line;
            if (size_of_column != 0 && (tmp == '1' || tmp == '3' || count_of_one_line != size_of_line)) {
                indicator = false;
                break;
            }
            size_of_column++;
            count_of_one_line = 0;
        }
    }
    if (indicator) {
        if (size_of_column > MAX_Y_DIM || size_of_column < 2 || size_of_line < 2)
            indicator = false;
        for (int i = size_of_column * size_of_line - 1; i >= (size_of_column - 1) * size_of_line; --i)
            if (number[i] == 2 || number[i] == 3) {
                indicator = false;
                break;
            }
    }
    return indicator;
}

void rebuild_grid(void) {
    for (int i = 0; i < size_of_column; ++i)
        for (int j = 0; j < size_of_line; ++j)
            grid[i][j] = number[size_of_line * i + j];
}

void trans_grid(void) {
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line - 1; ++j) {
            int a = grid[i][j];
            int b = grid[i][j+1];
            int c = grid[i+1][j];
            grid_transformed[i][j] = get_element(a, b, c);
        }   
}
/*Here I decompose the big matrix into 2*2 small matrix, and count the number of gates of small matrix
 * , which is the number of paths at the same position of original matrix.*/
int get_element(int a, int b, int c) {
    int count = 0;
    if (!a)
        count += 2;
    if (a == 2 || a == 1)
        count++;
    if (b == 0 || b == 1)
        count++;
    if (c == 0 || c == 2)
        count++;
    return count;
}

int looking_for_gates(void) {
    int num_of_gates = 0;
    for (int j = 0; j < size_of_line - 1; ++j) {
        if (grid[0][j] == 0 || grid[0][j] == 2)
            num_of_gates++;
    }
    for (int i = 0; i < size_of_column - 1; ++i) {
        if (grid[i][0] == 0 || grid[i][0] == 1)
            num_of_gates++;
    }
    for (int j = 0; j < size_of_line - 1; ++j) {
        if (grid[size_of_column-1][j] == 0)
            num_of_gates++;
    }
    for (int i = 0; i < size_of_column - 1; ++i) {
        if (grid[i][size_of_line-1] == 0)
            num_of_gates++;
    }
    return num_of_gates;
}

int looking_for_walls(void) {
    int num_of_walls = 0;
    for (int i = 0; i < size_of_column; ++i)
        for (int j = 0; j < size_of_line; ++j)
            if (grid[i][j]) {
                the_wall(i, j);
                num_of_walls++;
            }
    return num_of_walls;
}
/*I use this function to go alongside the wall and demolish it by replacing the number by 0.*/
void the_wall(int i, int j) {
    int i1, j1;
    switch (grid[i][j]) {
        case 1:
            grid[i][j] = 0;
            if ((grid[i-1][j] == 2 || grid[i-1][j] == 3) && i - 1 >= 0) {
                i1 = i - 1;
                j1 = j;
                the_wall(i1, j1);
            }
            if ((grid[i][j-1] == 1 || grid[i][j-1] == 3) && j - 1 >= 0) {
                i1 = i;
                j1 = j - 1;
                the_wall(i1, j1);
            }
            if (grid[i][j+1] && j + 1 < size_of_line) {
                i1 = i;
                j1 = j + 1;
                the_wall(i1, j1);
            }
            if ((grid[i-1][j+1] == 2 || grid[i-1][j+1] == 3)
                && i - 1 >= 0
                && j + 1 < size_of_line) {
                i1 = i - 1;
                j1 = j + 1;
                the_wall(i1, j1);
            }
            break;
        case 2:
            grid[i][j] = 0;
            if ((grid[i-1][j] == 2 || grid[i-1][j] == 3) && i - 1 >= 0) {
                i1 = i - 1;
                j1 = j;
                the_wall(i1, j1);
            }
            if ((grid[i][j-1] == 1 || grid[i][j-1] == 3) && j - 1 >= 0) {
                i1 = i;
                j1 = j - 1;
                the_wall(i1, j1);
            }
            if (grid[i+1][j] && i + 1 < size_of_column) {
                i1 = i + 1;
                j1 = j;
                the_wall(i1, j1);
            }
            if ((grid[i+1][j-1] == 1 || grid[i+1][j-1] == 3)
                && j - 1 >= 0
                && i + 1 < size_of_column) {
                i1 = i + 1;
                j1 = j - 1;
                the_wall(i1, j1);
            }
            break;
        case 3:
            grid[i][j] = 0;
            if ((grid[i-1][j] == 2 || grid[i-1][j] == 3) && i - 1 >= 0) {
                i1 = i - 1;
                j1 = j;
                the_wall(i1, j1);
            }
            if ((grid[i][j-1] == 1 || grid[i][j-1] == 3) && j - 1 >= 0) {
                i1 = i;
                j1 = j - 1;
                the_wall(i1, j1);
            }
            if (grid[i][j+1] && j + 1 < size_of_line) {
                i1 = i;
                j1 = j + 1;
                the_wall(i1, j1);
            }
            if (grid[i+1][j] && i + 1 < size_of_column) {
                i1 = i + 1;
                j1 = j;
                the_wall(i1, j1);
            }
            if ((grid[i-1][j+1] == 2 || grid[i-1][j+1] == 3)
                && i - 1 >= 0
                && j + 1 < size_of_line) {
                i1 = i - 1;
                j1 = j + 1;
                the_wall(i1, j1);
            }
            if ((grid[i+1][j-1] == 1 || grid[i+1][j-1] == 3)
                && j - 1 >= 0
                && i + 1 < size_of_column) {
                i1 = i + 1;
                j1 = j - 1;
                the_wall(i1, j1);
            }
            break;
    }
}
/*I use this function after finishing using the function "looking_for_access_area()".*/
int looking_for_inner_points(void) {
    int num_of_inner_points = 0;
    for (int i = 0; i < size_of_column -1; ++i)
        for (int j = 0; j < size_of_line - 1; ++j) {
            if (grid[i][j] != 4)
                num_of_inner_points++;
        }
    return num_of_inner_points;
}

int looking_for_access_areas(void) {
    int num_of_accessible_areas = 0;
    for (int i = 0, j = 0; j < size_of_line - 1; ++j)
        if (grid[i][j] == 0 || grid[i][j] == 2) {
            find_route(i, j);
            num_of_accessible_areas++;
        }
    for (int i = 0, j = 0; i < size_of_column - 1; ++i)
        if (grid[i][0] == 0 || grid[i][0] == 1) {
            find_route(i, j);
            num_of_accessible_areas++;
        }
    for (int i = size_of_column - 1, j = 0; j < size_of_line - 1; ++j)
        if (grid[size_of_column-1][j] == 0) {
            find_route(i, j);
            num_of_accessible_areas++;
        }
    for (int i = 0, j = size_of_line - 1; i < size_of_column - 1; ++i)
        if (grid[i][size_of_line-1] == 0) {
            find_route(i, j);
            num_of_accessible_areas++;
        }
    return num_of_accessible_areas;
}

void find_route(int i, int j) {
    int i1, j1;
    if (grid[i][j] == 0) {
        grid[i][j] = 4;
        if (grid[i][j-1] < 4
            && j-1 >= 0
            && i < size_of_column - 1) {
            i1 = i;
            j1 = j-1;
            find_route(i1, j1);
        }
        if (grid[i-1][j] < 4
            && i-1 >= 0
            && j < size_of_line - 1) {
            i1 = i-1;
            j1 = j;
            find_route(i1, j1);
        }
        if (grid[i][j+1] < 2
            && j+1 < size_of_line
            && i < size_of_column - 1) {
            i1 = i;
            j1 = j+1;
            find_route(i1, j1);
        }
        if (i+1 < size_of_column
            && j < size_of_line - 1
            && (grid[i+1][j] == 0 || grid[i+1][j] == 2)) {
            i1 = i+1;
            j1 = j;
            find_route(i1, j1);
        }
    }
    if (grid[i][j] == 1) {
        grid[i][j] = 4;
        if (grid[i][j-1] < 4
            && j-1 >= 0
            && i < size_of_column - 1) {
            i1 = i;
            j1 = j-1;
            find_route(i1, j1);
        }
        if (grid[i][j+1] < 2
            && j+1 < size_of_line
            && i < size_of_column - 1) {
            i1 = i;
            j1 = j+1;
            find_route(i1, j1);
        }
        if ((grid[i+1][j] == 0 || grid[i+1][j] == 2)
            && i+1 < size_of_column
            && j < size_of_line - 1) {
            i1 = i+1;
            j1 = j;
            find_route(i1, j1);
        }
    }
    if (grid[i][j] == 2) {
        grid[i][j] = 4;
        if (grid[i-1][j] < 4
            && i-1 >= 0
            && j < size_of_line - 1) {
            i1 = i-1;
            j1 = j;
            find_route(i1, j1);
        }
        if (grid[i][j+1] < 2
            && j+1 < size_of_line
            && i < size_of_column - 1) {
            i1 = i;
            j1 = j+1;
            find_route(i1, j1);
        }
        if ((grid[i+1][j] == 0 || grid[i+1][j] == 2)
            && i+1 < size_of_column
            && j < size_of_line - 1) {
            i1 = i+1;
            j1 = j;
            find_route(i1, j1);
        }
    }
    if (grid[i][j] == 3) {
        grid[i][j] = 4;
        if (grid[i][j+1] < 2
            && j+1 < size_of_line
            && i < size_of_column - 1) {
            i1 = i;
            j1 = j+1;
            find_route(i1, j1);
        }
        if ((grid[i+1][j] == 0 || grid[i+1][j] == 2)
            && i+1 < size_of_column
            && j < size_of_line - 1) {
            i1 = i+1;
            j1 = j;
            find_route(i1, j1);
        }
    }
}

int looking_for_dead_ends(void) {
    int num_of_dead_ends = 0;
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line - 1; ++j)
            if (grid[i][j] != 4)
                grid_transformed[i][j] = 0;
    rebuild_grid();
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line - 1; ++j)
            if (grid_transformed[i][j] == 1)
                mark_ends(i, j);  
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line - 1; ++j)
            if (grid_transformed[i][j] == 6) {
                count_ends(i, j);
                num_of_dead_ends++;
            }
    return num_of_dead_ends;
}

void mark_ends(int i, int j) {
    int i1, j1;
    grid_transformed[i][j] = 6;
    if (grid[i][j] == 0) {
        if (grid[i][j-1] < 4
            && grid_transformed[i][j-1] != 1
            && grid_transformed[i][j-1] != 6
            && j-1 >= 0) {
            grid_transformed[i][j-1]--;
            i1 = i;
            j1 = j-1;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
        if (grid[i-1][j] < 4
            && grid_transformed[i-1][j] != 1
            && grid_transformed[i-1][j] != 6
            && i-1 >= 0) {
            grid_transformed[i-1][j]--;
            i1 = i-1;
            j1 = j;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] != 1
            && grid_transformed[i][j+1] != 6
            && j+1 < size_of_line - 1) {
            grid_transformed[i][j+1]--;
            i1 = i;
            j1 = j+1;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] != 1
            && grid_transformed[i+1][j] != 6
            && i+1 < size_of_column - 1) {
            grid_transformed[i+1][j]--;
            i1 = i+1;
            j1 = j;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
    }
    if (grid[i][j] == 1) {
        if (grid[i][j-1] < 4
            && grid_transformed[i][j-1] != 1
            && grid_transformed[i][j-1] != 6
            && j-1 >= 0) {
            grid_transformed[i][j-1]--;
            i1 = i;
            j1 = j-1;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] != 1
            && grid_transformed[i][j+1] != 6
            && j+1 < size_of_line - 1) {
            grid_transformed[i][j+1]--;
            i1 = i;
            j1 = j+1;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] != 1
            && grid_transformed[i+1][j] != 6
            && i+1 < size_of_column - 1) {
            grid_transformed[i+1][j]--;
            i1 = i+1;
            j1 = j;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
    }
    if (grid[i][j] == 2) {
        if (grid[i-1][j] < 4
            && grid_transformed[i-1][j] != 1
            && grid_transformed[i-1][j] != 6
            && i-1 >= 0) {
            grid_transformed[i-1][j]--;
            i1 = i-1;
            j1 = j;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] != 1
            && grid_transformed[i][j+1] != 6
            && j+1 < size_of_line - 1) {
            grid_transformed[i][j+1]--;
            i1 = i;
            j1 = j+1;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] != 1
            && grid_transformed[i+1][j] != 6
            && i+1 < size_of_column - 1) {
            grid_transformed[i+1][j]--;
            i1 = i+1;
            j1 = j;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
    }
    if (grid[i][j] == 3) {
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] != 1
            && grid_transformed[i][j+1] != 6
            && j+1 < size_of_line - 1) {
            grid_transformed[i][j+1]--;
            i1 = i;
            j1 = j+1;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
        
        if ((grid[i+1][j] == 0 || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] != 1
            && grid_transformed[i+1][j] != 6
            && i+1 < size_of_column - 1) {
            grid_transformed[i+1][j]--;
            i1 = i+1;
            j1 = j;
            if (grid_transformed[i1][j1] == 1)
                mark_ends(i1, j1);
        }
    }
}

void count_ends(int i, int j) {
    int i1, j1;
    grid_transformed[i][j] = 7;
    if (grid[i][j] == 0) {
        if (grid[i][j-1] < 4
            && grid_transformed[i][j-1] == 6
            && j-1 >= 0) {
            i1 = i;
            j1 = j-1;
            count_ends(i1, j1);
        }
        if (grid[i-1][j] < 4
            && grid_transformed[i-1][j] == 6
            && i-1 >= 0) {
            i1 = i-1;
            j1 = j;
            count_ends(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] == 6
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            count_ends(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] == 6
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            count_ends(i1, j1);
        }
    }
    if (grid[i][j] == 1) {
        if (grid[i][j-1] < 4
            && grid_transformed[i][j-1] == 6
            && j-1 >= 0) {
            i1 = i;
            j1 = j-1;
            count_ends(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] == 6
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            count_ends(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] == 6
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            count_ends(i1, j1);
        }
    }
    if (grid[i][j] == 2) {
        if (grid[i-1][j] < 4
            && grid_transformed[i-1][j] == 6
            && i-1 >= 0) {
            i1 = i-1;
            j1 = j;
            count_ends(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] == 6
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            count_ends(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] == 6
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            count_ends(i1, j1);
        }
    }
    if (grid[i][j] == 3) {
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] == 6
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            count_ends(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] == 6
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            count_ends(i1, j1);
        }
    }
}

int looking_for_one_way_paths(void) {
    int num_of_paths = 0;
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line -1; ++j)
            if (grid_transformed[i][j] == 4)
                find_path(i, j);
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line -1; ++j)
            if (grid_transformed[i][j] == 3)
                find_path(i, j);      
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line -1; ++j)
            if (grid_transformed[i][j] == 2) {
                count_path(i, j);
                num_of_paths++;
            }
    return num_of_paths;
}

void find_path(int i, int j) {
    int i1, j1;
    grid_transformed[i][j] = 8;
    if (grid[i][j] == 0) {
        if (grid[i][j-1] < 4
            && grid_transformed[i][j-1] != 7
            && grid_transformed[i][j-1] != 8
            && j-1 >= 0) {
            i1 = i;
            j1 = j-1;
            find_path(i1, j1);
        }
        if (grid[i-1][j] < 4
            && grid_transformed[i-1][j] != 7
            && grid_transformed[i-1][j] != 8
            && i-1 >= 0) {
            i1 = i-1;
            j1 = j;
            find_path(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] != 7
            && grid_transformed[i][j+1] != 8
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            find_path(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] != 7
            && grid_transformed[i+1][j] != 8
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            find_path(i1, j1);
        }
    }
    if (grid[i][j] == 1) {
        if (grid[i][j-1] < 4
            && grid_transformed[i][j-1] != 7
            && grid_transformed[i][j-1] != 8
            && j-1 >= 0) {
            i1 = i;
            j1 = j-1;
            find_path(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] != 7
            && grid_transformed[i][j+1] != 8
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            find_path(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] != 7
            && grid_transformed[i+1][j] != 8
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            find_path(i1, j1);
        }
    }
    if (grid[i][j] == 2) {
        if (grid[i-1][j] < 4
            && grid_transformed[i-1][j] != 7
            && grid_transformed[i-1][j] != 8
            && i-1 >= 0) {
            i1 = i-1;
            j1 = j;
            find_path(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] != 7
            && grid_transformed[i][j+1] != 8
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            find_path(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] != 7
            && grid_transformed[i+1][j] != 8
            && i+1 < size_of_column -1) {
            i1 = i+1;
            j1 = j;
            find_path(i1, j1);
        }
    }
    if (grid[i][j] == 3) {
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] != 7
            && grid_transformed[i][j+1] != 8
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            find_path(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] != 7
            && grid_transformed[i+1][j] != 8
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            find_path(i1, j1);
        }
    }
}

void count_path(int i, int j) {
    int i1, j1;
    grid_transformed[i][j] = -2;
    if (grid[i][j] == 0) {
        if (grid[i][j-1] < 4
            && grid_transformed[i][j-1] == 2
            && j-1 >= 0) {
            i1 = i;
            j1 = j-1;
            count_path(i1, j1);
        }
        if (grid[i-1][j] < 4
            && grid_transformed[i-1][j] == 2
            && i-1 >= 0) {
            i1 = i-1;
            j1 = j;
            count_path(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] == 2
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            count_path(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] == 2
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            count_path(i1, j1);
        }
    }
    if (grid[i][j] == 1) {
        if (grid[i][j-1] < 4
            && grid_transformed[i][j-1] == 2
            && j-1 >= 0) {
            i1 = i;
            j1 = j-1;
            count_path(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] == 2
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            count_path(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] == 2
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            count_path(i1, j1);
        }
    }
    if (grid[i][j] == 2) {
        if (grid[i-1][j] < 4
            && grid_transformed[i-1][j] == 2
            && i-1 >= 0) {
            i1 = i-1;
            j1 = j;
            count_path(i1, j1);
        }
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] == 2
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            count_path(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] == 2
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            count_path(i1, j1);
        }
    }
    if (grid[i][j] == 3) {
        if (grid[i][j+1] < 2
            && grid_transformed[i][j+1] == 2
            && j+1 < size_of_line - 1) {
            i1 = i;
            j1 = j+1;
            count_path(i1, j1);
        }
        if ((!grid[i+1][j] || grid[i+1][j] == 2)
            && grid_transformed[i+1][j] == 2
            && i+1 < size_of_column - 1) {
            i1 = i+1;
            j1 = j;
            count_path(i1, j1);
        }
    }
}

void draw_horizontal_lines(void) {
    int x = 0;
    for (int i = 0; i < size_of_column; ++i)
        for (int j = 0; j < size_of_line; ++j) {
            if ((grid[i][j] == 1 || grid[i][j] == 3)
                && x == 0) {
                printf("    \\draw (%d,%d) -- ", j, i);
                x = j + 1;
            }
            if ((grid[i][j] == 1 || grid[i][j] == 3)
                && x != 0)
                x = j + 1;
            if ((grid[i][j] == 0 || grid[i][j] == 2)
                && x != 0) {
                printf("(%d,%d);\n", x, i);
                x = 0;
            }
        }
}

void draw_vertical_lines(void) {
    int y = 0;
    for (int j = 0; j < size_of_line; ++j)
        for (int i = 0; i < size_of_column; ++i) {
            if ((grid[i][j] == 2 || grid[i][j] == 3)
                && y == 0) {
                printf("    \\draw (%d,%d) -- ", j, i);
                y = i + 1;
            }
            if ((grid[i][j] == 2 || grid[i][j] == 3)
                && y != 0)
                y = i + 1;
            if ((grid[i][j] == 0 || grid[i][j] == 1)
                && y != 0) {
                printf("(%d,%d);\n", j, y);
                y = 0;
            }
        }
}

void draw_pillars(void) {
    printf("%% Pillars\n");
    for (int i = 0; i < size_of_column; ++i)
        for (int j = 0; j < size_of_line; ++j) {
            if (grid[i][j] == 0
                && grid[i-1][j] < 2
                && (grid[i][j-1] == 0 || grid[i][j-1] == 2))
                printf("    \\fill[green] (%d,%d) circle(0.2);\n", j, i);
        }
}

void draw_cross(void) {
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line - 1; ++ j) {
            if (grid_transformed[i][j] == 7)
                printf("    \\node at (%0.1f,%0.1f) {};\n", j + 0.5, i + 0.5);
        }
}
/*When at the gate of path, it ends up sticking out by 0.25cm. I enlarge the grid horizontally by 1 column at each
 * side so that I needn't to identify whether it's a gate or not. So is the situation when printing vertical dash.*/
void draw_horizontal_dash(void) {
    bool gate = 0; //to identify if it's a gate
    int start = 0;
    int dir = 1;
    int grid_new[size_of_column-1][size_of_line+1];
    int grid_new2[size_of_column][size_of_line+2];
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line + 1; ++j)
            grid_new[i][j] = 0;
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line - 1; ++j) {
            grid_new[i][j+1] = grid_transformed[i][j];
            gate = is_gate(i, j, dir);
            if (gate && j == 0)
                grid_new[i][j] = 2;
            if (gate && j == size_of_line - 2)
                grid_new[i][j+2] = 2;
        }
    for (int i = 0; i < size_of_column; ++i)
        for (int j = 0; j < size_of_line + 2; ++j)
            grid_new2[i][j] = 0;
    for (int i = 0; i < size_of_column; ++i)
        for (int j = 0; j < size_of_line; ++j)
            grid_new2[i][j+1] = grid[i][j];
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line + 1; ++j) {
            if (grid_new[i][j] == 2
                && grid_new[i][j+1] == 2
                && grid_new2[i][j+1] < 2
                && !start) {
                printf("    \\draw[dashed, yellow] (%0.1f,%0.1f) -- ", j - 1 + 0.5, i + 0.5);
                start = 1;
            }
            if (grid_new[i][j] == 2
                && (grid_new[i][j+1] != 2
                    || grid_new2[i][j+1] >= 2
                    || j == size_of_line)
                && start) {
                printf("(%0.1f,%0.1f);\n", j - 1 + 0.5, i + 0.5);
                start = 0;
            }
        }
}

void draw_vertical_dash(void) {
    bool gate = 0;
    int start = 0;
    int dir = 2;
    int grid_new[size_of_column+1][size_of_line-1];
    int grid_new2[size_of_column+2][size_of_line];
    for (int i = 0; i < size_of_column + 1; ++i)
        for (int j = 0; j < size_of_line - 1; ++j)
            grid_new[i][j] = 0;
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line - 1; ++j) {
            grid_new[i+1][j] = grid_transformed[i][j];
            gate = is_gate(i, j, dir);
            if (gate && i == 0)
                grid_new[i][j] = 2;
            if (gate && i == size_of_column - 2)
                grid_new[i+2][j] = 2;
        }
    for (int i = 0; i < size_of_column + 2; ++i)
        for (int j = 0; j < size_of_line; ++j)
            grid_new2[i][j] = 0;
    for (int i = 0; i < size_of_column; ++i)
        for (int j = 0; j < size_of_line; ++j)
            grid_new2[i+1][j] = grid[i][j];
    for (int j = 0; j < size_of_line - 1; ++j)
        for (int i = 0; i < size_of_column + 1; ++i) {
            if (grid_new[i][j] == 2
                && grid_new[i+1][j] == 2
                && grid_new2[i+1][j] != 1
                && grid_new2[i+1][j] != 3
                && !start) {
                printf("    \\draw[dashed, yellow] (%0.1f,%0.1f) -- ", j + 0.5, i - 1 + 0.5);
                start = 1;
            }
            if (grid_new[i][j] == 2
                && (grid_new[i+1][j] != 2
                    ||grid_new2[i+1][j] == 1
                    || grid_new2[i+1][j] == 3
                    || i == size_of_column)
                && start) {
                printf("(%0.1f,%0.1f);\n", j + 0.5, i - 1 + 0.5);
                start = 0;
            }
        }
}

bool is_gate(int i, int j, int dir) {
    if (dir == 1) {
        if (j == 0 && grid[i][j] < 2 && grid_transformed[i][j] == 2)
            return 1;
        if (j == size_of_line - 2 && grid[i][j+1] == 0 && grid_transformed[i][j] == 2)
            return 1;
        else
            return 0;
    }
    if (dir == 2) {
        if (i == 0 && (grid[i][j] == 0 || grid[i][j] == 2) && grid_transformed[i][j] == 2)
            return 1;
        if (i == size_of_column - 2 && grid[i+1][j] == 0 && grid_transformed[i][j] == 2)
            return 1;
        else
            return 0;
    }
}

void adjust_grid(void) {
    for (int i = 0; i < size_of_column - 1; ++i)
        for (int j = 0; j < size_of_line - 1; ++j) {
            if (grid_transformed[i][j] != -2)
                grid_transformed[i][j] = 0;
            if (grid_transformed[i][j] == -2)
                grid_transformed[i][j] = 2;
        }
}
