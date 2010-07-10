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


#include <QtGui/QHBoxLayout>

#include <mlplayer/common.hpp>

#include "subtitles_viewport/subtitles_view.hpp"

#include "subtitles_viewport.hpp"


namespace mlplayer {


Subtitles_viewport::Subtitles_viewport(QWidget *parent)
:
	QWidget(parent),
	layout(new QHBoxLayout(this))
{
	this->layout->setContentsMargins(0, 0, 0, 0);
}



void Subtitles_viewport::add(const QString& path, const QString& language)
{
	// Throws m::Exception
	Subtitles_view* view = new Subtitles_view(path, language, this);
	this->views << view;

	this->layout->addWidget(view);

	connect(this, SIGNAL(set_time(Time_ms)),
		view, SLOT(set_time(Time_ms)) );

	// Watching for original language subtitles
	if(this->views.size() == 1)
		connect(view, SIGNAL(current_subtitle_changed(const QString&)),
			this, SIGNAL(current_subtitle_changed(const QString&)) );

	view->show();
}


}
