
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
#include <QtCore/QMetaMethod>
#include <QtCore/QCoreApplication>
#include <QtCore/QtEndian>
#include "audiosampler.h"


AudioSampler::AudioSampler(QObject *parent)
    : QIODevice(parent) {

    _started = false;
    _samplesToWait = 4096;
    _samples = new std::vector<float>();
}

AudioSampler::~AudioSampler() {
    delete _samples;
}

void AudioSampler::elapsed() {
    if (this->isSignalConnected(QMetaMethod::fromSignal(&AudioSampler::samplesCollected))) {
        std::vector<float> *samples = new std::vector<float>(*_samples);
        emit this->samplesCollected(samples);
    }

    _samples->clear();
}

bool AudioSampler::start() {
    if (_started)
        return true;

    // audio format

    _format.setSampleRate(samplingFrequency());
    _format.setChannelCount(1);
    _format.setSampleSize(16);
    _format.setSampleType(QAudioFormat::SignedInt);
    _format.setByteOrder(QAudioFormat::LittleEndian);
    _format.setCodec("audio/pcm");

    // audio device

    _device = QAudioDeviceInfo::defaultInputDevice();
    if (_device.isNull()) {
        qDebug() << "default audio device is null, maybe there isn't one on this computer?";
        return false;
    }
    if (!_device.isFormatSupported(_format)) {
        qWarning() << "requested format not supported";
        return false;

        // TODO:
        //_format = _device.nearestFormat(_format);
    }
    qDebug() << "using default recording device:" << _device.deviceName();

    // TODO: handle device hotplugging
    _state = QAudio::IdleState;
    _input = new QAudioInput(_device, _format, this);
    QObject::connect(_input, &QAudioInput::stateChanged, this, &AudioSampler::audioInputStateChanged);
    this->open(QIODevice::WriteOnly);
    _input->start(this);

    return _started = true;
}

bool AudioSampler::isStarted() const {
    return _started;
}

void AudioSampler::stop() {
    if (!_started)
        return;

    _input->stop();
    _input->deleteLater();
    _samples->clear();
    this->close();

    _started = false;
}

quint32 AudioSampler::samplingFrequency() const {
    return 44100;
}

quint32 AudioSampler::samplesToWait() const {
    return _samplesToWait;
}

void AudioSampler::setSamplesToWait(quint32 value) {
    _samplesToWait = value;
}

qint64 AudioSampler::readData(char *data, qint64 maxlen) {
    // NOTE: you are not supposed to read from this object
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    return 0;
}

qint64 AudioSampler::writeData(const char *data, qint64 len) {
    if (!_started)
        return len;
    if (_state != QAudio::ActiveState)
        return len;

    // Assuming little endian, 1-channel, 16-bit unsigned integer samples
    Q_ASSERT(len % 2 == 0);
    const std::int16_t *samples = reinterpret_cast<const std::int16_t*>(data);

    for (qint64 i = 0; i < len / 2; i++) {
        std::int16_t sample = samples[i];
        _samples->push_back((float)sample);

        if (_samples->size() >= samplesToWait()) {
            elapsed();
        }
    }

    return len;
}

void AudioSampler::audioInputStateChanged(QAudio::State state) {
    _state = state;
}
