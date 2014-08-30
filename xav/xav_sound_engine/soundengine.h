#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QObject>
#include <QPointF>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "math.h"
#include "xav_sound_engine_global.h"

class XAV_SOUND_ENGINESHARED_EXPORT SoundEngine : public QObject
{
        Q_OBJECT

    public:

        explicit SoundEngine(QUrl sound, int max_distance_sound, QObject *parent = 0);
        ~SoundEngine();

        QMediaPlayer *      mediaPlayer() const;

    public slots:

        void                stopSound();

        void                playSound2D(QPointF origin, QPointF listener, double volume);

    private:

        QMediaPlayer*       _media_player;
        QMediaPlaylist*     _playlist;
        int                 _max_sound_distance;
};

#endif // SOUNDENGINE_H
