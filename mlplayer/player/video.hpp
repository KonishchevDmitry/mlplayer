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


#ifndef MLPLAYER_HEADER_PLAYER_VIDEO
#define MLPLAYER_HEADER_PLAYER_VIDEO

class QWidget;

namespace Phonon {
	class AudioOutput;
	class MediaObject;
	class VideoWidget;
}

#include <mlplayer/common.hpp>


namespace mlplayer { namespace player {


/// Video that a Player will play.
class Video: public QObject
{
	Q_OBJECT

	public:
		Video(const QString& file_path, QWidget* parent = NULL);


	public:
		/// Represents our video as an object.
		Phonon::MediaObject* const	object;

		/// Sends data to output devices.
		Phonon::AudioOutput* const	audio;

		/// Renders our video.
		Phonon::VideoWidget* const	widget;

	private:
		/// Current playing position on video.
		Time_ms	cur_pos;


	signals:
		/// Emitted when current playing position on video changes.
		void	pos_changed(Time_ms time);


	private slots:
		/// MediaObject's tick signal handler.
		void	tick(qint64 time);
};


}}

#endif
