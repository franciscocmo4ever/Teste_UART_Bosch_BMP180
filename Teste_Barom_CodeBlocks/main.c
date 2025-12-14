#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main(void)
{
    // ===== Coeficientes do teu BMP180 DUMP =====
    int32_t  AC1 = 408;
    int32_t  AC2 = -72;
    int32_t  AC3 = -14383;
    uint32_t AC4 = 32741;
    int32_t  AC5 = 32757;
    int32_t  AC6 = 23153;
    int32_t  B1  = 6190;
    int32_t  B2  = 4;
    int32_t  MC  = -8711;
    int32_t  MD  = 2868;

    // ===== Leituras brutas do teu DUMP =====
    int32_t UT  = 31487;
    int32_t UP  = 32945;
    int32_t OSS = 0;

    // =========================
    // TEMPERATURA (datasheet)
    // =========================
    int32_t X1 = ((UT - AC6) * AC5) >> 15;
    int32_t X2 = (int32_t)floor(((double)MC * 2048.0) / (double)(X1 + MD));
    int32_t B5 = X1 + X2;
    int32_t T  = (B5 + 8) >> 4;  // 0.1°C

    printf("X1 = %ld\n", (long)X1);
    printf("X2 = %ld\n", (long)X2);
    printf("B5 = %ld\n", (long)B5);
    printf("T  = %ld (0.1C) => %.1f C\n\n", (long)T, T / 10.0);

    // =========================
    // PRESSAO (datasheet)
    // =========================
    int32_t B6 = B5 - 4000;

    int32_t X1p = (B2 * ((B6 * B6) >> 12)) >> 11;
    int32_t X2p = (AC2 * B6) >> 11;
    int32_t X3  = X1p + X2p;

    int32_t B3 = (((AC1 * 4 + X3) << OSS) + 2) >> 2;

    X1p = (AC3 * B6) >> 13;
    X2p = (B1 * ((B6 * B6) >> 12)) >> 16;
    X3  = ((X1p + X2p) + 2) >> 2;

    uint32_t B4 = (AC4 * (uint32_t)(X3 + 32768)) >> 15;

    // Proteção contra divisão por zero
    if (B4 == 0) {
        printf("ERRO: B4=0 (divisao por zero). Confira AC4/coeficientes/leituras.\n");
        return 0;
    }

    uint32_t B7 = (uint32_t)(UP - B3) * (uint32_t)(50000U >> OSS);

    int32_t P;
    if (B7 < 0x80000000U) P = (int32_t)((B7 * 2U) / B4);
    else                  P = (int32_t)((B7 / B4) * 2U);

    X1p = (P >> 8) * (P >> 8);
    X1p = (X1p * 3038) >> 16;
    X2p = (-7357 * P) >> 16;

    P = P + ((X1p + X2p + 3791) >> 4); // Pa

    printf("P  = %ld Pa => %.2f hPa\n", (long)P, P / 100.0);

    return 0;
}
