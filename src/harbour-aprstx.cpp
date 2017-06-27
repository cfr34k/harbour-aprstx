/*
	This file is part of APRSTX - An APRS transmitter for SailfishOS
	Copyright (C) 2017  Thomas Kolb

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtQuick>

#include <sailfishapp.h>

#include "settings.h"
#include "aprsctrl.h"

int main(int argc, char *argv[])
{
	QGuiApplication *app = SailfishApp::application(argc, argv);
	QQuickView *view = SailfishApp::createView();

	APRSCtrl aprsctrl;

	qmlRegisterType<APRSCtrl>("APRSTypes", 1, 0, "APRSCtrlTypes");

	view->rootContext()->setContextProperty("settings", &(Settings::instance()));
	view->rootContext()->setContextProperty("aprsctrl", &aprsctrl);

	view->setSource(SailfishApp::pathTo("qml/harbour-aprstx.qml"));
	view->show();

	int result = app->exec();

	delete view;
	delete app;

	return result;
}
