# C-Language-Programming-COMP9021
Assignment2 of COMP9021

Maze:
1 Aims
2
• • • •
read data from standard input and store them in an array; perform operations on arrays;
execute tests and repetitions;
design recursive procedures.
General presentation
Assignment 2 COMP9021, Session 1, 2014
The main purpose of the assignment is to let you practice the following programming techniques:
You will design and implement a program that will
• analyse the various characteristics of a maze, represented by a particular coding of its basic con- stituents into numbers stored in a file whose contents is to be redirected to standard input, and
• – either display those characteristics
– or output some Latex code, to be redirected to a file, from which a pictorial representation of
the maze can be produced.
The representation of the maze is based on a coding with the four digits 0, 1, 2 and 3 such that
• 0 codes points that are connected to neither their right nor below neighbours
• 1 codes points that are connected to their right neighbours but not to their below ones:
• 2 codes points that are connected to their below neighbours but not to their right ones:
• 3 codes points that are connected to both their right and below neighbours:
A point that is connected to none of their left, right, above and below neighbours represents a pillar: Analysing the maze will allow you to also represent:
• cul-de-sacs:
• certain kinds of paths:
      1
3 Examples 3.1 First example
Given a file named maze1.txt whose contents is
10221230 32212022 30113100 20300120 32201232 10011000
your program when run as a.out <maze1.txt should output
The maze has 12 gates.
The maze has 8 sets of walls that are all connected.
The maze has 2 inaccessible inner points.
The maze has 4 accessible areas.
The maze has 3 sets of accessible cul-de-sacs that are all connected.
The maze has a unique entry-exit path with no intersection not to cul-de-sacs.
and when run as a.out print <maze1.txt should produce some output which if redirected to a file, say maze1.tex1, can be given as argument to pdflatex to produce a file named maze1.pdf that views as follows.
1by actually running a.out print <maze1.txt >maze1.tex
                                         2
3.2 Second example
Given a file named maze2.txt whose contents is
022302120222
222223111032
301322130302
312322232330
001000100000
your program when run as a.out <maze2.txt should output
The maze has 20 gates.
The maze has 4 sets of walls that are all connected.
The maze has 4 inaccessible inner points.
The maze has 13 accessible areas.
The maze has 11 sets of accessible cul-de-sacs that are all connected.
The maze has 5 entry-exit paths with no intersections not to cul-de-sacs.
and when run as a.out print <maze2.txt should produce some output which if redirected to a file, say maze2.tex2, can be given as argument to pdflatex to produce a file named maze2.pdf that views as follows.
2by actually running a.out print <maze2.txt >maze2.tex
                                                                  3
3.3 Third example
Given a file named labyrinth1.txt whose contents is
31111111132
21122131202
33023022112
20310213122
31011120202
21230230112
30223031302
03122121212
22203110322
22110311002
11111101110
your program when run as a.out <labyrinth1.txt should output
The maze has 2 gates.
The maze has 2 sets of walls that are all connected.
The maze has no inaccessible inner point.
The maze has a unique accessible area.
The maze has 8 sets of accessible cul-de-sacs that are all connected.
The maze has a unique entry-exit path with no intersection not to cul-de-sacs.
andwhenrunasa.out print <labyrinth1.txtshouldproducesomeoutputwhichifredirectedtoafile, say labyrinth1.tex3, can be given as argument to pdflatex to produce a file named labyrinth1.pdf that views as follows.
                                                                                                                                                   3by actually running a.out print <labyrinth1.txt >labyrinth1.tex
4
4 Detailed description 4.1 Input
The input is expected to consist of ydim lines of xdim members of {0, 1, 2, 3}, where xdim and ydim are at least equal to 2 and at most equal to 31 and 41, respectively, with possibly lines consisting of spaces only that will be ignored and with possibly spaces anywhere on the lines with digits. If n is the xth digit of theyth linewithdigits,with0≤x<xdim and0≤y<ydim,then
• n is to be associated with a point situated x × 0.5 cm to the right and y × 0.5 cm below an origin, • nistobeconnectedtothepoint0.5cmtoitsrightjustincasen=1orn=3,and
• nistobeconnectedtothepoint0.5cmbelowitselfjustincasen=2orn=3.
The last digit on every line with digits cannot be equal to 1 or 3, and the digits on the last line with digits cannot be equal to 2 or 3, which ensures that the input encodes a maze, that is, a grid of width (xdim − 1) × 0.5 cm and of height (ydim − 1) × 0.5 cm (hence of maximum width 15 cm and of maximum height 20 cm), with possibly gaps on the sides and inside. A point not connected to any of its neighbours is thought of as a pillar; a point connected to at least one of its neighbours is thought of as part of a wall.
We talk about inner point to refer to a point that lies (x+0.5)×0.5 cm to the right of and (y+0.5)×0.5 cmbelowtheoriginwith0≤x<xdim −1and0≤y<ydim −1.
4.2 Output
If not run as a.out or as a.out print (followed by <filename where filename is the name of a file that stores the input) then the program should print out a single line that reads
I expect no command line argument or "print" as unique command line argument.
and immediately exit. Otherwise, if the input is incorrect, that is, does not satisfy the conditions spelled out in the previous section, then the program should print out a single line that reads
Incorrect input.
and immediately exit.
4.2.1 When a.out is run with no command-line argument
If the input is correct and the program is run as a.out (followed by <filename where filename is the name of a file that stores the input) then the program should output a first line that reads one of
The maze has no gate.
The maze has a single gate.
The maze has N gates.
with N an appropriate integer at least equal to 2, a second line that reads one of 5
The maze has no wall.
The maze has walls that are all connected.
The maze has N sets of walls that are all connected.
with N an appropriate integer at least equal to 2, a third line that reads one of
The maze has no inaccessible inner point.
The maze has a unique inaccessible inner point.
The maze has N inaccessible inner points.
with N an appropriate integer at least equal to 2, a fourth line that reads one of
The maze has no accessible area.
The maze has a unique accessible area.
The maze has N accessible areas.
with N an appropriate integer at least equal to 2, a fifth line that reads one of
The maze has no accessible cul-de-sac.
The maze has accessible cul-de-sacs that are all connected.
The maze has N sets of accessible cul-de-sacs that are all connected.
with N an appropriate integer at least equal to 2, and a sixth line that reads one of
The maze has no entry-exit path with no intersection not to cul-de-sacs.
The maze has a unique entry-exit path with no intersection not to cul-de-sacs.
The maze has N entry-exit paths with no intersections not to cul-de-sacs.
with N an appropriate integer at least equal to 2.
• A gate is any pair of consecutive points on one of the four sides of the maze that are not connected.
• An inaccessible inner point is an inner point that cannot be reached from any gate.
• An accessible area is a maximal set of inner points that can all be accessed from the same gate (so the number of accessible inner points is at most equal to the number of gates).
• A set of accessible cul-de-sacs that are all connected is a maximal set S of connected inner points that can all be accessed from the same gate g and such that for all points p in S, if p has been accessed from g for the first time, then either p is in a dead end or moving on without ever getting back leads into a dead end.
• An entry-exit path with no intersections not to cul-de-sacs is a maximal set S of connected inner points that go from a gate to another (necessarily different) gate and such that for all points p in S, there is only one way to move on from p without getting back and without entering a cul-de-sac.
Pay attention to the expected format, including spaces. Note that your program should output no blank line. For a given test, the output of your program will be compared with the expected output; your program will pass the test if and only if both outputs are absolutely identical, character for character,
6
including spaces. For the provided examples, the expected outputs are available in files that end in _output.txt. To check that the output of your program on those examples is correct, you can redirect it to a file and compare the contents of that file with the contents of the appropriate _output.txt file using the diff command. If diff silently exits then your program passes the test; otherwise it fails it. For instance, run
and then
  a.out <maze1.txt >maze1_my_output.txt
diff maze1_my_output.txt maze1_output.txt
to check whether your program succeeds on the first provided example.
4.3 When a.out is run with print as command-line argument
If the input is correct and the program is run as a.out print (followed by <filename where filename is the name of a file that stores the input) then the program should output some lines that redirected to a file, say maze.tex, can be given as an argument to pdflatex to produce a file named maze.pdf that depicts the maze. The provided examples will show you what maze.tex should contain.
• Walls are drawn in blue. There is a command for every longest segment that is part of a wall. Horizontal segments are drawn starting with the topmost leftmost segment and finishing with the bottommost rightmost segment. Then vertical segments are drawn starting with the topmost leftmost segment and finishing with the bottommost rightmost segment.
• Pillars are drawn as green circles.
• Inner points in accessible cul-de-sacs are drawn as red crosses.
• The paths with no intersection not to cul-de-sacs are drawn as dashed yellow lines. There is a command for every longest segment on such a path. Horizontal segments are drawn starting with the topmost leftmost segment and finishing with the bottommost rightmost segment, with those segments that end at a gate sticking out by 0.25 cm. Then vertical segments are drawn starting with the topmost leftmost segment and finishing with the bottommost rightmost segment, with those segments that end at a gate sticking out by 0.25 cm.
Pay attention to the expected format, including spaces and blank lines. Lines that start with % are comments; there are 4 such lines, that have to be present even when there is no item to be displayed of the kind described by the comment. The output of your program redirected to a file will be compared with the expected output saved in a file (of a different name of course) using the diff command. For your program to pass the associated test, diff should silently exit, which requires that the contents of both files be absolutely identical, character for character, including spaces and blank lines. Check your program on the provided examples using the associated .tex files. For instance, run
and then
a.out print <maze1.txt >my_maze1.tex
    diff my_maze1.tex maze1.tex
to check whether your program succeeds on the first provided example. 7
