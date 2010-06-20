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

#include <mlplayer/common.hpp>
#include <mlplayer/subtitles.hpp>

#include "subtitles_view.hpp"


namespace mlplayer {


Subtitles_view::Subtitles_view(QWidget *parent)
:
	QWidget(parent)
{
	Subtitles s("/my_files/programs/mlplayer.build/FlashForward.S01E16.HDTV.XviD-2HD.srt");
}


}
