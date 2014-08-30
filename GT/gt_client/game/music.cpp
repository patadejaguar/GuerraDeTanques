#include "music.h"

Music::Music(QObject *parent) :
    QObject(parent),
    ConfigurationObserver(QList<ConfigurationObserverType>()<<COT_SOUND)
{
    _media_player = new QMediaPlayer(this);
    _media_play_list = new QMediaPlaylist(this);
    _media_play_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    _media_player->setPlaylist(_media_play_list);
}

bool Music::setMedia(QUrl media)
{
    if(_media_play_list->clear())
    {
        return _media_play_list->addMedia(media);
    }
    return false;
}

void Music::play()
{
    _media_player->play();
}

void Music::stop()
{
    _media_player->stop();
}

QMediaPlayer::State Music::state() const
{
    return _media_player->state();
}

void Music::update()
{
    _media_player->setVolume(Configuration::volumeMusic());
}
