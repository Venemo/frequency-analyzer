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
