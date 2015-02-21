#ifndef SEARCHFACADE_H_
#define SEARCHFACADE_H_

#include "facade.h"

class SearchFacade: public Facade {
public:
	SearchFacade(ConnectionPtr & theConnection): Facade(theConnection) {}
	struct SearchResults {
		// ... domain specific data
	};
	SearchResults search(const QString & theSearchPhrase);
};

#endif
