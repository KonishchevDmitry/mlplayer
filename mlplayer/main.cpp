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


#include <QtCore/QTextCodec>

#include <QtGui/QApplication>

#include <mlplayer/common.hpp>
#include <mlplayer/main_window.hpp>


int main(int argc, char *argv[])
{
	using namespace mlplayer;

	QApplication app(argc, argv);

	// Configuring application -->
		QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

		app.setApplicationName(MLPLAYER_APP_NAME);
#warning
//		app.setApplicationVersion(get_version());
//		APP_BINARY_PATH = QDir(app.applicationFilePath()).absolutePath();
	// Configuring application <--

	// Processing command line arguments
//	process_command_line_options(app);
//
//	m::set_message_handler(m::MESSAGE_TYPE_INFO, &message_handler);
//#if MLPLAYER_DEVELOP_MODE
//	m::set_message_handler(m::MESSAGE_TYPE_SILENT_WARNING, &message_handler);
//#endif
//	m::set_message_handler(m::MESSAGE_TYPE_WARNING, &message_handler);
//	m::set_message_handler(m::MESSAGE_TYPE_ERROR, &message_handler);
		m::set_debug_level(m::DEBUG_LEVEL_VERBOSE);

	MLIB_D("Starting the application...");

Main_window* main_window = new Main_window(NULL);
main_window->show();
//media->enqueue("/home/username/music/song.mp3");
//media->enqueue(":/sounds/endsound.ogg");
//	Phonon::VideoWidget* player_widget = new Phonon::VideoWidget();
//	Phonon::VideoPlayer* player = new Phonon::VideoPlayer();
//	Phonon::MediaSource* source = new Phonon::MediaSource("/my_files/video/pub/What\ the\ Bleep\ Do\ We\ Know/What_the_Bleep_Do_We_Know.mkv");
//	Phonon::createPath(source, player_widget);
//	player.play();

//	QString install_dir = get_install_dir();
//	QLocale locale = QLocale::system();
//
//	QTranslator qt_translator;
//	QTranslator app_translator;
//	QTranslator mlib_translator;
//
//	// TODO: All code above does not show translated strings to the user.
//
//#ifdef Q_OS_UNIX
//	// Qt's locale detection under UNIX works wrong -->
//	{
//		QMap<QString,QString> env_vars;
//
//		Q_FOREACH(const QString& var, QProcess::systemEnvironment())
//		{
//			int pos = var.indexOf('=');
//			QString name = var.mid(0, pos);
//			QString value = var.mid(pos + 1);
//			env_vars[name] = value;
//			MLIB_DV("Gotten an environment variable: '%1'='%2'.", name, value);
//		}
//
//		if(!env_vars["LC_ALL"].isEmpty())
//		{
//			locale = QLocale(env_vars["LC_ALL"]);
//			MLIB_D("Setting the locale to LC_ALL's value '%1'.", locale.name());
//		}
//		else if(!env_vars["LC_MESSAGES"].isEmpty())
//		{
//			locale = QLocale(env_vars["LC_MESSAGES"]);
//			MLIB_D("Setting the locale to LC_MESSAGES's value '%1'.", locale.name());
//		}
//		else if(!env_vars["LANG"].isEmpty())
//		{
//			locale = QLocale(env_vars["LANG"]);
//			MLIB_D("Setting the locale to LANG's value '%1'.", locale.name());
//		}
//	}
//	// Qt's locale detection under UNIX works wrong <--
//#endif
//
//	// Loading translations -->
//	{
//		bool is;
//
//		is = qt_translator.load("qt_" + locale.name(),
//			QLibraryInfo::location(QLibraryInfo::TranslationsPath) );
//		if(is)
//			app.installTranslator(&qt_translator);
//		else
//			MLIB_D("Qt's translations for '%1' did not found.", locale.name());
//
//
//		if(!install_dir.isEmpty())
//		{
//			is = app_translator.load(_F("%1_%2", MLPLAYER_APP_UNIX_NAME, locale.name()),
//				QDir(install_dir).absoluteFilePath(MLPLAYER_APP_TRANSLATIONS_DIR) );
//			if(is)
//				app.installTranslator(&app_translator);
//			else
//				MLIB_D("Application's translations for '%1' did not found.", locale.name());
//
//			is = mlib_translator.load("mlib_" + locale.name(),
//				QDir(install_dir).absoluteFilePath(MLPLAYER_APP_TRANSLATIONS_DIR) );
//			if(is)
//				app.installTranslator(&mlib_translator);
//			else
//				MLIB_D("MLib's translations for '%1' did not found.", locale.name());
//		}
//	}
//	// Loading translations <--
//
//	// Handle operating system signals
//	m::sys::connect_end_work_system_signal(&app, SLOT(quit()));
//
//	// Setting application icon -->
//		if(!install_dir.isEmpty())
//		{
//			int sizes[] = { 8, 12, 16, 22, 24, 32, 48, 64, 128 };
//
//			QIcon icon;
//
//			BOOST_FOREACH(int size, sizes)
//			{
//				QString size_name = QString::number(size) + "x" + QString::number(size);
//
//				icon.addFile(
//					QDir(install_dir).absoluteFilePath(
//						MLPLAYER_APP_ICONS_DIR "/" + size_name + "/apps/" MLPLAYER_APP_UNIX_NAME ".png" ),
//					QSize(size, size)
//				);
//			}
//
//			icon.addFile(
//				QDir(install_dir).absoluteFilePath(
//					MLPLAYER_APP_ICONS_DIR "/scalable/apps/" MLPLAYER_APP_UNIX_NAME ".svg" )
//			);
//
//			app.setWindowIcon(icon);
//		}
//	// Setting application icon <--
return app.exec();
}

