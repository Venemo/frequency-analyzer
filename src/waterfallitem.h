
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

#ifndef WATERFALLITEM_H
#define WATERFALLITEM_H

#include <vector>
#include <QQuickPaintedItem>
#include <QImage>

#include "dft/dft.h"
#include "audiosampler.h"

class WaterfallItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool isStarted READ isStarted NOTIFY isStartedChanged)
    Q_PROPERTY(float minFrequency READ minFrequency NOTIFY minFrequencyChanged)
    Q_PROPERTY(float maxFrequency READ maxFrequency NOTIFY maxFrequencyChanged)
    Q_PROPERTY(float sensitivity READ sensitivity WRITE setSensitivity NOTIFY sensitivityChanged)

    QImage _image;
    QList<QRgb> _colors;
    AudioSampler _sampler;
    Dft *_dft;
    bool _samplesUpdated;
    unsigned _sampleNumber;
    double _sensitivity;


public:
    explicit WaterfallItem(QQuickItem *parent = 0);
    void paint(QPainter *painter);
    bool isStarted() const;
    float minFrequency() const;
    float maxFrequency() const;
    float sensitivity() const;
    void setSensitivity(float value);

    Q_INVOKABLE bool start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void clear();

private slots:
    void samplesCollected(std::vector<float> *samples);
    void sizeChanged();

signals:
    void isStartedChanged();
    void minFrequencyChanged();
    void maxFrequencyChanged();
    void sensitivityChanged();

};

#endif // WATERFALLITEM_H
