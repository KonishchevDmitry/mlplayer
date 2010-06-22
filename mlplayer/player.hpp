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


#ifndef MLPLAYER_HEADER_PLAYER
#define MLPLAYER_HEADER_PLAYER

class QStackedLayout;
#include <QtGui/QWidget>

#include <mlplayer/common.hpp>
#include <mlplayer/player/video.hxx>


namespace mlplayer {


/// Widget that plays video files on several languages.
class Player: public QWidget
{
	Q_OBJECT

	private:
		typedef player::Video Video;

		/// Describes player's current state.
		enum State {
			/// Player just created and loads all videos given to it. It wont
			/// work in this state.
			LOADING,

			/// Player is ready for requests.
			WORKING
		};


	public:
		Player(QWidget* parent = 0);


	private:
		/// Player's current state.
		State						state;


		/// Id of the video with original language.
		static const size_t			master_video_id = 0;

		/// Videos that this object plays.
		QList<Video*>				videos;

		/// Currently selected video.
		Video*						current_video;

		/// Index of currently selected video.
		size_t						current_video_id;


		/// Layout with video widgets.
		QStackedLayout* const		video_layout;


	private:
		/// Adds video to the list of available videos.
		void	add_video(const QString& file_path);

		/// Checks whether the player now is playing a video.
		bool	playing(void);

		/// Switches to the track which id is += \a direction.
		void	roll_tracks(ssize_t direction);

		/// Sets current video to video whith id == \a id.
		void	set_current_video(size_t id);


	signals:
		/// Emitted when master stream playing position changes.
		void	master_pos_changed(Time_ms time);


	public slots:
		/// Switches to the next track.
		void	next_track(void);

		/// Pauses playing.
		void	pause(void);

		/// Starts playing.
		void	play(void);

		/// Toggles playing/pausing state.
		void	play_pause(void);

		/// Switches to the previous track.
		void	previous_track(void);

		/// Seeks video from the current position.
		void	seek(int seconds);

	private slots:
		/// Called when video source state changes.
		void	video_source_state_changed(void);
};


}

#endif
