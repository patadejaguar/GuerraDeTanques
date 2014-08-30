#include "soundengine.h"

SoundEngine::SoundEngine(QUrl sound, int max_distance_sound, QObject *parent) :
    QObject(parent)
{
    _media_player = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    _playlist = new QMediaPlaylist(this);

    _playlist->addMedia(QMediaContent(sound));
    _media_player->setPlaylist(_playlist);

    _max_sound_distance = max_distance_sound;
}

SoundEngine::~SoundEngine()
{
    delete _playlist;
    delete _media_player;
}

void SoundEngine::stopSound()
{
    _media_player->stop();
}

void SoundEngine::playSound2D(QPointF source, QPointF listener, double volume)
{
    if(volume <= 0)
        return;

    double v = volume/100.0;

    double distance = pow(pow(source.x()-listener.x(),2)+pow(source.y()-listener.y(),2),0.5);
    int effect = (_max_sound_distance - distance)*100/_max_sound_distance;
    if(effect > 0)
    {
        _media_player->setVolume(effect * v);
        _media_player->play();
    }
}

QMediaPlayer* SoundEngine::mediaPlayer() const
{
    return _media_player;
}
