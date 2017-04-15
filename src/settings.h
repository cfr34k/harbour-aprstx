#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QDir>

class Settings : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString userCall READ getUserCall WRITE setUserCall NOTIFY userCallChanged)
	Q_PROPERTY(QString comment READ getComment WRITE setComment NOTIFY commentChanged)
	Q_PROPERTY(unsigned int autoTxInterval READ getAutoTxInterval WRITE setAutoTxInterval NOTIFY autoTxIntervalChanged)

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
	QString getComment(void) { return m_settings->value("Comment", "").toString(); }
	unsigned int getAutoTxInterval(void) { return m_settings->value("AutoTxInterval", 60).toUInt(); }

public slots:
	void setUserCall(const QString &call)
	{
		m_settings->setValue("UserCall", call);
		emit userCallChanged(call);
	}

	void setComment(const QString &comment)
	{
		m_settings->setValue("Comment", comment);
		emit commentChanged(comment);
	}

	void setAutoTxInterval(unsigned int interval)
	{
		m_settings->setValue("AutoTxInterval", interval);
		emit autoTxIntervalChanged(interval);
	}

signals:
	void userCallChanged(const QString &newCall);
	void commentChanged(const QString &newComment);
	void autoTxIntervalChanged(unsigned int interval);
};

#endif // SETTINGS_H
