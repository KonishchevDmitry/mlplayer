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


#ifndef MLPLAYER_HEADER_MAIN_WINDOW
#define MLPLAYER_HEADER_MAIN_WINDOW

#include <boost/scoped_ptr.hpp>

class QKeySequence;
#include <QtGui/QMainWindow>

#include <medialib/subtitles.hxx>

#include <mlplayer/common.hpp>


namespace mlplayer {


namespace Ui {
    class Main_window;
}


/// Application main window.
class Main_window: public QMainWindow
{
	Q_OBJECT

	private:
		typedef medialib::Subtitles_info Subtitles_info;


	public:
		Main_window(
			const QList<QString>& videos,
			const QList<Subtitles_info>& subtitles,
			QWidget* parent = 0
		);
		~Main_window(void);


	private:
		boost::scoped_ptr<Ui::Main_window> ui;


	private:
		/// Creates a hotkey connection.
		void	create_shortcut(const QKeySequence& key, QObject* target, const char* slot);


	private slots:
		/// Called when current subtitle changes.
		void	current_subtitle_changed(const QString& text);

		/// Called when current video's playing position changes.
		void	current_video_position_changed(Time_ms cur_time, Time_ms total_time);

		/// Seeks video from current position by the big jump.
		void	seek_left_big(void);

		/// Seeks video from current position by the ordinary jump.
		void	seek_left_ordinary(void);

		/// Seeks video from current position by the small jump.
		void	seek_left_small(void);

		/// Seeks video from current position by the big jump.
		void	seek_right_big(void);

		/// Seeks video from current position by the ordinary jump.
		void	seek_right_ordinary(void);

		/// Seeks video from current position by the small jump.
		void	seek_right_small(void);
};


}

#endif
