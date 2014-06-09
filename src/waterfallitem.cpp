
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

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <cmath>
#include <cfloat>
#include <climits>
#include "waterfallitem.h"
#include "dft/radix2fft.h"

WaterfallItem::WaterfallItem(QQuickItem *parent)
    : QQuickPaintedItem(parent) {

    // Initialize connections
    QObject::connect(&_sampler, &AudioSampler::samplesCollected, this, &WaterfallItem::samplesCollected);
    QObject::connect(this, &QQuickItem::widthChanged, this, &WaterfallItem::sizeChanged);
    QObject::connect(this, &QQuickItem::heightChanged, this, &WaterfallItem::sizeChanged);

    // Initialize properties
    this->setVisible(true);
    this->setFlag(QQuickItem::ItemHasContents);
    _samplesUpdated = false;
    _sampleNumber = _sampler.samplesToWait();
    _image = QImage((int)this->width(), (int)this->height(), QImage::Format_ARGB32);
    _image.fill(QColor(255, 255, 255));
    _dft = new Radix2Fft(_sampleNumber);
    _sensitivity = 0.05;

    // Generate displayable colors
    QImage img(500, 1, QImage::Format_ARGB32);
    _colors.reserve(img.height());
    QPainter painter;
    painter.begin(&img);
    QLinearGradient gradient;
    gradient.setStart(0, 0);
    gradient.setFinalStop(img.width(), 0);
    gradient.setColorAt(0, QColor(255, 255, 255));
    gradient.setColorAt(0.2, QColor(0, 0, 255));
    gradient.setColorAt(0.4, QColor(255, 240, 0));
    gradient.setColorAt(0.6, QColor(255, 0, 0));
    gradient.setColorAt(1, QColor(0, 0, 0));
    gradient.setSpread(QGradient::PadSpread);
    painter.fillRect(QRect(0, 0, img.width(), 1), QBrush(gradient));
    painter.end();
    for (int i = 0; i < img.width(); i++) {
        QRgb rgb = img.pixel(i, 0);
        _colors.append(rgb);
    }

    // Repaint this item
    update();
}

void WaterfallItem::sizeChanged() {
    QImage img = QImage((int)this->width(), (int)this->height(), QImage::Format_ARGB32);
    img.fill(QColor(255, 255, 255));

    if (!_image.isNull()) {
        QPainter painter;
        painter.begin(&img);
        painter.drawImage(QRect(0, 0, width(), height()), _image, QRect(0, 0, _image.width(), _image.height()));
        painter.end();
    }

    _image = img;
    update();
}

void WaterfallItem::paint(QPainter *painter) {
    painter->drawImage(QRect(0, 0, width(), height()), _image, QRect(0, 0, _image.width(), _image.height()));
}

bool WaterfallItem::start() {
    bool result = _sampler.start();
    emit this->isStartedChanged();
    return result;
}

void WaterfallItem::stop() {
    _sampler.stop();
    emit this->isStartedChanged();
}

bool WaterfallItem::isStarted() const {
    return _sampler.isStarted();
}

void WaterfallItem::clear() {
    _image = QImage((int)this->width(), (int)this->height(), QImage::Format_ARGB32);
    _image.fill(QColor(255, 255, 255));
}

float WaterfallItem::minFrequency() const {
    return 0.0f;
}

float WaterfallItem::maxFrequency() const {
    return (float)(_sampler.samplingFrequency() / 10);
}

float WaterfallItem::sensitivity() const {
    return _sensitivity;
}

void WaterfallItem::setSensitivity(float value) {
    _sensitivity = value;
    emit this->sensitivityChanged();
}

void WaterfallItem::samplesCollected(std::vector<float> *samples) {

    // Use this to test just the DFT and visualize without audio:
    //auto s = Dft::generateSineSamples(4096, 100, 4000);
    //std::vector<std::complex<float> > result = _dft->compute(s);

    // Compute DFT
    std::vector<std::complex<float> > result = _dft->compute(*samples);
    delete samples;

    // Create new image
    QImage img((int)this->width(), (int)this->height(), QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(&img);

    // Draw 1st pixel row: new values
    for (int x = 0; x < img.width(); x++) {
        unsigned i1 = x * _sampleNumber / 10 / img.width();
        float amplitude = std::log10(std::abs(result[i1]));
        int value = (int)(amplitude * (float)_sensitivity * (float)_colors.length());
        if (value < 0)
            value = 0;
        if (value >= _colors.length())
            value = _colors.length() - 1;

        painter.setPen(_colors[value]);
        painter.drawRect(x, 0, 1, 5);
    }

    // Draw old values
    if (!_image.isNull()) {
        painter.drawImage(QRect(0, 6, width(), height()), _image, QRect(0, 0, _image.width(), _image.height()));
    }

    painter.end();

    // Redraw the item
    _image = img;
    update();
}
