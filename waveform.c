#include "waveform.h"
#include <math.h>

static double get_phase_value(const WaveformSample *s, int phase) {
    switch (phase) {
        case 0: return s->phase_A_voltage;
        case 1: return s->phase_B_voltage;
        default: return s->phase_C_voltage;
    }
}

double compute_rms(const WaveformSample *samples, size_t count, int phase) {
    if (count == 0) return 0.0;
    const WaveformSample *p = samples;
    long double sumsq = 0.0L;
    for (size_t i = 0; i < count; ++i, ++p) {
        double v = get_phase_value(p, phase);
        sumsq += (long double)v * (long double)v;
    }
    long double mean = sumsq / (long double)count;
    return (double)sqrt((double)mean);
}

double compute_peak_to_peak(const WaveformSample *samples, size_t count, int phase) {
    if (count == 0) return 0.0;
    const WaveformSample *p = samples;
    double max = get_phase_value(p, phase);
    double min = max;
    for (size_t i = 0; i < count; ++i, ++p) {
        double v = get_phase_value(p, phase);
        if (v > max) max = v;
        if (v < min) min = v;
    }
    return max - min;
}

double compute_dc_offset(const WaveformSample *samples, size_t count, int phase) {
    if (count == 0) return 0.0;
    const WaveformSample *p = samples;
    long double sum = 0.0L;
    for (size_t i = 0; i < count; ++i, ++p) {
        sum += get_phase_value(p, phase);
    }
    return (double)(sum / (long double)count);
}

int detect_clipping(const WaveformSample *samples, size_t count, int phase, double threshold) {
    const WaveformSample *p = samples;
    for (size_t i = 0; i < count; ++i, ++p) {
        double v = get_phase_value(p, phase);
        if (fabsl(v) >= threshold) return 1;
    }
    return 0;
}

int check_tolerance(double rms, double nominal, double tol_percent) {
    double low = nominal * (1.0 - tol_percent / 100.0);
    double high = nominal * (1.0 + tol_percent / 100.0);
    return (rms >= low && rms <= high) ? 1 : 0;
}
