/**************************************************************************
*                                                                         *
*   MLPlayer - Multi-language player                                      *
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
	{
		QShortcut* shortcut;

		shortcut = new QShortcut(Qt::Key_Space, this);
		connect(shortcut, SIGNAL(activated()), ui->player, SLOT(play()));

		shortcut = new QShortcut(Qt::Key_J, this);
		connect(shortcut, SIGNAL(activated()), ui->player, SLOT(next_track()));

		shortcut = new QShortcut(Qt::Key_K, this);
		connect(shortcut, SIGNAL(activated()), ui->player, SLOT(previous_track()));
	}
	// Setting up shortcuts <--
}



Main_window::~Main_window()
{
}


}
