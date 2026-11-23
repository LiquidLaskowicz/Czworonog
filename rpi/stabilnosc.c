#include <math.h>
#include <stddef.h>
#include "stabilnosc.h"

#define ROZSTAW_BIODER_L 0.345
#define ROZSTAW_BIODER_W 0.273
#define MARGINES_MIN     0.010

static double strona(const double a[2], const double b[2], const double p[2])
{
    return (b[0] - a[0]) * (p[1] - a[1])
         - (b[1] - a[1]) * (p[0] - a[0]);
}

double margines_stabilnosci(const double stopy[4][2], const int w_podparciu[4], const double cog[2])
{
    static const double srodek[2] = {0.0, 0.0};
    if (cog == NULL) cog = srodek;
    if (!isfinite(cog[0]) || !isfinite(cog[1])) return NAN;

    const double *p[4];
    int n = 0;

    for (int i = 0; i < 4; ++i) {
        if (!w_podparciu[i]) continue;
        if (!isfinite(stopy[i][0]) || !isfinite(stopy[i][1])) return NAN;
        p[n++] = stopy[i];
    }

    double minimum = INFINITY;
    int poza = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            int lewo = 0;
            int prawo = 0;

            for (int k = 0; k < n; ++k)
            {
                if (k == i || k == j) continue;

                double s = strona(p[i], p[j], p[k]);
                if (s > 0) lewo = 1;
                if (s < 0) prawo = 1;
            }

            if (lewo == prawo) continue;

            double s = strona(p[i], p[j], cog);
            if ((lewo && s < 0) || (prawo && s > 0)) poza = 1;

            double dx = p[j][0] - p[i][0];
            double dy = p[j][1] - p[i][1];
            double x = cog[0] - p[i][0];
            double y = cog[1] - p[i][1];

            double t = (x * dx + y * dy) / (dx * dx + dy * dy);
            t = fmax(0.0, fmin(1.0, t));
            minimum = fmin(minimum, hypot(x - t * dx, y - t * dy));
        }
    }

    if (!isfinite(minimum)) return NAN;
    return poza ? -minimum : minimum;
}

int mozna_podniesc_noge(const double stopy[4][2], const int w_podparciu[4], const double cog[2], int noga)
{
    if (noga < 0 || noga >= 4 || !w_podparciu[noga]) return 0;

    int po[4];
    for (int i = 0; i < 4; ++i) {
        po[i] = (i != noga) && w_podparciu[i];
    }

    return margines_stabilnosci(stopy, po, cog) >= MARGINES_MIN;
}