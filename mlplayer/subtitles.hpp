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


/// Parses a file with subtitles.
class Subtitles_parser: public QObject
{
	Q_OBJECT

	public:
		/// Parses a file \a path and returns a list of Subtitle objects.
		/// @param language - a language of subtitles or an empty string.
		/// @throw m::Exception.
		QList<Subtitle>	get(const QString& path, const QString& language) const;

	private:
		/// Finds a codec suitable for \a data.
		/// @param language - a language of subtitles or an empty string.
		QByteArray		find_codec_for(const QByteArray& data, const QString& language) const;

		/// Reads subtitles from a stream;
		/// @throw m::Exception.
		QList<Subtitle>	parse(const QString& source_path, QTextStream* stream) const;
};


}

#endif
