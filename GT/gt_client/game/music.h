#ifndef MUSIC_H
#define MUSIC_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "configuration.h"

class Music : public QObject, public ConfigurationObserver
{
        Q_OBJECT

    public:

        explicit Music(QObject *parent = 0);

        bool                    setMedia(QUrl media);

        void                    play();

        void                    stop();

        QMediaPlayer::State     state() const;

        void                    update();

    private:

        QMediaPlayer*           _media_player;

        QMediaPlaylist*         _media_play_list;
};

#endif // MUSIC_H
