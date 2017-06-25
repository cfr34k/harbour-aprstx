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
	Q_PROPERTY(uint autoTxIntervalMin READ getAutoTxIntervalMin WRITE setAutoTxIntervalMin NOTIFY autoTxIntervalMinChanged)
	Q_PROPERTY(uint autoTxIntervalForced READ getAutoTxIntervalForced WRITE setAutoTxIntervalForced NOTIFY autoTxIntervalForcedChanged)
	Q_PROPERTY(uint autoTxDistance READ getAutoTxDistance WRITE setAutoTxDistance NOTIFY autoTxDistanceChanged)
	Q_PROPERTY(uint autoTxHeadingChange READ getAutoTxHeadingChange WRITE setAutoTxHeadingChange NOTIFY autoTxHeadingChangeChanged)
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
	uint getAutoTxIntervalMin(void) { return m_settings->value("AutoTxIntervalMin", 30).toUInt(); }
	uint getAutoTxIntervalForced(void) { return m_settings->value("AutoTxIntervalForced", 180).toUInt(); }
	uint getAutoTxDistance(void) { return m_settings->value("AutoTxDistance", 250).toUInt(); }
	uint getAutoTxHeadingChange(void) { return m_settings->value("AutoTxHeadingChange", 30).toUInt(); }
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

	void setAutoTxIntervalMin(uint interval)
	{
		m_settings->setValue("AutoTxIntervalMin", interval);
		emit autoTxIntervalMinChanged(interval);
	}

	void setAutoTxIntervalForced(uint interval)
	{
		m_settings->setValue("AutoTxIntervalForced", interval);
		emit autoTxIntervalForcedChanged(interval);
	}

	void setAutoTxDistance(uint distance)
	{
		m_settings->setValue("AutoTxDistance", distance);
		emit autoTxDistanceChanged(distance);
	}

	void setAutoTxHeadingChange(uint headingChange)
	{
		m_settings->setValue("AutoTxHeadingChange", headingChange);
		emit autoTxHeadingChangeChanged(headingChange);
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
	void autoTxIntervalMinChanged(uint interval);
	void autoTxIntervalForcedChanged(uint interval);
	void autoTxDistanceChanged(uint distance);
	void autoTxHeadingChangeChanged(uint headingChange);
	void voxToneDurationChanged(uint milliseconds);
	void tailFlagsChanged(uint flags);
	void iconIndexChanged(uint index);
};

#endif // SETTINGS_H
