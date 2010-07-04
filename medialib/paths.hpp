/**************************************************************************
*                                                                         *
*   medialib - a small collection of classes that may be useful in        *
*   development of applications that works with media files.              *
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


#ifndef MEDIALIB_HEADER_PATHS
#define MEDIALIB_HEADER_PATHS

#include <mlib/core.hpp>

#include <medialib/subtitles.hxx>


namespace medialib {


/// Stores information about TV show gotten from its video file path.
class Tv_show_info
{
	public:
		Tv_show_info(void);


	public:
		/// Possible names of a TV show.
		///
		/// Sometimes it's too hard to determine TV show name, so we provide a list
		/// of all possible names.
		QList<QString>	names;

		/// TV show season number.
		int				season;

		/// Season's episode number.
		int				episode;

		/// Extra info (releaser name, language, etc).
		QList<QString>	extra_info;


	public:
		/// Checks whether this object and \a info points to the same TV show
		/// episode.
		bool	equals(const Tv_show_info& info) const;

		/// Returns info's string representation.
		QString	to_string(void) const;
};


/// Gets TV show info from path to its video file.
/// @throw m::Exception.
Tv_show_info	get_tv_show_info_by_path(const QString& path);

/// Returns a list of subtitles for TV show episode \a path that we can find in
/// the same directory and a list with \a path followed by other video files
/// with this episode (possible with different translations).
/// @throw m::Exception.
void			get_tv_show_related_media(const QString& path, QList<QString>* videos, QList<Subtitles_info>* subtitles);

/// Checks whether path is a video file.
bool			is_video_file(const QString& path);


}

#endif
