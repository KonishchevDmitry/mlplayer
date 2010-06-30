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

#include <boost/foreach.hpp>

#include <QtGui/QKeySequence>
#include <QtGui/QShortcut>

#include <mlplayer/common.hpp>

#include "main_window.hpp"
#include "ui_main_window.h"


namespace mlplayer {


Main_window::Main_window(QWidget *parent)
:
	QMainWindow(parent),
	ui(new Ui::Main_window)
{
	ui->setupUi(this);

	// Setting up shortcuts -->
		this->create_shortcut( Qt::Key_Space, ui->player, SLOT(play_pause()) );

		this->create_shortcut( Qt::Key_Left, this, SLOT(seek_left_small()) );
		this->create_shortcut( Qt::Key_Right, this, SLOT(seek_right_small()) );
		this->create_shortcut( Qt::Key_Comma, this, SLOT(seek_left_ordinary()) );
		this->create_shortcut( Qt::Key_Period, this, SLOT(seek_right_ordinary()) );
		this->create_shortcut( Qt::Key_M, this, SLOT(seek_left_big()) );
		this->create_shortcut( Qt::Key_Slash, this, SLOT(seek_right_big()) );

		this->create_shortcut( Qt::Key_J, ui->player, SLOT(next_track()) );
		this->create_shortcut( Qt::Key_K, ui->player, SLOT(previous_track()) );
	// Setting up shortcuts <--

	connect(ui->player, SIGNAL(master_pos_changed(Time_ms)),
		ui->subtitles, SIGNAL(set_time(Time_ms)) );

	connect(ui->player, SIGNAL(cur_pos_changed(Time_ms, Time_ms)),
		this, SLOT(current_video_position_changed(Time_ms, Time_ms)) );

	connect(ui->subtitles, SIGNAL(current_subtitle_changed(const QString&)),
		this, SLOT(current_subtitle_changed(const QString&)) );
}



Main_window::~Main_window()
{
}



void Main_window::create_shortcut(const QKeySequence& key, QObject* target, const char* slot)
{
	connect(new QShortcut(key, this), SIGNAL(activated()), target, slot);
}



void Main_window::current_subtitle_changed(const QString& text)
{
	ui->current_subtitle_text->setText(QString(text).replace('\n', ' '));
}



void Main_window::current_video_position_changed(Time_ms cur_time, Time_ms total_time)
{
	cur_time /= 1000;
	total_time /= 1000;

	const Time minute = 60;
	const Time hour = 60 * minute;
	bool show_hours = ( cur_time >= hour || total_time >= hour ? true : false );

	QString time_string;
	Time times[] = { cur_time, total_time };

	BOOST_FOREACH(Time time, times)
	{
		if(!time_string.isEmpty())
			time_string += '/';

		if(show_hours)
			time_string += QString().sprintf("%02lld:", time / hour);

		time_string += QString().sprintf("%02lld:%02lld", time / minute % hour, time % minute);
	}

	ui->time->setText(time_string);
}



void Main_window::seek_left_big(void)
{
	ui->player->seek(-config::big_seek_size);
}



void Main_window::seek_left_ordinary(void)
{
	ui->player->seek(-config::ordinary_seek_size);
}



void Main_window::seek_left_small(void)
{
	ui->player->seek(-config::small_seek_size);
}



void Main_window::seek_right_big(void)
{
	ui->player->seek(config::big_seek_size);
}



void Main_window::seek_right_ordinary(void)
{
	ui->player->seek(config::ordinary_seek_size);
}



void Main_window::seek_right_small(void)
{
	ui->player->seek(config::small_seek_size);
}


}
