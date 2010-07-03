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


#ifndef MLPLAYER_HEADER_SUBTITLES_VIEWPORT
#define MLPLAYER_HEADER_SUBTITLES_VIEWPORT

class QHBoxLayout;
#include <QtGui/QWidget>

#include <mlplayer/common.hpp>
#include <mlplayer/subtitles_view.hxx>


namespace mlplayer {


/// Widget that displays several subtitles for a video.
class Subtitles_viewport: public QWidget
{
	Q_OBJECT

	public:
		Subtitles_viewport(QWidget* parent = 0);


	private:
		/// Widget's layout.
		QHBoxLayout*			layout;

		/// Widgets that displays subtitles.
		QList<Subtitles_view*>	views;


	public:
		/// Adds subtitles to view.
		/// @throw m::Exception.
		void	add(const QString& path, const QString& language = "");


	signals:
		/// Emitted when current subtitle changes.
		void	current_subtitle_changed(const QString& text);

		/// Sets time for which we need to display subtitles.
		void	set_time(Time_ms time);
};


}

#endif
