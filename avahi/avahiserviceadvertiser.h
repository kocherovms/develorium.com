#ifndef AVAHISERVICEADVERTISER_H_
#define AVAHISERVICEADVERTISER_H_

#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-common/simple-watch.h>
#include <QThread>

class AvahiServiceAdvertiser: public QThread {
	Q_OBJECT
public:
	AvahiServiceAdvertiser(const QString & theServiceName, const int thePort);
	~AvahiServiceAdvertiser();
private:
	void release();
public:
	void stop();
signals:
	void serviceAdvertisingOk();
	void serviceAdvertisingFailed();
	void serviceAdvertisingOkInternal();
	void serviceAdvertisingFailedInternal();
protected:
	virtual void run();
private:
	static void clientCallback(AvahiClient * theClient, const AvahiClientState theState, void * theUserData);
	static void groupCallback(AvahiEntryGroup * theGroup, const AvahiEntryGroupState theState, void * theUserData);
	void advertiseService(AvahiClient * theClient);
	void prepareServiceForReadvertising();
	void unadvertiseService();
	void abruptAvahiPoll();
private:
	char * serviceName_ = 0;
	const int port_;
	AvahiSimplePoll * avahiPoll_ = 0;
	AvahiEntryGroup * group_ = 0;
	volatile bool quit_ = false;
	Q_DISABLE_COPY(AvahiServiceAdvertiser)
};

#endif
