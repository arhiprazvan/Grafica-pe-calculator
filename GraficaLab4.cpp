#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "glut.h"

unsigned char prevKey;

class CartesianGrid {
    int rows; // rows
    int cols; // columns
    float xStart; // left
    float yStart; // down
    float padding; // padding
    float colDim; // width of a cell
    float rowDim; // height of a cell
    float radius; // radius of pixel

public:
    CartesianGrid(int rows, int cols) : rows(rows), cols(cols) {
        this->padding = 0.1;
        this->xStart = -1 + padding;
        this->yStart = -1 + padding;
        this->colDim = 2 * (1 - padding) / float(cols);
        this->rowDim = 2 * (1 - padding) / float(rows);
        this->radius = std::min(this->rowDim, this->colDim) / 4;
        drawGrid();
    }

    void drawGrid() {
        glColor3f(0, 0, 0);

        float x = this->xStart;
        for (int i = 0; i <= this->cols; i++) {
            glBegin(GL_LINES);
            glVertex2f(x, -1 + this->padding);
            glVertex2f(x, +1 - this->padding);
            x += colDim;
            glEnd();
        }

        float y = this->yStart;
        for (int j = 0; j <= this->rows; j++) {
            glBegin(GL_LINES);
            glVertex2d(-1 + this->padding, y);
            glVertex2d(+1 - this->padding, y);
            y += rowDim;
            glEnd();
        }

        glFlush();
    }

    void writePixel(int i, int j) {
        double x = this->xStart + i * this->colDim;
        double y = this->yStart + j * this->rowDim;
        double pi = 4 * atan(1.0);
        int segments = 1000;

        glColor3f(0.3, 0.3, 0.3);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int k = 0; k <= segments; k++) {
            float xAux = x + this->radius * cos(2 * pi * k / segments);
            float yAux = y + this->radius * sin(2 * pi * k / segments);
            glVertex2f(xAux, yAux);
        }
        glEnd();
    }

    void Segment3(int x0, int y0, int xn, int yn) {
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINES);
        glVertex2f(this->xStart + x0 * this->colDim, this->yStart + y0 * this->rowDim);
        glVertex2f(this->xStart + xn * this->colDim, this->yStart + yn * this->rowDim);
        glEnd();

        if (x0 > xn) {
            std::swap(x0, xn);
            std::swap(y0, yn);
        }

        int dx = abs(xn - x0);
        int dy = abs(yn - y0);

        int panta = dy / dx;

        if (dx == 0) {
            int ymin = y0 < yn ? y0 : yn;
            int ymax = y0 > yn ? y0 : yn;
            for (int y = ymin; y <= ymax; y++) {
                if (x0 < xn && y0 < yn && panta == 0) {
                    this->drawMultiplePixels2(x0, y);
                }
                else
                    this->drawMultiplePixels(x0, y);
            }
        }
        else if (dy == 0) {
            int xmin = x0 < xn ? x0 : xn;
            int xmax = x0 > xn ? x0 : xn;
            for (int x = xmin; x <= xmax; x++) {
                if (x0 < xn && y0 < yn && panta == 0) {
                    this->drawMultiplePixels2(x, y0);
                }
                else
                    this->drawMultiplePixels(x, y0);
            }
        }
        else if (dx >= dy) {
            int d = 2 * dy - dx;
            int dE = 2 * dy;
            int dNE = 2 * (dy - dx);
            int x = x0, y = y0;

            while (x <= xn) {
                if ((y0 >= yn && y >= yn) || (y0 <= yn && y <= yn)) {
                    if (x0 < xn && y0 < yn && panta == 0) {
                        this->drawMultiplePixels2(x, y);
                    }
                    else
                        this->drawMultiplePixels(x, y);
                }
                if (d <= 0) {
                    d += dE;
                    x++;
                }
                else {
                    d += dNE;
                    x++;
                    y += y0 < yn ? 1 : -1;
                }
            }
        }
        else {
            int d = 2 * dx - dy;
            int dN = 2 * dx;
            int dNE = 2 * (dx - dy);
            int x = x0, y = y0;

            while (x <= xn) {
                if ((y0 >= yn && y >= yn) || (y0 <= yn && y <= yn)) {
                    if (x0 < xn && y0 < yn && panta == 0) {
                        this->drawMultiplePixels2(x, y);
                    }
                    else
                        this->drawMultiplePixels(x, y);
                }
                if (d < 0) {
                    d += dN;
                    y += y0 < yn ? 1 : -1;
                }
                else {
                    d += dNE;
                    x++;
                    y += y0 < yn ? 1 : -1;
                }
            }
        }
    }

    void drawMultiplePixels2(int x, int y) {
        this->writePixel(x, y); 
    }

    void drawMultiplePixels(int x, int y) {
        this->writePixel(x, y);
        if (x + 1 <= 20) {
            this->writePixel(x + 1, y);
        }
        if (x - 1 >= this->xStart) {
            this->writePixel(x - 1, y);
        }
        if (y + 1 <= 20) {
            this->writePixel(x, y + 1);
        }
        if (y - 1 >= this->yStart) {
            this->writePixel(x, y - 1);
        }
    }

    void Cerc4(int radius) {
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        double pi = 4 * atan(1.0);
        int segments = 10000;

        for (int k = 0; k < segments; k++) {
            float xAux = this->xStart + (radius * this->colDim) * cos(2 * pi * k / segments);
            float yAux = this->yStart + (radius * this->rowDim) * sin(2 * pi * k / segments);
            if (
                xAux <= 1 - this->padding && yAux <= 1 - this->padding &&
                xAux >= -1 + this->padding && yAux >= -1 + this->padding
                ) {
                glVertex2f(xAux, yAux);
            }
        }
        glEnd();

        int x = 0;
        int y = radius;
        int d = 1 - radius;
        int dE = 3;
        int dSE = -2 * radius + 5;

        this->drawMultiplePixels(y, x);

        while (y > x) {
            if (d < 0) {
                d += dE;
                dE += 2;
                dSE += 2;
            }
            else {
                d += dSE;
                dE += 2;
                dSE += 4;
                y--;
            }
            x++;

            this->drawMultiplePixels(y, x);
            
        }
    }
};

void Init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(1);
    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    CartesianGrid cg = CartesianGrid(10, 20);

    switch (prevKey) {
    case '1':
        cg.Segment3(0, 0, 20, 7);
        break;
    case '2':
        cg.Segment3(0, 20, 20, 4);
        break;
    case '3':
        cg.Segment3(19, 0, 2, 6);
        break;
    case '4':
        cg.Segment3(16, 0, 1, 20);
        break;
    case '5':
        cg.Segment3(10, 5, 20, 5);
        break;
    case '6':
        cg.Cerc4(18);
        break;
    }

    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow(argv[0]);
    Init();
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(KeyboardFunc);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}
