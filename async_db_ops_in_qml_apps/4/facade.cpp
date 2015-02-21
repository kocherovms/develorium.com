#include "facade.h"

template<>
void extractResult(QFutureWatcher<void> & theFw) { 
	theFw.waitForFinished(); 
}
