#ifndef MANAGER_H_
#define MANAGER_H_

#include <QQmlApplicationEngine>
#include "facade.h"

class Manager: public QObject {
	Q_OBJECT
public:
	Manager();
public slots:
	void onSearchButtonPressed(const QString & theSearchPhrase);
	void onStatisticsButtonPressed();
private:
	void loadQml();
	void connectToDatabase();
private:
	QQmlApplicationEngine engine_;
	QObject * rootObject_;
	ConnectionPtr connection_;
};

#endif
