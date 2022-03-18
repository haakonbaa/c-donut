#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

const double r = 1;
const double dTheta = 0.01;
const double dPhi = 0.01;
const double cameraDist = 1;
const double cameraWall = -1.1;

const char gradient[] = ".,-~:;=!*#$@";
const int gradlen = 12;

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int main() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    // printf("%d,%d", w.ws_row, w.ws_col);

    unsigned rows = w.ws_row;
    unsigned cols = w.ws_col;
    // allocate buffer
    char** buffer = (char**)malloc(sizeof(char*) * rows);
    for (unsigned int i = 0; i < rows; i++) {
        buffer[i] = (char*)malloc(sizeof(char) * cols);
    }
    // fill buffer with spaces
    for (unsigned r = 0; r < rows; r++) {
        for (unsigned c = 0; c < cols; c++) {
            buffer[r][c] = ' ';
        }
    }
    // fill in circle
    for (double theta = 0; theta < 2 * M_PI; theta += dTheta) {
        for (double phi = 0; phi < M_PI; phi += dPhi) {
            double x = r * cos(theta) * sin(phi);
            double y = r * sin(theta) * sin(phi);
            double z = r * cos(phi);
            double sz = (cameraDist * z) / (cameraDist + y - cameraWall);
            double sx = (cameraDist * x) / (cameraDist + y - cameraWall);
            int bz = (int)((sz + 1) * 0.5 * rows + 0.5);
            // characters are double as high as they are wide
            int bx = (int)(0.5 * cols + sx * rows + 0.5);
            unsigned int bufferx = max(0, min(bx, cols - 1));
            unsigned int bufferz = max(0, min(bz, rows - 1));
            buffer[bufferz][bufferx] = '#';
        }
    }
    //  print buffer
    for (unsigned r = 0; r < rows; r++) {
        for (unsigned c = 0; c < cols; c++) {
            putchar(buffer[r][c]);
        }
    }

    // free buffer
    for (unsigned int i = 0; i < rows; i++) {
        free(buffer[i]);
    }
    free(buffer);

    return 0;
}