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


#ifndef MLPLAYER_HEADER_SUBTITLES
#define MLPLAYER_HEADER_SUBTITLES

class QTextStream;

#include <mlplayer/common.hpp>


namespace mlplayer {


/// Represents subtitles for video file.
class Subtitles: public QObject
{
	Q_OBJECT

	private:
		/// Represents a single subtitle.
		class Subtitle
		{
			public:
				Subtitle(void);

			public:
				/// Subtitle start time.
				Time_ms	start_time;

				/// Subtitle end time.
				Time_ms end_time;

				/// Subtitle text.
				QString	text;
		};


	public:
		/// @throw m::Exception.
		Subtitles(const QString& path);


	private:
		/// Subtitles loaded from the file.
		QList<Subtitle>	subtitles;


	private:
		/// Reads subtitles from a stream;
		/// @throw m::Exception.
		void	parse(const QString& source_path, QTextStream* stream);
};


}

#endif
