
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

#ifndef AUDIOSAMPLER_H
#define AUDIOSAMPLER_H

#include <vector>
#include <QtCore/QScopedPointer>
#include <QtCore/QIODevice>
#include <QtCore/QTimer>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioBuffer>


class AudioSampler : public QIODevice
{
private:
    Q_OBJECT
    QAudioFormat _format;
    QAudioDeviceInfo _device;
    QAudioInput *_input;
    std::vector<float> *_samples;
    bool _started;
    QAudio::State _state;
    quint32 _samplesToWait;

public:
    explicit AudioSampler(QObject *parent = 0);
    ~AudioSampler();

    bool start();
    bool isStarted() const;
    void stop();
    quint32 samplingFrequency() const;
    quint32 samplesToWait() const;
    void setSamplesToWait(quint32 samples);

protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

signals:
    void samplesCollected(std::vector<float>*);

private slots:
    void elapsed();
    void audioInputStateChanged(QAudio::State state);
};

#endif // AUDIOSAMPLER_H
