
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
