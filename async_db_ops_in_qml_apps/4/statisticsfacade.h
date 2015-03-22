/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#ifndef STATISTICSFACADE_H_
#define STATISTICSFACADE_H_

#include "facade.h"

class StatisticsFacade: public Facade {
public:
	StatisticsFacade(ConnectionPtr & theConnection): Facade(theConnection) {}
	struct StatisticsItem {
		// ... domain specific data
	};
	void putIntoStatistics(const StatisticsItem & theItem);
};

#endif
