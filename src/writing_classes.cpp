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
    const char _display;
    const int _numLines;

public:
    TriangleShape(char display, int numLines)
        : _display(display), _numLines(numLines) {}

    int getNumLines() const { return _numLines; }
    char getDisplay() const { return _display; }
};

class PrintTriangle {
private:
    const TriangleShape &_shape;

public:
    explicit PrintTriangle(const TriangleShape &triangle) : _shape(triangle) {}

    void display() {
        const char display = _shape.getDisplay();
        const int numLines = _shape.getNumLines();
        for (int ii = 1; ii <= numLines; ++ii) {
            // printing spaces
            for (int jj = 1; jj <= numLines - ii; ++jj) {
                cout << " ";
            }

            // printing display char
            for (int bb = 0; bb < (ii * 2) - 1; ++bb) {
                cout << display;
            }
            cout << endl;
        }
    }
};

class RectangleShape {
private:
    const int _row;
    const int _col;
    const char _display;

public:
    RectangleShape(char display, int row, int col)
        : _display(display), _row(row), _col(col) {}

    int getRow() const { return _row; }

    int getCol() const { return _col; }

    char getDisplay() const { return _display; }
};

class PrintRectangle {
private:
    const RectangleShape &_rect;

public:
    explicit PrintRectangle(const RectangleShape &rectangle)
        : _rect(rectangle) {}

    void display() {
        char display = _rect.getDisplay();
        int row = _rect.getRow();
        int col = _rect.getCol();

        for (int ii = 0; ii < row; ++ii) {
            for (int bb = 0; bb < col; ++bb) {
                cout << display;
            }

            cout << endl;
        }
    }
};

class AnyShape {
private:
    const int _dimension;
    const char _display;
    const int _shapeNum;

public:
    AnyShape(char dis, int dim, int shape)
        : _dimension(dim), _display(dis), _shapeNum(shape) {}

    int getDimsenion() const { return _dimension; }

    char getDisplay() const { return _display; }

    int getShapeNum() const { return _shapeNum; }
};

class PrintAnyShape {
private:
    const AnyShape &_shape;

public:
    explicit PrintAnyShape(const AnyShape &shape) : _shape(shape) {}

    void display() const {
        const int dim = _shape.getDimsenion();
        const int shapeNum = _shape.getShapeNum();
        const char disp = _shape.getDisplay();

        if (shapeNum == 1) {
            PrintTriangle show(TriangleShape(disp, dim));
            show.display();
        } else if (shapeNum == 2) {
            PrintRectangle show(RectangleShape(disp, dim, dim));
            show.display();
        } else {
            cout << "Error";
        }
    }
};

int main(int argc, const char *argv[]) {
    // tests 'TriangleShape' class and 'PrintTriangle' class
    {
        TriangleShape shape('@', 8);
        PrintTriangle render(shape);
        render.display();

        cout << endl;
    }

    // tests 'RectangleShape' class and 'PrintRectangle' class
    {
        RectangleShape shape('^', 5, 8);
        PrintRectangle render(shape);
        render.display();

        cout << endl;
    }

    // tests 'AnyShape' class and 'PrintAnyShape' class
    {
        PrintAnyShape render(AnyShape('?', 5, 1));
        render.display();
    }

    return 0;
}