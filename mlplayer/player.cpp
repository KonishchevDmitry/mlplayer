/**************************************************************************
*                                                                         *
*   MLPlayer - Multi-language player                                      *
*                                                                         *
*   Copyright (C) 2010, Dmitry Konishchev                                 *
*   http://konishchevdmitry.blogspot.com/                                 *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 3 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
*   GNU General Public License for more details.                          *
*                                                                         *
**************************************************************************/

#include <QtGui/QStackedLayout>

#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/VideoPlayer>
#include <Phonon/VideoWidget>

#include <mlplayer/common.hpp>

#include "player.hpp"
#include "player_private.hpp"


namespace mlplayer {


namespace Player_aux {


	Video::Video(const QString& file_path, QWidget* parent)
	:
		QObject(parent),
		object( new Phonon::MediaObject(this) ),
		audio( new Phonon::AudioOutput(Phonon::VideoCategory, this) ),
		widget( new Phonon::VideoWidget(parent) )
	{
		Phonon::createPath(this->object, this->audio);
		Phonon::createPath(this->object, this->widget);
		this->object->setCurrentSource(file_path);
	}


}


Player::Player(QWidget *parent)
:
	QWidget(parent),
	state(LOADING),
	video_layout(new QStackedLayout(this))
{
	this->add_video("/my_files/programs/mlplayer.build/FlashForward.S01E16.HDTV.XviD-2HD.avi");
	this->add_video("/my_files/programs/mlplayer.build/FlashForward.s01e16.rus.LostFilm.TV.avi");
}



Player::~Player(void)
{
}



void Player::add_video(const QString& file_path)
{
	Video* video = new Video(file_path, this);
	connect(video->object, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
		this, SLOT(video_source_state_changed()) );
	this->video_layout->addWidget(video->widget);
	video->widget->show();
	this->videos << video;
}



void Player::next_track(void)
{
	this->roll_tracks(1);
}



void Player::pause(void)
{
	if(this->state != WORKING)
		return;

	MLIB_D("Pausing current video...");
	this->current_video->object->pause();
}



void Player::play(void)
{
	if(this->state != WORKING)
		return;

	MLIB_D("Playing current video...");
	this->current_video->object->play();
}



bool Player::playing(void)
{
	if(this->state == WORKING)
	{
		switch(this->current_video->object->state())
		{
			case Phonon::PlayingState:
			case Phonon::BufferingState:
				return true;
				break;

			default:
				return false;
				break;
		}
	}
	else
		return false;
}



void Player::previous_track(void)
{
	this->roll_tracks(-1);
}



void Player::roll_tracks(ssize_t direction)
{
	if(this->state != WORKING)
		return;

	MLIB_D("Rolling tracks to [%1]...", direction);

	size_t new_video_id = this->current_video_id + direction;

	if(new_video_id >= size_t(this->videos.size()))
		new_video_id = (direction < 0 ? this->videos.size() - 1 : 0);

	if(this->current_video_id != new_video_id)
	{
		bool now_playing = this->playing();
		this->pause();

		this->set_current_video(new_video_id);

		if(now_playing)
			this->play();
	}
}



void Player::set_current_video(size_t id)
{
	this->current_video_id = id;
	this->current_video = this->videos[id];

	MLIB_D("Setting current video to the '%1'...",
		this->current_video->object->currentSource().fileName() );
	this->video_layout->setCurrentIndex(id);
}



void Player::video_source_state_changed(void)
{
	if(this->state != LOADING)
		return;

	Phonon::MediaObject* object =
		m::checked_qobject_cast<Phonon::MediaObject*>(this->sender());

	MLIB_D("Video source '%1' changed its state to [%2].",
		object->currentSource().fileName(), object->state() );

	if(object->state() == Phonon::ErrorState)
	{
		// TODO
		MLIB_E(PAM(
			_F( tr("Error while opening file '%1':"), object->currentSource().fileName() ),
			object->errorString()
		));
	}
	else
	{
		bool all_is_ready = true;

		Q_FOREACH(Video* video, this->videos)
		{
			if(video->object->state() != Phonon::StoppedState)
			{
				all_is_ready = false;
				break;
			}
		}

		if(all_is_ready)
		{
			MLIB_D("All videos has been loaded.");
			this->set_current_video(this->master_video_id);
			this->state = WORKING;
		}
	}
}


}
