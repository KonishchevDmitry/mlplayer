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


#include <boost/foreach.hpp>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>

#include <mlib/core.hpp>

#include <medialib/subtitles.hpp>

#include "paths.hpp"


namespace medialib {


namespace {

	/// A list of known video file extensions.
	const char* const VIDEO_FILE_EXTENSIONS[] = {
		"avi",
		"mkv",
		"mp4",
		"wmv"
	};


	/// An empty object for using its tr() method in functions of this file.
	class Media_paths: public QObject
	{
	};


	/// Parses video or subtitles file name and returns info about TV show to
	/// which it belongs.
	/// @param subtitle - determines whether this is a video or a subtitles
	/// file.
	/// @throw m::Exception.
	Tv_show_info	parse_tv_show_file_name(const QString& path, bool subtitle = false);

	/// Brings tr() to functions.
	QString			tr(const char* text, const char* disambiguation = 0, int n = -1);



	Tv_show_info parse_tv_show_file_name(const QString& path, bool subtitle)
	{
		try
		{
			Tv_show_info info;

			QString file_name = QFileInfo(path).fileName();
			file_name = file_name.mid(0, file_name.lastIndexOf('.'));

			// Splitting file name's data -->
			{
				QRegExp std_regexp("^(.+)\\.s(\\d+)e(\\d+)(\\..*){0,1}$", Qt::CaseInsensitive);
				QRegExp spaced_regexp("^(.+)\\s+-\\s+(\\d+)x(\\d+)(\\s+-.*){0,1}$", Qt::CaseInsensitive);

				if(!std_regexp.indexIn(file_name))
				{
					QString name = std_regexp.cap(1).trimmed();
					info.season = std_regexp.cap(2).toInt();
					info.episode = std_regexp.cap(3).toInt();
					info.extra_info = std_regexp.cap(4).mid(1).split('.');

					info.names << name;

					// Checking the show name -->
					{
						// Sometimes releasers include in the show name a year when TV show
						// started. For example, V TV show files named as
						// V.2009.S01E10.HDTV.XviD-2HD.avi.

						QRegExp show_year_regex("^(.+)\\.\\d{4}$");

						if(show_year_regex.indexIn(name) >= 0)
							info.names << show_year_regex.cap(1);
					}
					// Checking the show name <--

					// Converting names to the human readable format
					BOOST_FOREACH(QString& name, info.names)
						name = name.replace('.', ' ').trimmed();
				}
				else if(!spaced_regexp.indexIn(file_name))
				{
					info.names << spaced_regexp.cap(1).trimmed();
					info.season = spaced_regexp.cap(2).toInt();
					info.episode = spaced_regexp.cap(3).toInt();

					QString extra_info = spaced_regexp.cap(4);
					if(!extra_info.isEmpty())
						extra_info = extra_info.mid(extra_info.indexOf('-') + 1).trimmed();
					info.extra_info = extra_info.split('.');
				}
				else
					M_THROW("Invalid file name.");
			}
			// Splitting file name's data <--

			// Checking gotten data -->
			{
				QRegExp name_regexp("[a-z0-9]", Qt::CaseInsensitive);

				BOOST_FOREACH(QString& name, info.names)
					if(name.isEmpty() || name_regexp.indexIn(name) < 0)
						M_THROW("Invalid TV show name.", name);

				if(!info.season || !info.episode)
					M_THROW("Invalid season or episode number.");
			}
			// Checking gotten data <--

			return info;
		}
		catch(m::Exception& e)
		{
			if(subtitle)
				M_THROW(tr("This is not a TV show subtitles file or it has non-standard file name."));
			else
				M_THROW(tr("This is not a TV show video file or it has non-standard file name."));
		}
	}



	QString tr(const char* text, const char* disambiguation, int n)
	{
		return Media_paths::tr(text, disambiguation, n);
	}
}



// Tv_show_info -->
	Tv_show_info::Tv_show_info(void)
	:
		season(0),
		episode(0)
	{
	}



	bool Tv_show_info::equals(const Tv_show_info& info) const
	{
		if(this->season != info.season || this->episode != info.episode)
			return false;

		Q_FOREACH(const QString& this_name, this->names)
			Q_FOREACH(const QString& other_name, info.names)
				if(this_name.toLower() == other_name.toLower())
					return true;

		return false;
	}



	QString Tv_show_info::to_string(void) const
	{
		return _F("%1 season %2 episode %3 %4",
			this->names, this->season, this->episode, this->extra_info ).trimmed();
	}
// Tv_show_info <--



Tv_show_info get_tv_show_info_by_path(const QString& path)
{
	if(!is_video_file(path))
		M_THROW(tr("This is not a video file."));

	return parse_tv_show_file_name(path);
}



void get_tv_show_related_media(const QString& path, QList<QString>* videos, QList<Subtitles_info>* subtitles)
{
	MLIB_D("Searching for media related to TV show '%1'...", path);

	QDir video_dir(QFileInfo(path).dir());
	QString master_video_file_name = QFileInfo(path).fileName();

	Tv_show_info master_info = get_tv_show_info_by_path(path);
	MLIB_D("Gotten TV show info: %1.", master_info.to_string());
	*videos << path;

	// Searching for videos -->
	{
		QStringList video_filter;
		BOOST_FOREACH(const char* extension, VIDEO_FILE_EXTENSIONS)
			video_filter << QString("*.") + extension;

		QStringList video_files = video_dir.entryList(
			video_filter, QDir::Files, QDir::Name | QDir::IgnoreCase | QDir::LocaleAware );

		Q_FOREACH(const QString& file_name, video_files)
		{
			if(file_name == master_video_file_name)
				continue;

			try
			{
				QString video_file = video_dir.filePath(file_name);
				MLIB_D("Checking for '%1'...", video_file);
				Tv_show_info info = get_tv_show_info_by_path(video_file);

				if(info.equals(master_info))
				{
					*videos << video_file;
					MLIB_D("    Added (%1).", info.to_string());
				}
				else
					MLIB_D("    %1 is not equals to our master video file.", info.to_string());
			}
			catch(m::Exception& e)
			{
				MLIB_D("    %1", EE(e));
			}
		}
	}
	// Searching for videos <--

	// Searching for subtitles -->
	{
		QStringList subtitles_filter;
		subtitles_filter << "*.srt";

		QStringList subtitles_files = video_dir.entryList(
			subtitles_filter, QDir::Files, QDir::Name | QDir::IgnoreCase | QDir::LocaleAware );

		Q_FOREACH(const QString& file_name, subtitles_files)
		{
			try
			{
				QString subtitles_file = video_dir.filePath(file_name);
				MLIB_D("Checking for '%1'...", subtitles_file);
				Tv_show_info info = parse_tv_show_file_name(subtitles_file, true);

				if(info.equals(master_info))
				{
					QString language;

					if(!info.extra_info.isEmpty() && info.extra_info.last().size() == 2)
						language = info.extra_info.last();

					*subtitles << Subtitles_info(subtitles_file, language);
					MLIB_D("    Added (%1).", info.to_string());
				}
				else
					MLIB_D("    %1 is not belongs to our master video file.", info.to_string());
			}
			catch(m::Exception& e)
			{
				MLIB_D("    %1", EE(e));
			}
		}
	}
	// Searching for subtitles <--
}



bool is_video_file(const QString& path)
{
	BOOST_FOREACH(const char* extension, VIDEO_FILE_EXTENSIONS)
		if(path.toLower().endsWith(QString(".") + extension))
			return true;

	return false;
}


}

