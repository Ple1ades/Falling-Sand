#include "Utilities.h"

namespace FLUID{
    void setBnd(int b, double *x, int width, int height) {
        for (int i = 1; i < width - 1; i++) {
            x[(i + 0 * width)] = b == 2 ? -x[(i + 1 * width)] : x[(i + 1 * width)];
            x[(i + (height - 1) * width)] = b == 2 ? -x[(i + (height - 2) * width)] : x[(i + (height - 2) * width)];
        }
        for (int j = 1; j < height - 1; j++) {
            x[(0 + j * width)] = b == 1 ? -x[(1 + j * width)] : x[(1 + j * width)];
            x[(width - 1 + j * width)] = b == 1 ? -x[(width - 2 + j *width)] : x[(width - 2+ j * width)];
        }

        x[(0 + 0)] = 0.5 * (x[(1 + 0)] + x[(0 + 1 * width)]);
        x[(0 + (height - 1) * width)] = 0.5 * (x[(1 + (height - 1) * width)] + x[(0 + (height - 2) * width)]);
        x[(width - 1 + 0 * width)] = 0.5 * (x[(width - 2 + 0 * width)] + x[(width - 1 + 1 * width)]);
        x[(width - 1 + (height - 1) * width)] = 0.5 * (x[(width - 2 + (height - 1) * width)] + x[(width - 1 + (height - 2) * width)]);
    }
    void linSolve(int b, double *x, double *x0, double a, double c, int iter, int width, int height) {
        double cRecip = 1.0 / c;
        for (int t = 0; t < iter; t++) {
            for (int j = 1; j < height - 1; j++) {
                for (int i = 1; i < width - 1; i++) {
                    x[(i + j * width)] =
                    (x0[(i + j * width)] +
                        a *
                        (x[(i + 1 + j * width)] +
                            x[(i - 1 + j * width)] +
                            x[(i + (j + 1) * width)] +
                            x[(i + (j - 1) * width)])) *
                    cRecip;
                }
            }
            setBnd(b, x, width, height);
        }
    }
    
    void addDensity(int x, int y, int width, double amount, double * density){
        density[x + y * width] += amount;
    }
    void addVelocity(int x, int y, int width, double _velX, double _velY, double *Vx, double *Vy){
        Vx[x + y * width] += _velX;
        Vy[x + y * width] += _velY;
    }
    void diffuse(int b, double *x, double*x0, double diff, double dt, int width, int height) {
        double a = dt * diff * (width - 2) * (height - 2);
        linSolve(b, x, x0, a, 1 + 6 * a, 4, width, height);
    }
    void advect(int b, double *d, double * d0, double *velocX, double *velocY, double dt, int width, int height) {
        int i0, i1, j0, j1;

        double dtx = dt * (width - 2);
        double dty = dt * (height - 2);

        double s0, s1, t0, t1;
        double tmp1, tmp2, tmp3, x, y;

        double NXfloat = width - 2;
        double NYfloat = height - 2;
        double ifloat, jfloat;
        int i, j, k;

        for (j = 1, jfloat = 1; j < height - 1; j++, jfloat++) {
            for (i = 1, ifloat = 1; i < width - 1; i++, ifloat++) {
            tmp1 = dtx * velocX[(i + j * width)];
            tmp2 = dty * velocY[(i + j * width)];
            x = ifloat - tmp1;
            y = jfloat - tmp2;

            if (x < 0.5) x = 0.5;
            if (x > NXfloat + 0.5) x = NXfloat + 0.5;
            i0 = std::floor(x);
            i1 = i0 + 1.0;
            if (y < 0.5) y = 0.5;
            if (y > NYfloat + 0.5) y = NYfloat + 0.5;
            j0 = std::floor(y);
            j1 = j0 + 1.0;

            s1 = x - i0;
            s0 = 1.0 - s1;
            t1 = y - j0;
            t0 = 1.0 - t1;

            int i0i = (i0);
            int i1i = (i1);
            int j0i = (j0);
            int j1i = (j1);

            d[(i + j * width)] =
                s0 * (t0 * d0[(i0i + j0i * width)] + t1 * d0[(i0i + j1i * width)]) +
                s1 * (t0 * d0[(i1i + j0i * width)] + t1 * d0[(i1i + j1i * width)]);
            }
        }

        setBnd(b, d, width, height);
    }
    void project(double * velocX, double *velocY, double *p, double *div, int width, int height) {
        for (int j = 1; j < height - 1; j++) {
            for (int i = 1; i < width - 1; i++) {
            div[(i + j * width)] =
                (-0.5 *
                (velocX[(i + 1 + j * width)] -
                    velocX[(i - 1 + j * width)] +
                    velocY[(i + (j + 1) * width)] -
                    velocY[(i + (j - 1) * width)])) /
                256;
            p[(i + j * width)] = 0;
            }
        }
    }
    void fadeDensity(double * density){
        for (int i = 0; i < sizeof(density); ++i){
            double d = density[i];
            density[i] = std::max(0.0, std::min(255.0, d - 0.02));
        }
    }
}
