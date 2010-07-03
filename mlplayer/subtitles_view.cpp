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


#include <QtCore/QEvent>

#include <QtGui/QScrollBar>
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtGui/QTextEdit>

#include <mlplayer/common.hpp>
#include <mlplayer/subtitles.hpp>

#include "subtitles_view.hpp"


namespace mlplayer {


Subtitles_view::Subtitles_view(const QString& path, const QString& language, QWidget *parent)
:
	QTextEdit(parent),
	been_showed(false),
	cur_subtitle(-1),
	// Throws m::Exception
	subtitles(Subtitles_parser().get(path, language))
{
	this->setReadOnly(true);
	this->document()->setUndoRedoEnabled(false);
	this->char_format_active.setFontWeight(QFont::Bold);

	// Filling the widget -->
	{
		QTextCursor cursor(this->document());

		Q_FOREACH(const Subtitle& subtitle, this->subtitles)
		{
			if(!this->text_mappings.empty())
				cursor.insertBlock();

			this->text_mappings << cursor.position();
			cursor.insertHtml(subtitle.text);
		}

		cursor.movePosition(QTextCursor::Start);
		cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
		cursor.setCharFormat(this->char_format_default);
	}
	// Filling the widget <--
}



bool Subtitles_view::event(QEvent* event)
{
	bool processed = QTextEdit::event(event);

	// We must scroll widget at first time it has been showed - it does not
	// scrolls until it has no X window.
	if(!this->been_showed && event->type() == QEvent::Show)
	{
		this->been_showed = true;
		this->scroll_to_active();
	}

	return processed;
}



void Subtitles_view::scroll_to_active(void)
{
	if(this->cur_subtitle < 0)
		this->verticalScrollBar()->setValue(this->verticalScrollBar()->minimum());
	else
	{
		QTextCursor cursor(this->document());

		cursor.setPosition(this->text_mappings[this->cur_subtitle]);

		if(this->cur_subtitle < this->text_mappings.size() - 1)
			cursor.setPosition(this->text_mappings[this->cur_subtitle + 1], QTextCursor::KeepAnchor);
		else
			cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

		this->setTextCursor(cursor);
		this->ensureCursorVisible();

		cursor.setPosition(this->text_mappings[this->cur_subtitle]);
		this->setTextCursor(cursor);
	}
}



void Subtitles_view::set_active_subtitle(int id)
{
	if(this->cur_subtitle == id)
		return;

	MLIB_D("Setting active subtitle to [%1].", id);

	if(this->cur_subtitle >= 0)
		this->set_subtitle_format(this->cur_subtitle, this->char_format_default);

	if(id >= 0)
		this->set_subtitle_format(id, this->char_format_active);

	// TODO: remove formatting
	emit this->current_subtitle_changed(
		id >=0 ? this->subtitles.at(id).text : QString() );

	this->cur_subtitle = id;
	this->scroll_to_active();
}



void Subtitles_view::set_subtitle_format(int id, const QTextCharFormat& format)
{
	QTextCursor cursor(this->document());
	cursor.setPosition(this->text_mappings.at(id));

	if(id != this->subtitles.size() - 1)
		cursor.setPosition(this->text_mappings.at(id + 1), QTextCursor::KeepAnchor);
	else
		cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

	cursor.setCharFormat(format);
}



void Subtitles_view::set_time(Time_ms time)
{
	int id = ( this->cur_subtitle >= 0 ? this->cur_subtitle : 0 );
	enum {
		BACKWARD = -1,
		FORWARD = 1
	} direction = ( time >= this->subtitles.at(id).start_time ? FORWARD : BACKWARD );

	while(id >= 0 && id < this->subtitles.size())
	{
		const Subtitle& subtitle = this->subtitles.at(id);

		if(subtitle.start_time <= time && time <= subtitle.end_time)
			break;

		if(direction == FORWARD)
		{
			if(subtitle.start_time > time)
			{
				id--;
				break;
			}
		}

		id += direction;
	}

	if(id >= this->subtitles.size())
		id = -1;

	this->set_active_subtitle(id);
}


}
