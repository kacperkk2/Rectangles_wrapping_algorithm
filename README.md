AUTHOR:
Kacper Klimczuk
kacper.k2@o2.pl
K.Klimczuk@stud.elka.pw.edu.pl

PROJECT NAME:
Rectangles wrapping algorithm - unused space in wrapped area

PROGRAM ACTIVATION OPTIONS:
--help		
show proper usage of program, possible options of activation, 
		and output for those options.
-g [NUMBER]	runs program with generator, which generates convex hull 
		and rectangles in it. That data will be input for algorithm.
		On output will be printed: solution for monte carlo check,
		solution for algorithm, elapsed time (for algorithm work).
-a [DATA]	runs program with data specified by user. After -a you should
		put one number which specifies number of rectangles, and after
		that 5*number_of_rectangles additional numbers which means:
		x, y coord of i rectangle center, height, width of i rectangle
		and angle of rotation i rectangle

DECOMPOSITION:
Basic problem:
algorithm.h - functions used in algorithm
generator.h - only generator function and monte_carlo for check
generator_support.h - small functions which generator uses
Rectangle.h - rectangle class

Additional problem:
algorithm_add.h - includes algorithm.h and specifies one additional functions for overlapping
generator_add.h - new generator for additional problem, uses functions of generator_support.h
Rectangle_add.h - rectangle_add class which inherits from Rectangle.h and implements one extra method and covers one method

OUTPUT, INPUT CONVENTION:
Input: for -a option it should be 1 + number_of_rectangles*5 numbers,
with space after every and with enter after last number.
Output: will be printed % od used space counted by algorithm and monte carlo
(in option with generator), and time elapsed during algorithm work.

GENERAL ALGORITHM:
In basic problem:
1. Make rectangles
2. Count rectangles field
3. Find convex hull
4. Count convex hull field
5. Print rectangles_field/convex_field

In additional problem:
5. Search for overlapping and subtract that field
6. Print rectangles_field/convex_field

In program were used mainly: vectors, arrays and my classes: Rectangle, Point, Line. 
