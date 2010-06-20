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
}



Main_window::~Main_window()
{
}



void Main_window::create_shortcut(const QKeySequence& key, QObject* target, const char* slot)
{
	connect(new QShortcut(key, this), SIGNAL(activated()), target, slot);
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
