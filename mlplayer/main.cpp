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


#include <cstdlib>
#include <cstdio>

#include <memory>

#include <boost/foreach.hpp>

#include <QtCore/QDir>
#include <QtCore/QSize>
#include <QtCore/QTextStream>

#include <QtGui/QApplication>
#include <QtGui/QIcon>

#include <mlib/gui/core.hpp>
#include <mlib/sys/system_signal_notifier.hpp>

#include <mlplayer/common.hpp>
#include <mlplayer/main_window.hpp>
// TODO
#include <medialib/paths.hpp>
#include <medialib/subtitles.hpp>


namespace {


	/// Parses and processes command line options.
	/// @return paths to video files to play.
	QStringList	process_command_line_options(const QStringList& args);



	QStringList process_command_line_options(const QStringList& args)
	{
		bool first_arg = true;
		bool debug = false;
		bool verbose = false;
		QStringList videos;

		Q_FOREACH(const QString& arg, args)
		{
			if(first_arg)
			{
				first_arg = false;
				continue;
			}

			if(arg == "--debug")
				debug = true;
			else if(arg == "--help")
			{
				QTextStream stream(stdout);
				stream << _F(QApplication::tr(
						"Usage: %1 [OPTION]...\n"
						"\n"
						"Options:\n"
						"  --help     show this help message\n"
						"  --debug    turn on debug messages\n"
						"  --verbose  turn on verbose debug messages\n"
						"  --version  print application version and exit\n"
				), MLPLAYER_APP_UNIX_NAME) << endl;
				exit(EXIT_SUCCESS);
			}
			else if(arg == "--verbose")
				verbose = true;
			else if(arg == "--version")
			{
				QTextStream stream(stdout);
				stream
					<< QCoreApplication::applicationName()
					<< " "
					<< QCoreApplication::applicationVersion()
					<< endl;
				exit(EXIT_SUCCESS);
			}
			else
				videos << arg;
		}

		if(debug)
		{
			if(verbose)
				m::set_debug_level(m::DEBUG_LEVEL_VERBOSE);
			else
				m::set_debug_level(m::DEBUG_LEVEL_ENABLED);
		}
		else if(verbose)
		{
			MLIB_W(QApplication::tr(
				"Invalid command line options: --verbose must be specified in conjunction with --debug." ));
			exit(EXIT_FAILURE);
		}
	#if MLPLAYER_DEVELOP_MODE
		else
		{
			m::set_debug_level(m::DEBUG_LEVEL_ENABLED);
			//m::set_debug_level(m::DEBUG_LEVEL_VERBOSE);
		}
	#endif

		return videos;
	}


}
// TODO
#if 0
QString get_app_home_dir(void)
{
	#ifdef Q_OS_UNIX
		return QDir::home().filePath("." MLPLAYER_APP_UNIX_NAME);
	#else
		return QDir::home().filePath(MLPLAYER_APP_NAME);
	#endif
}
#endif



int main(int argc, char* argv[])
{
	std::auto_ptr<QApplication> app = m::gui::init(argc, argv, MLPLAYER_APP_NAME,
		m::get_version(MLPLAYER_VERSION_MAJOR, MLPLAYER_VERSION_MINOR, MLPLAYER_VERSION_PATCH) );

	MLIB_D("Starting the application...");

	QString install_dir;

	// Getting installation directory -->
		try
		{
			install_dir = m::get_app_install_dir(MLPLAYER_APP_BIN_DIR);
		}
		catch(m::Exception& e)
		{
			MLIB_SW(QApplication::tr("Unable to determine application's installation directory"), EE(e));
		}
	// Getting installation directory <--

	// Loading translations
	m::load_translations(
		install_dir.isEmpty() ? install_dir : QDir(install_dir).absoluteFilePath(MLPLAYER_APP_TRANSLATIONS_DIR),
		MLPLAYER_APP_UNIX_NAME
	);

	// Handle operating system signals
	m::sys::connect_end_work_system_signal(app.get(), SLOT(quit()));

	// Setting application icon -->
		if(!install_dir.isEmpty())
		{
			int sizes[] = { 24, 48 };

			QIcon icon;

			BOOST_FOREACH(int size, sizes)
			{
				QString size_name = QString::number(size) + "x" + QString::number(size);

				icon.addFile(
					QDir(install_dir).absoluteFilePath(
						MLPLAYER_APP_ICONS_DIR "/" + size_name + "/apps/" MLPLAYER_APP_UNIX_NAME ".svg" ),
					QSize(size, size)
				);
			}

			icon.addFile(
				QDir(install_dir).absoluteFilePath(
					MLPLAYER_APP_ICONS_DIR "/48x48/apps/" MLPLAYER_APP_UNIX_NAME ".svg" )
			);

			app->setWindowIcon(icon);
		}
	// Setting application icon <--

	try
	{
		// Processing command line arguments
		QStringList args = process_command_line_options(app->arguments());

		if(args.isEmpty())
			M_THROW(QApplication::tr("There is no video file specified to play."));
		else if(args.size() != 1)
			M_THROW(QApplication::tr("Gotten too may video files to play. You must specify only one video file."));

		QStringList videos;
		QList<medialib::Subtitles_info> subtitles;

		try
		{
			medialib::get_tv_show_related_media(args.first(), &videos, &subtitles);
		}
		catch(m::Exception& e)
		{
			M_THROW(PAM( _F(QApplication::tr("Error while getting '%1' info:"), args.first()), EE(e) ));
		}

		int exit_code;

		// Running the application -->
		{
			mlplayer::Main_window main_window(videos, subtitles);
			m::gui::set_main_window(&main_window);

			main_window.show();
			exit_code = app->exec();

			MLIB_D("Destroying the main window...");
			m::gui::set_main_window(NULL);
		}
		// Running the application <--

		MLIB_D("Exiting with code %1...", exit_code);
		return exit_code;
	}
	catch(m::Exception& e)
	{
		MLIB_W(_F( QApplication::tr("Unable to start %1"), MLPLAYER_APP_NAME ), EE(e) );
		return EXIT_FAILURE;
	}
}

