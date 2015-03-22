/*

Copyright Michael Kocherov, 2015
http://develorium.com

*/
#include "facade.h"

template<>
void extractResult(QFutureWatcher<void> & theFw) { 
	theFw.waitForFinished(); 
}
