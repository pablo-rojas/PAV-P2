#include <stdio.h>
#include <stdint.h>
#include "fic_wave.h"

FILE *abre_wave(const char *ficWave, float *audioFormat, float *numChannels, float *fm, float *bps)
{
    FILE *fpWave;

    if ((fpWave = fopen(ficWave, "r")) == NULL)
        return NULL;
    if (fseek(fpWave, 44, SEEK_SET) < 0)
        return NULL;

    if (fseek(fpWave, 20, SEEK_SET) < 0)
        return NULL;
    int16_t aux;

    if (fread(&aux, 2, 1, fpWave) <= 0)
        return NULL;
    *audioFormat = aux;

    if (fread(&aux, 2, 1, fpWave) <= 0)
        return NULL;
    *numChannels = aux;

    int32_t aux2;
    if (fread(&aux2, 4, 1, fpWave) <= 0)
        return NULL;
    *fm = aux2;

    if (fseek(fpWave, 34, SEEK_SET) < 0)
        return NULL;
    if (fread(&aux, 2, 1, fpWave) <= 0)
        return NULL;
    *bps = aux;

    if (fseek(fpWave, 44, SEEK_SET) < 0)
        return NULL;

    return fpWave;
}

size_t lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave)
{
    return fread(x, size, nmemb, fpWave);
}

void cierra_wave(FILE *fpWave)
{
    fclose(fpWave);
}