/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 2 - Day 3: Writing simple C++ classes
 */

#include <iostream>
using namespace std;

class TriangleShape {
private:
    char _display;
    int _numLines;

public:
    TriangleShape(char display, int numLines)
        : _display(display), _numLines(numLines) {}
};

class PrintTriangle {
private:
    const TriangleShape &shape;

public:
    PrintTriangle(const TriangleShape &triangle) : shape(triangle) {}
};
