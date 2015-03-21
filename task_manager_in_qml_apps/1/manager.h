/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef MANAGER_H_
#define MANAGER_H_

#include <QQmlApplicationEngine>

class Manager: public QObject {
	Q_OBJECT
public:
	Manager();
public slots:
	void onSlowOperation();
	void onLongOperationsChain0();
	void onLongOperationsChain1();
	void onLongOperationsChain2();
	void onLongOperationsChain3();
	void onLongOperationsChain4();
	void onLongOperationsChain5();
	void onLongOperationsChain6();
	void onLongOperationsChain7();
	void onLongOperationsChain8();
	void onLongOperationsChain9();
	void onLongOperationsChain10();
	void onLongOperationsChain11();
	void onLongOperationsChain12();
	void onLongOperationsChain13();
	void onLongOperationsChain14();
	void onLongOperationsChain15();
	void onLongOperationsChain16();
	void onLongOperationsChain17();
	void onLongOperationsChain18();
	void onLongOperationsChain19();
private:
	void loadQml();
private:
	QQmlApplicationEngine engine_;
	QObject * rootObject_;
};

#endif
