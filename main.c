#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

const double r = 1;
const double dTheta = 0.01;
const double dPhi = 0.01;
const double cameraDist = 9;
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
    // hide cursor
    printf("\033[?25l");

    unsigned rows = w.ws_row;
    unsigned cols = w.ws_col;
    const unsigned pixels = rows * cols;
    // allocate character and depth buffer
    char* buffer = (char*)malloc(sizeof(char) * pixels);
    float* zbuffer = (float*)malloc(sizeof(float) * pixels);
    // set default light angle
    double lightAngleTheta = 0;
    double lightAnglePhi = M_PI * 0.5;
    while (1) {
        // set default values in all buffers
        memset(buffer, ' ', sizeof(char) * pixels);
        for (unsigned int i = 0; i < pixels; i++) {
            zbuffer[i] = FLT_MAX;
        }
        // update light normal from lightAngleTheta
        double lx = cos(lightAngleTheta) * sin(lightAnglePhi);
        double ly = sin(lightAngleTheta) * sin(lightAnglePhi);
        double lz = cos(lightAnglePhi);
        // fill in circle
        for (double theta = 0; theta < 2 * M_PI; theta += dTheta) {
            for (double phi = 0; phi < M_PI; phi += dPhi) {
                // position (x,y,z) in 3-dimensions
                double x = r * cos(theta) * sin(phi);
                double y = r * sin(theta) * sin(phi);
                double z = r * cos(phi);
                // camera x and y position
                double sy = (cameraDist * z) / (cameraDist + y - cameraWall);
                double sx = (cameraDist * x) / (cameraDist + y - cameraWall);
                // buffer y and x position
                int by = (int)((sy + 1) * 0.5 * rows + 0.5);
                int bx = (int)(0.5 * cols + sx * rows + 0.5);
                // normalized buffer values
                unsigned int bufferx = max(0, min(bx, cols - 1));
                unsigned int buffery = max(0, min(by, rows - 1));
                unsigned int bufferIndex = buffery * cols + bufferx;
                // find distance from camera to point squared
                double distCamera = x * x + (y - cameraWall + cameraDist) * (y - cameraWall + cameraDist) + z * z;
                if (distCamera < zbuffer[bufferIndex]) {
                    // find surface normals
                    double nx = x / r, ny = y / r, nz = z / r;
                    unsigned int light = (unsigned int)(((nx * lx + ny * ly + nz * lz) + 1) * 0.5 * (gradlen - 1) + 0.5);
                    light = max(0, min(light, gradlen - 1));
                    zbuffer[bufferIndex] = distCamera;
                    buffer[buffery * cols + bufferx] = gradient[light];
                }
            }
        }
        //  print buffer
        printf("\033[H");
        for (unsigned i = 0; i < pixels; i++) {
            putchar(buffer[i]);
        }
        fflush(stdout);
        sleep(0.1);
        // update light angle
        lightAngleTheta += 0.05;
        lightAnglePhi += 0.0;
    }

    // free buffer
    free(buffer);
    free(zbuffer);

    // re-enable cursor
    printf("\033[?25h");

    return 0;
}