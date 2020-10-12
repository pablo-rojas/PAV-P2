#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "pav_analysis.h"
#include "fic_wave.h"

int main(int argc, char *argv[])
{
    float durTrm = 0.010;
    float fm;
    float audioFormat;
    int numChannels;
    float aux;
    float bps;
    int N;
    int trm;
    float *x;
    short *buffer;
    FILE *fpWave, *file;

    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "Empleo: %s inputfile [outputfile]\n", argv[0]);
        return -1;
    }

    if ((fpWave = abre_wave(argv[1], &audioFormat, &aux, &fm, &bps)) == NULL)
    {
        fprintf(stderr, "Error al abrir %s (%s)\n", argv[1], strerror(errno));
        return -1;
    }
    numChannels = aux;

    printf("audioFormat=%f\n numChannels=%i\n fm=%f\n bps=%f\n", audioFormat, numChannels, fm, bps);

    if (audioFormat != 1)
    {
        printf("Error con el formato del audio. Este debe ser del tipo PCM lineal.\n");
        return -1;
    }

    N = durTrm * fm;
    if ((buffer = malloc(N * numChannels * sizeof(*buffer))) == 0 ||
        (x = malloc(N * sizeof(*x))) == 0)
    {
        fprintf(stderr, "Error al ubicar los vectores (%s)\n", strerror(errno));
        return -1;
    }

    trm = 0;
    if (argc == 3)
    {
        file = fopen(argv[2], "w");
    }

    while (lee_wave(buffer, sizeof(*buffer), (N * numChannels), fpWave) == N*numChannels)
    {
        for (int n = 0; n < N; n++)
        {
            x[n] = 0.0;
            for (int i = 0; i < numChannels; i++)
            {
                x[n] += (buffer[n*numChannels+i] / (float)(1 << 15))/numChannels;
            }
        }
        if (argc == 2)
        {
            printf("%d\t%f\t%f\t%f\n", trm, compute_power(x, N), compute_am(x, N), compute_zcr(x, N, fm));
        }
        else if (argc == 3)
        {
            fprintf(file, "%d\t%f\t%f\t%f\n", trm, compute_power(x, N), compute_am(x, N), compute_zcr(x, N, fm));
        }

        trm += 1;
    }

    if (argc == 3)
    {
        fclose(file);
    }

    cierra_wave(fpWave);
    free(buffer);
    free(x);

    return 0;
}