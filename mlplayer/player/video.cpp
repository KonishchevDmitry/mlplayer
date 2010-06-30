/**************************************************************************
*                                                                         *
*   MLPlayer - Multi-language video player                                *
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


#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/VideoWidget>

#include <mlplayer/common.hpp>

#include "video.hpp"


namespace mlplayer { namespace player {


Video::Video(const QString& file_path, QWidget* parent)
:
	QObject(parent),
	object( new Phonon::MediaObject(this) ),
	audio( new Phonon::AudioOutput(Phonon::VideoCategory, this) ),
	widget( new Phonon::VideoWidget(parent) ),
	cur_pos(0),
	total_time(0)
{
	this->object->setCurrentSource(file_path);
// TODO
	this->object->setTickInterval(200);

	connect(this->object, SIGNAL(totalTimeChanged(qint64)),
		this, SLOT(total_time_changed(qint64)) );

	connect(this->object, SIGNAL(tick(qint64)),
		this, SLOT(tick(qint64)) );

	// We need to get the paused state on the object to unable seeking on it.
	this->object->play();
	this->object->pause();

	Phonon::createPath(this->object, this->audio);
	Phonon::createPath(this->object, this->widget);
}



Time_ms Video::get_cur_pos(void)
{
	return this->cur_pos;
}



Time_ms Video::get_total_time(void)
{
	return this->total_time;
}



void Video::total_time_changed(qint64 total_time)
{
	this->total_time = total_time;
	emit this->pos_changed(this->cur_pos, this->total_time);
}



void Video::tick(qint64 time)
{
	if(this->cur_pos != time)
	{
		this->cur_pos = time;
		emit this->pos_changed(this->cur_pos, this->total_time);
	}
}


}}
