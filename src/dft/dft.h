#ifndef DFT_H
#define DFT_H

#include <complex>
#include <vector>

class Dft {
private:
    unsigned _sampleCount;
    double _samplingFrequency;

public:
    explicit inline Dft(unsigned sampleCount) : _sampleCount(sampleCount) { }
    virtual std::vector<std::complex<float> > compute(const std::vector<float> &samples) = 0;

    inline unsigned sampleCount() {
        return _sampleCount;
    }

    static float benchmark(Dft *reference, Dft *benchmarked, unsigned sampleCount = 4096);

    static bool test(Dft *reference, Dft *impl, unsigned sampleCount = 4096);

    static std::vector<float> generateSineSamples(unsigned n = 1024, float amplitude = 100, float f = 1);
};

class TrivialDft final : public Dft {
public:
    explicit TrivialDft(unsigned sampleCount);

    std::vector<std::complex<float> > compute(const std::vector<float> &samples);
};

#endif // DFT_H
