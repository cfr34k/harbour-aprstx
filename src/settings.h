#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QDir>

class Settings : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString userCall READ getUserCall WRITE setUserCall NOTIFY userCallChanged)
	Q_PROPERTY(uint userSSID READ getUserSSID WRITE setUserSSID NOTIFY userSSIDChanged)
	Q_PROPERTY(QString comment READ getComment WRITE setComment NOTIFY commentChanged)
	Q_PROPERTY(uint autoTxInterval READ getAutoTxInterval WRITE setAutoTxInterval NOTIFY autoTxIntervalChanged)
	Q_PROPERTY(uint voxToneDuration READ getVoxToneDuration WRITE setVoxToneDuration NOTIFY voxToneDurationChanged)
	Q_PROPERTY(uint tailFlags READ getTailFlags WRITE setTailFlags NOTIFY tailFlagsChanged)
	Q_PROPERTY(uint iconIndex READ getIconIndex WRITE setIconIndex NOTIFY iconIndexChanged)

private:
	QSettings *m_settings;

	Settings(); // singleton class, constructor is private

public:
	~Settings();

	static Settings& instance()
	{
		static Settings theSettings;
		return theSettings;
	}

	QString getUserCall(void) { return m_settings->value("UserCall", "").toString(); }
	uint getUserSSID(void) { return m_settings->value("UserSSID", 0).toUInt(); }
	QString getComment(void) { return m_settings->value("Comment", "").toString(); }
	uint getAutoTxInterval(void) { return m_settings->value("AutoTxInterval", 60).toUInt(); }
	uint getVoxToneDuration(void) { return m_settings->value("VoxToneDuration", 100).toUInt(); }
	uint getTailFlags(void) { return m_settings->value("TailFlags", 20).toUInt(); }
	uint getIconIndex(void) { return m_settings->value("IconIndex", 0).toUInt(); }

public slots:
	void setUserCall(const QString &call)
	{
		m_settings->setValue("UserCall", call);
		emit userCallChanged(call);
	}

	void setUserSSID(uint ssid)
	{
		m_settings->setValue("UserSSID", ssid);
		emit userSSIDChanged(ssid);
	}

	void setComment(const QString &comment)
	{
		m_settings->setValue("Comment", comment);
		emit commentChanged(comment);
	}

	void setAutoTxInterval(uint interval)
	{
		m_settings->setValue("AutoTxInterval", interval);
		emit autoTxIntervalChanged(interval);
	}

	void setVoxToneDuration(uint milliseconds)
	{
		m_settings->setValue("VoxToneDuration", milliseconds);
		emit voxToneDurationChanged(milliseconds);
	}

	void setTailFlags(uint flags)
	{
		m_settings->setValue("TailFlags", flags);
		emit tailFlagsChanged(flags);
	}

	void setIconIndex(uint index)
	{
		m_settings->setValue("IconIndex", index);
		emit iconIndexChanged(index);
	}

signals:
	void userCallChanged(const QString &newCall);
	void userSSIDChanged(uint ssid);
	void commentChanged(const QString &newComment);
	void autoTxIntervalChanged(uint interval);
	void voxToneDurationChanged(uint milliseconds);
	void tailFlagsChanged(uint flags);
	void iconIndexChanged(uint index);
};

#endif // SETTINGS_H
