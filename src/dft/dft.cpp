
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// This file is part of the frequency-analyzer application.
// It is licensed to you under the terms of the MIT license.
// http://opensource.org/licenses/MIT
//
// Copyright (c) 2014 Timur Kristóf

#include "dft.h"
#include <iostream>
#include <QElapsedTimer>
#include <QDebug>

float Dft::benchmark(Dft *reference, Dft *benchmarked, unsigned sampleCount) {
    std::vector<float> samples(sampleCount);
    for (unsigned i = 0; i < sampleCount; i++) {
        samples[i] = (float)qrand();
    }

    float referenceTime, benchmarkedTime;
    QElapsedTimer timer;

    timer.start();
    reference->compute(samples);
    referenceTime = (float)timer.elapsed();

    timer.start();
    benchmarked->compute(samples);
    benchmarkedTime = (float)timer.elapsed();

    timer.invalidate();
    return benchmarkedTime / referenceTime;
}

bool Dft::test(Dft *reference, Dft *impl, unsigned sampleCount) {
    std::vector<float> samples = generateSineSamples(sampleCount);

    bool ok = true;
    auto r1 = reference->compute(samples);
    auto r2 = impl->compute(samples);

    float diff, allowed;

    for (unsigned i = 0; i < sampleCount; i++) {
        diff = std::abs(std::abs(r1[i]) - std::abs(r2[i]));
        allowed = std::abs(r1[i]) * 0.1 + 0.01;
        if (diff > allowed) {
            std::cout << "problem with amplitude at index " << i << " ref: " << std::abs(r1[i]) << "; got: " << std::abs(r2[i]) << std::endl;
            ok = false;
        }

        diff = std::abs(std::arg(r1[i]) - std::arg(r2[i]));
        allowed = std::abs(std::arg(r1[i])) * 0.1 + 0.01;
        if (diff > allowed) {
            std::cout << "problem with phase at index " << i << " ref: " << std::arg(r1[i]) << "; got: " << std::arg(r2[i]) << std::endl;
            ok = false;
        }
    }

    return ok;
}

std::vector<float> Dft::generateSineSamples(unsigned n, float a, float f) {
    const float pi = std::acos(-1);
    float om = 2 * pi * f / n;
    std::vector<float> samples(n);
    for (unsigned i = 0; i < n; i++) {
        samples[i] = a * std::cos(om * i);
    }

    return samples;
}

TrivialDft::TrivialDft(unsigned sampleCount) : Dft(sampleCount) {
}

std::vector<std::complex<float> > TrivialDft::compute(const std::vector<float> &samples) {
    unsigned N = sampleCount();
    if (samples.size() < N) {
        std::cout << "sample count " << samples.size() << ", expected: " << N;
        throw std::exception();
    }

    const float pi = std::acos(-1);
    const std::complex<float> j(0, 1);
    const std::complex<float> aa = -j * 2.0f * pi / (float)N;

    std::vector<std::complex<float> > result(N);
    for (unsigned n = 0; n < N; n++) {
        std::complex<float> bb = aa * (float)n;

        for (unsigned k = 0; k < N; k++) {
            result[n] += samples[k] * std::exp(bb * (float)k);
        }
    }

    return result;
}


/*
 * Many implementations of the DFT are possible.
 * You can test an implementation with code like this:

#include <iostream>
#include <complex>
#include <cmath>
#include <vector>

#include "dft/dft.h"
#include "dft/radix2fft.h"

int main() {
    // Generate sine samples
    unsigned n = 1024;
    std::vector<float> samples = Dft::generateSineSamples(n);

    // Create object instances
    TrivialDft dft(n);
    Radix2Fft fft(n);
    std::vector<std::complex<float> > result;

    // Compute te result with trivial DFT
    std::cout << std::endl << "trivial DFT:" << std::endl;
    result = dft.compute(samples);
    for (unsigned i = 0; i < n; i++) {
        float amplitude = 2 * std::abs(result[i]) / n;
        if (amplitude > 10) {
            std::cout << "index: " << i << "; amplitude:" << amplitude << std::endl;
        }
    }

    // Compute the result with radix-2 FFT
    std::cout << std::endl << "radix 2 FFT:" << std::endl;
    result = fft.compute(samples);
    for (unsigned i = 0; i < n; i++) {
        float amplitude = 2 * std::abs(result[i]) / n;
        if (amplitude > 10) {
            std::cout << "index: " << i << "; amplitude:" << amplitude << std::endl;
        }
    }

    // Test an implementation numerically
    bool ok = Dft::test(&dft, &fft, n);
    std::cout << "tested implementation " << (ok ? "is ok" :  "sucks") << std::endl;

    // Benchmark an implementation (for execution time)
    float r = Dft::benchmark(&dft, &fft, n);
    std::cout << "tested implementation's execution time is " << r << "× of the reference" << std::endl;

    return 0;
}

 */





