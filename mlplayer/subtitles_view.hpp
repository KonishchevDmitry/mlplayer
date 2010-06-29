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


#ifndef MLPLAYER_HEADER_SUBTITLES_VIEW
#define MLPLAYER_HEADER_SUBTITLES_VIEW

#include <QtGui/QTextCharFormat>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

#include <mlplayer/common.hpp>
#include <mlplayer/subtitles.hpp>


namespace mlplayer {


/// Widget that displays subtitles for a video.
class Subtitles_view: public QTextEdit
{
	Q_OBJECT

	public:
		/// @throw m::Exception.
		Subtitles_view(const QString& path, QWidget* parent = 0);


	private:
		/// Did this widget been showed.
		bool			been_showed;


		/// ID of the current subtitle.
		int				cur_subtitle;

		/// Subtitles that we display.
		QList<Subtitle>	subtitles;

		/// Maps subtitle id to its position in the QTextEdit.
		QList<int>		text_mappings;


		/// Default character format.
		QTextCharFormat	char_format_default;

		/// Character format for currently active subtitle.
		QTextCharFormat	char_format_active;


	private:
		/// See QWidget::event() for details.
		bool	event(QEvent* event);

		/// Scrolls to currently active subtitle.
		void	scroll_to_active(void);

		/// Sets currently active subtitle.
		void	set_active_subtitle(int id);

		/// Sets format for text of subtitle \a id.
		void	set_subtitle_format(int id, const QTextCharFormat& format);


	signals:
		/// Emitted when current subtitle changes.
		void	current_subtitle_changed(const QString& text);


	public slots:
		/// Sets time for which we need to display subtitles.
		void	set_time(Time_ms time);
};


}

#endif
