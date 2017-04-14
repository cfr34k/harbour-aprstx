#include "settings.h"

Settings::Settings()
	: QObject()
{
	m_settings = new QSettings("cfr34k", "APRSTX");
}

Settings::~Settings()
{
	delete m_settings;
}
