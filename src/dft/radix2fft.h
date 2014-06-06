#ifndef RADIX2FFT_H
#define RADIX2FFT_H

#include "dft.h"

class Radix2Fft final : public Dft {
private:
    unsigned _log2sc;
    std::vector<unsigned> _indices;

public:
    explicit Radix2Fft(unsigned sampleCount);

    std::vector<std::complex<float> > compute(const std::vector<float> &samples);
};

#endif // RADIX2FFT_H
