#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <stddef.h>

typedef struct {
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} WaveformSample;

double compute_rms(const WaveformSample *samples, size_t count, int phase);
double compute_peak_to_peak(const WaveformSample *samples, size_t count, int phase);
double compute_dc_offset(const WaveformSample *samples, size_t count, int phase);
int detect_clipping(const WaveformSample *samples, size_t count, int phase, double threshold);
int check_tolerance(double rms, double nominal, double tol_percent);

#endif // WAVEFORM_H
