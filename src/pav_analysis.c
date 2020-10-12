#include <math.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N)
{
    float p = 0;

    for (int n = 0; n < N; n++)
    {
        p += pow(x[n], 2);
    }
    p = 10 * log10(p / N);

    return p;
}

float compute_power_hamming(const float *x, unsigned int N)
{
    float p = 0;
    float w = 0;

    for (int n = 0; n < N; n++)
    {
        p += pow(x[n] * (0.53836 - 0.46164 * cos(2 * M_PI * n / (N - 1))), 2);
        w = pow((0.53836 - 0.46164 * cos(2 * M_PI * n / (N - 1))), 2);
    }
    p = 10 * log10(p / (N * w));

    return p;
}

float compute_am(const float *x, unsigned int N)
{
    float A = 0;

    for (int n = 0; n < N; n++)
    {
        A += fabs(x[n]);
    }
    A = A / N;

    return A;
}

float compute_zcr(const float *x, unsigned int N, float fm)
{
    float ZCR = 0;

    for (int n = 1; n < N; n++)
    {
        if (x[n] * x[n - 1] < 0)
        {
            ZCR++;
        }
    }
    ZCR = fm / (2 * (N - 1)) * ZCR;

    return ZCR;
}
