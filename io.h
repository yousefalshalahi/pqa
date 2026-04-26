#ifndef IO_H
#define IO_H

#include <stddef.h>
#include "waveform.h"

int load_csv(const char *filename, WaveformSample **samples_out, size_t *count_out);

int write_report(const char *outpath, const char *input_filename,
                 const WaveformSample *samples, size_t count,
                 double rmsA, double rmsB, double rmsC,
                 double p2pA, double p2pB, double p2pC,
                 double dcA, double dcB, double dcC,
                 int clippingA, int clippingB, int clippingC,
                 int complianceA, int complianceB, int complianceC);

#endif // IO_H
