#ifndef MANAGER_H_
#define MANAGER_H_

#include <QQmlApplicationEngine>

class Manager: public QObject {
	Q_OBJECT
public:
	Manager();
public slots:
	void onSearchButtonPressed(const QString & theSearchPhrase);
private:
	QQmlApplicationEngine engine_;
	QObject * rootObject_;
};

#endif
