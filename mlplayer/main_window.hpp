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


#ifndef MLPLAYER_MAIN_WINDOW
#define MLPLAYER_MAIN_WINDOW

#include <boost/scoped_ptr.hpp>

#include <QtGui/QMainWindow>

#include <mlplayer/common.hpp>


namespace mlplayer {


namespace Ui {
    class Main_window;
}


/// Application main window.
class Main_window: public QMainWindow
{
	Q_OBJECT

	public:
		Main_window(QWidget* parent = 0);
		~Main_window(void);

	private:
		boost::scoped_ptr<Ui::Main_window> ui;
};


}

#endif
