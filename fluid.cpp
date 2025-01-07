#include "Utilities.h"

class FLUID
{
public:
    double *D;
    double *Dt;

    double *Vx;
    double *Vy;
    double *Vtx;
    double *Vty;

    double *Ax;
    double *Ay;
    double *Atx;
    double *Aty;

    double *div;
    double *p;

    FLUID(int width, int height)
    {
        D = (double *)malloc((width * height + 2) * sizeof(double));
        Vx = (double *)malloc((width * height + 2) * sizeof(double));
        Vy = (double *)malloc((width * height + 2) * sizeof(double));
        Ax = (double *)malloc((width * height + 2) * sizeof(double));
        Ay = (double *)malloc((width * height + 2) * sizeof(double));

        Dt = (double *)malloc((width * height + 2) * sizeof(double));
        Vtx = (double *)malloc((width * height + 2) * sizeof(double));
        Vty = (double *)malloc((width * height + 2) * sizeof(double));
        Atx = (double *)malloc((width * height + 2) * sizeof(double));
        Aty = (double *)malloc((width * height + 2) * sizeof(double));

        div = (double *)malloc((width * height + 2) * sizeof(double));
        p = (double *)malloc((width * height + 2) * sizeof(double));

        for (int i = 0; i < (width * height + 2); ++i)
        {
            D[i] = 0;
            Vx[i] = 0;
            Vy[i] = 0;
            Dt[i] = 0;
            Vtx[i] = 0;
            Vty[i] = 0;
            div[i] = 0;
            p[i] = 0;
        }
    }
    static void diffuse(int w, int h, double *D, double *Dt, double m, double r)
    {

        int c = 0;
        double r0 = 1 - 4 * r;
        double r1 = 1 - 3 * r;
        double r2 = 1 - 2 * r;

        for (int x_0 = 1; x_0 < w - 1; ++x_0)
        {
            Dt[x_0] = std::max(-m, std::min(D[x_0] * (r1) + (D[x_0 - 1] + D[x_0 + 1] + D[x_0 + w]) * r, m));
            Dt[x_0 + (h - 1) * w] = std::max(-m, std::min(D[x_0 + (h - 1) * w] * (r1) + (D[x_0 + (h - 1) * w - 1] + D[x_0 + (h - 1) * w + 1] + D[x_0 + (h - 1) * w - w]) * r, m));
            for (int y_0 = 1; y_0 < h - 1; ++y_0)
            {
                c = (x_0) + y_0 * w;
                Dt[c] = std::max(-m, std::min(D[c] * (r0) + (D[c - 1] + D[c + 1] + D[c + w] + D[c - w]) * r, m));
            }
        }
        for (int y_0 = 1; y_0 < h - 1; ++y_0)
        {
            c = y_0 * w;
            Dt[c] = std::max(-m, std::min(D[c] * (r1) + (D[c - w] + D[c + w] + D[c + 1]) * r, m));
            c = y_0 * w + w - 1;
            Dt[c] = std::max(-m, std::min(D[c] * (r1) + (D[c - w] + D[c + w] + D[c - 1]) * r, m));
        }
        for (int i = 0; i < w * h; ++i)
        {
            D[i] = 0;
        }
        // Dt[0] = std::max(-m, std::min(D[0] * (r2) + (D[w] + D[1]) * r, m));
        // Dt[w - 1] = std::max(-m, std::min(D[w - 1] * (r2) + (D[2 * w - 1] + D[w - 2]) * r, m));
        // Dt[(w - 1) * h] = std::max(-m, std::min(D[(w - 1) * h] * (r2) + (D[(w - 2) * h] + D[(w - 1) * h + 1]) * r, m));
        // Dt[w * h - 1] = std::max(-m, std::min(D[w * h - 1] * (r2) + (D[(w - 1) * h - 1] + D[(w * h - 2)]) * r, m));

        // memmove(D, Dt, sizeof(double) * w * h);
    }
    static void vDiffuse(int w, int h, double *V, double *Vt, double m, double r, double *D)
    {

        int c = 0;
        double r0 = 1 - 4 * r;
        double r1 = 1 - 3 * r;
        double r2 = 1 - 2 * r;

        for (int x_0 = 1; x_0 < w - 1; ++x_0)
        {
            Vt[x_0] = std::max(-m, std::min(V[x_0] * (r1) + (V[x_0 - 1] + V[x_0 + 1] + V[x_0 + w]) * r, m));
            Vt[x_0 + (h - 1) * w] = std::max(-m, std::min(V[x_0 + (h - 1) * w] * (r1) + (V[x_0 + (h - 1) * w - 1] + V[x_0 + (h - 1) * w + 1] + V[x_0 + (h - 1) * w - w]) * r, m));
            for (int y_0 = 1; y_0 < h - 1; ++y_0)
            {
                c = (x_0) + y_0 * w;
                Vt[c] = std::max(-m, std::min(V[c] * (r0) + (V[c - 1] + V[c + 1] + V[c + w] + V[c - w]) * r, m));
            }
        }
        for (int y_0 = 1; y_0 < h - 1; ++y_0)
        {
            c = y_0 * w;
            Vt[c] = std::max(-m, std::min(V[c] * (r1) + (V[c - w] + V[c + w] + V[c + 1]) * r, m));
            c = y_0 * w + w - 1;
            Vt[c] = std::max(-m, std::min(V[c] * (r1) + (V[c - w] + V[c + w] + V[c - 1]) * r, m));
        }
        for (int i = 0; i < w * h; ++i)
        {
            if (D[i] < 10)
            {
                Vt[i] /= 3;
                V[i] /= 3;
            }
            else
            {

                // V[i] = Vt[i];
            }
        }
        // Dt[0] = std::max(-m, std::min(D[0] * (r2) + (D[w] + D[1]) * r, m));
        // Dt[w - 1] = std::max(-m, std::min(D[w - 1] * (r2) + (D[2 * w - 1] + D[w - 2]) * r, m));
        // Dt[(w - 1) * h] = std::max(-m, std::min(D[(w - 1) * h] * (r2) + (D[(w - 2) * h] + D[(w - 1) * h + 1]) * r, m));
        // Dt[w * h - 1] = std::max(-m, std::min(D[w * h - 1] * (r2) + (D[(w - 1) * h - 1] + D[(w * h - 2)]) * r, m));

        // memmove(D, Dt, sizeof(double) * w * h);
    }
    static void advect(int w, int h, double *Vx, double *Vy, double *D, double *Dt, double r)
    {
        int c = 0;
        double x = 0.0;
        double y = 0.0;
        int i0, i1, j0, j1;
        double s0, s1, t0, t1;
        double r1 = 1 - r;

        for (int x_0 = 1; x_0 < w - 1; ++x_0)
        {
            D[x_0] += Dt[x_0];
            D[x_0 + (h - 1) * w] += Dt[x_0 + (h - 1) * w];
            for (int y_0 = 1; y_0 < h - 1; ++y_0)
            {

                c = (x_0) + y_0 * w;

                x = x_0 + Vx[c];
                y = y_0 + Vy[c];
                if (x < 0.5)
                    x = 0.5;
                if (x > 0.5 + w - 1)
                    x = 0.5 + w - 1;
                if (y < 0.5)
                    y = 0.5;
                if (y > 0.5 + h - 1)
                    y = 0.5 + h - 1;
                i0 = floor(x);
                i1 = i0 + 1;
                j0 = floor(y);
                j1 = j0 + 1;
                s1 = x - i0;
                s0 = 1 - s1;
                t1 = y - j0;
                t0 = 1 - t1;
                D[i0 + j0 * w] += s0 * t0 * r * Dt[c];
                D[i0 + j1 * w] += s0 * t1 * r * Dt[c];
                D[i1 + j0 * w] += s1 * t0 * r * Dt[c];
                D[i1 + j1 * w] += s1 * t1 * r * Dt[c];
                // Dt[c] *= r1;
                D[c] += Dt[c] * r1;
                // D[c] = s0 * (t0 * Dt[i0 + j0 * w] + t1 * Dt[i0 + j1 * w]) + s1 * (t0 * Dt[i1 + j0 * w] + t1 * Dt[i1 + j1 * w]);
            }
        }
        for (int y_0 = 1; y_0 < h - 1; ++y_0)
        {
            c = y_0 * w;
            D[c] += Dt[c];
            c = y_0 * w + w - 1;
            D[c] += Dt[c];
        }
        D[0] += Dt[0];
        D[w - 1] += Dt[w - 1];
        D[w * (h - 1)] += Dt[w * (h - 1)];
        D[w * h - 1] += Dt[w * h - 1];
    }

    static void vAdvect(int w, int h, double *Vx, double *Vy, double *D, double *Dt, double r, bool x_)
    {
        int c = 0;
        double x = 0.0;
        double y = 0.0;
        int i0, i1, j0, j1;
        double s0, s1, t0, t1;
        double r1 = 1 - r;

        for (int x_0 = 1; x_0 < w - 1; ++x_0)
        {
            if (x_)
            {
                D[x_0] += Dt[x_0] * r;
                D[x_0 + (h - 1) * w] += Dt[x_0 + (h - 1) * w] * r;
            }
            else
            {
                D[x_0] = 0;
                D[x_0 + (h - 1) * w] = 0;
            }
            for (int y_0 = 1; y_0 < h - 1; ++y_0)
            {

                c = (x_0) + y_0 * w;

                x = x_0 + Vx[c];
                y = y_0 + Vy[c];
                if (x < 0.5)
                    x = 0.5;
                if (x > 0.5 + w - 1)
                    x = 0.5 + w - 1;
                if (y < 0.5)
                    y = 0.5;
                if (y > 0.5 + h - 1)
                    y = 0.5 + h - 1;
                i0 = floor(x);
                i1 = i0 + 1;
                j0 = floor(y);
                j1 = j0 + 1;
                s1 = x - i0;
                s0 = 1 - s1;
                t1 = y - j0;
                t0 = 1 - t1;
                D[i0 + j0 * w] += s0 * t0 * r * Dt[c];
                D[i0 + j1 * w] += s0 * t1 * r * Dt[c];
                D[i1 + j0 * w] += s1 * t0 * r * Dt[c];
                D[i1 + j1 * w] += s1 * t1 * r * Dt[c];
                // Dt[c] *= r1;
                D[c] += Dt[c] * r1;
                // D[c] = s0 * (t0 * Dt[i0 + j0 * w] + t1 * Dt[i0 + j1 * w]) + s1 * (t0 * Dt[i1 + j0 * w] + t1 * Dt[i1 + j1 * w]);
            }
        }
        if (!x_)
        {
            for (int y_0 = 1; y_0 < h - 1; ++y_0)
            {
                c = y_0 * w;
                D[c] += Dt[c] * r;
                c = y_0 * w + w - 1;
                D[c] += Dt[c] * r;
            }
        }
        else
        {
            for (int y_0 = 1; y_0 < h - 1; ++y_0)
            {
                c = y_0 * w;
                D[c] = 0;
                c = y_0 * w + w - 1;
                D[c] = 0;
            }
        }

        D[0] = 0;
        D[w - 1] = 0;
        D[w * (h - 1)] = 0;
        D[w * h - 1] = 0;
    }
    static void project(double *Vx, double *Vy, double *div, double *p, int w, int h)
    {
        int c = 0;
        for (int x = 1; x < w - 1; ++x)
        {
            for (int y = 1; y < h - 1; ++y)
            {
                c = x + y * w;
                div[c] = -0.5f * (Vx[c + 1] - Vx[c - 1] + Vy[c + w] - Vy[c - w]);
            }
        }
        diffuse(w, h, div, p, 0, 0.2);
        for (int x = 1; x < w - 1; ++x)
        {
            for (int y = 1; y < h - 1; ++y)
            {
                c = x + y * w;
                Vx[c] -= 0.5 * (p[c + 1] - p[c - 1]) * w;
                Vy[c] -= 0.5 * (p[c + w] - p[c - w]) * h;
            }
        }
    }
    static void updateV(std::vector<line> *lines, double *Vx, double *Vy, int w, int h, int s, double m)
    {

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int c = x + y * w;
                (*lines)[c].x1 = x * s + s / 2;
                (*lines)[c].y1 = y * s + s / 2;
                (*lines)[c].x2 = x * s - (int)(std::min(m, std::max(-m, Vx[c])) * s) + s / 2;
                (*lines)[c].y2 = y * s - (int)(std::min(m, std::max(-m, Vy[c])) * s) + s / 2;
            }
        }
    }
};
