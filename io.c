#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUF 512

static size_t count_data_lines(FILE *f) {
    size_t count = 0;
    char buf[LINE_BUF];
    while (fgets(buf, sizeof(buf), f)) {
        count++;
    }
    return count;
}

int load_csv(const char *filename, WaveformSample **samples_out, size_t *count_out) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;
    char line[LINE_BUF];

    if (!fgets(line, sizeof(line), f)) { fclose(f); return -2; }

    size_t data_lines = count_data_lines(f);
    if (data_lines == 0) { fclose(f); *samples_out = NULL; *count_out = 0; return 0; }

    rewind(f);
    // skip header
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -2; }

    WaveformSample *samples = (WaveformSample*)malloc(sizeof(WaveformSample) * data_lines);
    if (!samples) { fclose(f); return -3; }

    size_t idx = 0;
    while (fgets(line, sizeof(line), f)) {
        char *tok = NULL;
        char *saveptr = NULL;
        tok = strtok_s(line, ",", &saveptr);
        if (!tok) break;
        samples[idx].timestamp = strtod(tok, NULL);
        tok = strtok_s(NULL, ",", &saveptr);
        samples[idx].phase_A_voltage = tok ? strtod(tok, NULL) : 0.0;
        tok = strtok_s(NULL, ",", &saveptr);
        samples[idx].phase_B_voltage = tok ? strtod(tok, NULL) : 0.0;
        tok = strtok_s(NULL, ",", &saveptr);
        samples[idx].phase_C_voltage = tok ? strtod(tok, NULL) : 0.0;
        tok = strtok_s(NULL, ",", &saveptr);
        samples[idx].line_current = tok ? strtod(tok, NULL) : 0.0;
        tok = strtok_s(NULL, ",", &saveptr);
        samples[idx].frequency = tok ? strtod(tok, NULL) : 0.0;
        tok = strtok_s(NULL, ",", &saveptr);
        samples[idx].power_factor = tok ? strtod(tok, NULL) : 0.0;
        tok = strtok_s(NULL, ",\r\n", &saveptr);
        samples[idx].thd_percent = tok ? strtod(tok, NULL) : 0.0;

        idx++;
        if (idx >= data_lines) break;
    }

    fclose(f);
    *samples_out = samples;
    *count_out = idx;
    return 0;
}

int write_report(const char *outpath, const char *input_filename,
                 const WaveformSample *samples, size_t count,
                 double rmsA, double rmsB, double rmsC,
                 double p2pA, double p2pB, double p2pC,
                 double dcA, double dcB, double dcC,
                 int clippingA, int clippingB, int clippingC,
                 int complianceA, int complianceB, int complianceC) {
    FILE *f = fopen(outpath, "w");
    if (!f) return -1;
    fprintf(f, "Input file: %s\n", input_filename);
    fprintf(f, "Samples read: %zu\n\n", count);

    fprintf(f, "=== Phase A ===\n");
    fprintf(f, "RMS: %.3f V\n", rmsA);
    fprintf(f, "Peak-to-peak: %.3f V\n", p2pA);
    fprintf(f, "DC offset: %.6f V\n", dcA);
    fprintf(f, "Clipping: %s\n", clippingA ? "YES" : "NO");
    fprintf(f, "In tolerance (±10%%): %s\n\n", complianceA ? "YES" : "NO");

    fprintf(f, "=== Phase B ===\n");
    fprintf(f, "RMS: %.3f V\n", rmsB);
    fprintf(f, "Peak-to-peak: %.3f V\n", p2pB);
    fprintf(f, "DC offset: %.6f V\n", dcB);
    fprintf(f, "Clipping: %s\n", clippingB ? "YES" : "NO");
    fprintf(f, "In tolerance (±10%%): %s\n\n", complianceB ? "YES" : "NO");

    fprintf(f, "=== Phase C ===\n");
    fprintf(f, "RMS: %.3f V\n", rmsC);
    fprintf(f, "Peak-to-peak: %.3f V\n", p2pC);
    fprintf(f, "DC offset: %.6f V\n", dcC);
    fprintf(f, "Clipping: %s\n", clippingC ? "YES" : "NO");
    fprintf(f, "In tolerance (±10%%): %s\n\n", complianceC ? "YES" : "NO");

    fclose(f);
    return 0;
}
