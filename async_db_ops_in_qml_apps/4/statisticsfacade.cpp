/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include <unistd.h>
#include "statisticsfacade.h"

void StatisticsFacade::putIntoStatistics(const StatisticsItem & theItem) {
	Q_UNUSED(theItem);
	// Emulate some heavy SQL(s) here
	sleep(3);
}
