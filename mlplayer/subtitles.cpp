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


#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

#include <mlplayer/common.hpp>

#include "subtitles.hpp"


namespace mlplayer {


Subtitle::Subtitle(void)
:
	start_time(0),
	end_time(0)
{
}



QByteArray Subtitles_parser::find_codec_for(const QByteArray& data, const QString& language) const
{
	MLIB_D("Finding suitable text codec...");

	QByteArray best_codec = QTextCodec::codecForLocale()->name();
	QList<const char*> language_codecs;

	if(language == "ru")
	{
		language_codecs << "Windows-1251";
		language_codecs << "KOI8-R";
	}

	Q_FOREACH(const char* codec_name, language_codecs)
	{
		MLIB_D("Trying %1...", codec_name);

		QTextCodec* codec = QTextCodec::codecForName(codec_name);

		if(!codec)
		{
			MLIB_SW(tr("Unable to load text codec %1."), codec_name);
			continue;
		}

		QTextCodec::ConverterState state;
		codec->toUnicode(data.data(), data.size(), &state);

		if(!state.invalidChars)
		{
			best_codec = codec_name;
			break;
		}
	}

	MLIB_D("Using text codec %1.", best_codec);
	return best_codec;
}



QList<Subtitle> Subtitles_parser::get(const QString& path, const QString& language) const
{
	MLIB_D("Reading subtitles from '%1'...", path);

	QFile file(path);

	if(!file.open(QIODevice::ReadOnly))
	{
		M_THROW(PAM(
			_F( tr("Error while reading subtitle file '%1':"), file.fileName() ),
			file.errorString()
		));
	}

	QByteArray data = file.readAll();
	QTextStream stream(&data);
	stream.setCodec(this->find_codec_for(data, language));

	try
	{
		return this->parse(file.fileName(), &stream);
	}
	catch(m::Exception& e)
	{
		M_THROW(PAM(
			_F( tr("Error while parsing subtitle file '%1':"), file.fileName() ),
			EE(e)
		));
	}
}



QList<Subtitle> Subtitles_parser::parse(const QString& source_path, QTextStream* stream) const
{
	enum {
		// Reading ID of a subtitle.
		READING_ID,

		// Reading timings of a subtitle.
		READING_TIMINGS,

		// Reading subtitle text.
		READING_SUBTITLE
	} state = READING_ID;

	Subtitle subtitle;
	QList<Subtitle> subtitles;
	size_t line_num = 0;
	size_t last_id = 0;

	QRegExp id_regexp("^(\\d+)$");
	QRegExp timings_regexp(
		"^(\\d{1,2}):(\\d{1,2}):(\\d{1,2}),(\\d{1,3})\\s+-->\\s+(\\d{1,2}):(\\d{1,2}):(\\d{1,2}),(\\d{1,3})$" );

	while(true)
	{
		bool at_end = stream->atEnd();
		QString line = stream->readLine().trimmed();

		if(!at_end)
			line_num++;

		switch(state)
		{
			case READING_ID:
			{
				if(!at_end && !line.isEmpty())
				{
					if(id_regexp.indexIn(line) == -1)
						M_THROW(tr("Invalid subtitle id '%1' at line %2."), line, line_num);

					size_t id = id_regexp.cap(1).toUInt();
					MLIB_DV("ID: %1.", id);

					if(id < last_id)
					{
						MLIB_SW(_F(
							tr("Subtitles file '%1' at line %2: invalid subtitle id %3 - previous subtitle id was %4."),
							source_path, line_num, id, last_id
						));
					}
					last_id = id;

					state = READING_TIMINGS;
					subtitle.text.clear();
				}
			}
			break;

			case READING_TIMINGS:
			{
				if(at_end)
					M_THROW(tr("Unexpected end of file."));

				if(timings_regexp.indexIn(line) == -1)
					M_THROW(tr("Invalid subtitle timings '%1' at line %2."), line, line_num);

				subtitle.start_time =
				(
					+ Time_ms(timings_regexp.cap(1).toInt()) * 60 * 60
					+ Time_ms(timings_regexp.cap(2).toInt()) * 60
					+ Time_ms(timings_regexp.cap(3).toInt())
				) * 1000 + Time_ms(timings_regexp.cap(4).toInt());

				subtitle.end_time =
				(
					+ Time_ms(timings_regexp.cap(5).toInt()) * 60 * 60
					+ Time_ms(timings_regexp.cap(6).toInt()) * 60
					+ Time_ms(timings_regexp.cap(7).toInt())
				) * 1000 + Time_ms(timings_regexp.cap(8).toInt());

				MLIB_DV("Timings: %1 - %2.", subtitle.start_time, subtitle.end_time);

				state = READING_SUBTITLE;
			}
			break;

			case READING_SUBTITLE:
			{
				if(line.isEmpty())
				{
					if(subtitle.text.isEmpty())
					{
						MLIB_SW(_F(
							tr("Subtitles file '%1' at line %2: gotten subtitle [%3] with empty text."),
							source_path, line_num, last_id
						));
					}
					else
					{
						MLIB_DV("Text: %1", subtitle.text);
						subtitles << subtitle;
					}

					state = READING_ID;
				}
				else
				{
					if(!subtitle.text.isEmpty())
						subtitle.text += "\n";

					subtitle.text += line;
				}
			}
			break;

			default:
				M_THROW(tr("Logical error."));
				break;
		}

		if(at_end)
			break;
	}

	if(subtitles.empty())
		M_THROW(tr("File is empty."));

	return subtitles;
}


}

