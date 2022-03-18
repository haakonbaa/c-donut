#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    const unsigned pixels = rows * cols;
    // allocate character buffer
    char* buffer = (char*)malloc(sizeof(char) * pixels);
    memset(buffer, ' ', sizeof(char) * pixels);
    // allocate depth buffer
    float* zbuffer = (float*)malloc(sizeof(float) * pixels);
    memset(zbuffer, 0, sizeof(float) * pixels);
    // fill buffer with spaces
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
            buffer[bufferz * cols + bufferx] = '#';
        }
    }
    //  print buffer
    for (unsigned i = 0; i < pixels; i++) {
        putchar(buffer[i]);
    }

    // free buffer
    free(buffer);
    free(zbuffer);

    return 0;
}