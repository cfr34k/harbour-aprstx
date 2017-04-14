#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QDir>

class Settings : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString userCall READ getUserCall WRITE setUserCall NOTIFY userCallChanged)
	Q_PROPERTY(QString comment READ getComment WRITE setComment NOTIFY commentChanged)

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

signals:
	void userCallChanged(const QString &newCall);
	void commentChanged(const QString &newComment);
};

#endif // SETTINGS_H
