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
};

#endif // AUDIOSAMPLER_H
