#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <csv-file>\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];
    WaveformSample *samples = NULL;
    size_t count = 0;
    int ret = load_csv(filename, &samples, &count);
    if (ret != 0) {
        fprintf(stderr, "Failed to load '%s' (error %d)\n", filename, ret);
        return 2;
    }

    double rmsA = compute_rms(samples, count, 0);
    double rmsB = compute_rms(samples, count, 1);
    double rmsC = compute_rms(samples, count, 2);

    double p2pA = compute_peak_to_peak(samples, count, 0);
    double p2pB = compute_peak_to_peak(samples, count, 1);
    double p2pC = compute_peak_to_peak(samples, count, 2);

    double dcA = compute_dc_offset(samples, count, 0);
    double dcB = compute_dc_offset(samples, count, 1);
    double dcC = compute_dc_offset(samples, count, 2);

    const double CLIP_THRESHOLD = 324.9;
    int clipA = detect_clipping(samples, count, 0, CLIP_THRESHOLD);
    int clipB = detect_clipping(samples, count, 1, CLIP_THRESHOLD);
    int clipC = detect_clipping(samples, count, 2, CLIP_THRESHOLD);

    int compA = check_tolerance(rmsA, 230.0, 10.0);
    int compB = check_tolerance(rmsB, 230.0, 10.0);
    int compC = check_tolerance(rmsC, 230.0, 10.0);

    int w = write_report("results.txt", filename, samples, count,
                         rmsA, rmsB, rmsC,
                         p2pA, p2pB, p2pC,
                         dcA, dcB, dcC,
                         clipA, clipB, clipC,
                         compA, compB, compC);
    if (w != 0) {
        fprintf(stderr, "Failed to write results.txt\n");
    } else {
        fprintf(stdout, "Analysis complete. Results written to results.txt\n");
    }

    free(samples);
    return 0;
}
