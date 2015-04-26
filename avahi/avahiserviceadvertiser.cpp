#include <stdexcept>
#include <avahi-common/error.h>
#include <avahi-common/malloc.h>
#include <QDebug>
#include "avahiserviceadvertiser.h"

/* AvahiServiceAdvertiser */

AvahiServiceAdvertiser::AvahiServiceAdvertiser(const QString & theServiceName, const int thePort): port_(thePort) {
	try {
		connect(this, &AvahiServiceAdvertiser::serviceAdvertisingOkInternal, 
			this, &AvahiServiceAdvertiser::serviceAdvertisingOk, Qt::QueuedConnection);
		connect(this, &AvahiServiceAdvertiser::serviceAdvertisingFailedInternal, 
			this, &AvahiServiceAdvertiser::serviceAdvertisingFailed, Qt::QueuedConnection);
		serviceName_ = avahi_strdup(theServiceName.toLocal8Bit().data());
	} catch(...) {
		release();
		throw;
	}
}

AvahiServiceAdvertiser::~AvahiServiceAdvertiser() {
	release();
}

void AvahiServiceAdvertiser::release() {
	if(isRunning()) {
		quit_ = true;

		if(!wait(10000)) {
			qDebug() << "Failed to stop advertising thread gracefully, abrupting thread";
			terminate();
		}
	}

	if(serviceName_) 
		avahi_free(serviceName_);
}

void AvahiServiceAdvertiser::stop() {
	quit_ = true;
}

void AvahiServiceAdvertiser::run() {
	qDebug() << "Advertising thread begin";
	setPriority(QThread::IdlePriority);
	AvahiSimplePoll * poll = 0;
	AvahiClient * client = 0;
	bool withError = false;
 
	try {
		// Make sure we do propagation of just created poll to a class member which will be automatically sychronized with local 'poll' var
		struct PollGuard {
			~PollGuard() { p = 0; }
			AvahiSimplePoll *& p;
		} pollGuard = { avahiPoll_ };
		pollGuard.p = poll = avahi_simple_poll_new();

		if(!poll)
			throw std::runtime_error("Failed to create simple poll");

		qDebug() << "Simple poll created";
		
		int errorCode = 0;
		client = avahi_client_new(avahi_simple_poll_get(poll), (AvahiClientFlags)0, clientCallback, this, &errorCode);

		if(!client) 
			throw std::runtime_error(QString("Failed to create client: %1").arg(avahi_strerror(errorCode)).toStdString());

		qDebug() << "Client created";

		while(!quit_) {
			const int ret = avahi_simple_poll_iterate(poll, 500);

			if(ret == 1) {
				withError = true;
				break;
			}
			else if(ret < 0)
				throw std::runtime_error(QString("Error during event processing: error code %1").arg(ret).toStdString());
		}
	} catch(const std::exception & theE) {
		qDebug() << theE.what();
		withError = true;
	}

	if(client) {
		avahi_client_free(client);
		qDebug() << "Client released";
	}

	if(poll) {
		avahi_simple_poll_free(poll);
		qDebug() << "Simple poll released";
	}

	if(withError)
		emit serviceAdvertisingFailedInternal();

	qDebug() << "Advertising thread end";
}

void AvahiServiceAdvertiser::clientCallback(AvahiClient * theClient, const AvahiClientState theState, void * theUserData) {
	AvahiServiceAdvertiser * advertiser = reinterpret_cast<AvahiServiceAdvertiser *>(theUserData);
	Q_ASSERT(theClient);
	Q_ASSERT(advertiser);

	try {
		switch(theState) {
		case AVAHI_CLIENT_S_RUNNING:
			qDebug() << "Client state AVAHI_CLIENT_S_RUNNING, server is running. Advertising service";
			advertiser->advertiseService(theClient);
			break;
		case AVAHI_CLIENT_S_COLLISION:
		case AVAHI_CLIENT_S_REGISTERING:
			if(theState == AVAHI_CLIENT_S_COLLISION)
				qDebug() << "Client state AVAHI_CLIENT_S_COLLISION, preparing for readvertising";
			else
				qDebug() << "Client state AVAHI_CLIENT_S_REGISTERING, preparing for readvertising";

			advertiser->prepareServiceForReadvertising();
			break;
		case AVAHI_CLIENT_FAILURE:
			qDebug() << QString("Client state AVAHI_CLIENT_FAILURE: %1. Closing").arg(avahi_strerror(avahi_client_errno(theClient)));
			advertiser->unadvertiseService();
			advertiser->abruptAvahiPoll();
			break;
		default: 
			qDebug() << "Client state ord." << static_cast<int>(theState);
			break;
		}
	} catch(const std::exception & theE) {
		qDebug() << QString("Failed to handle client / server state change: %1. Closing").arg(theE.what());
		advertiser->unadvertiseService();
		advertiser->abruptAvahiPoll();
	}
}

void AvahiServiceAdvertiser::groupCallback(AvahiEntryGroup * theGroup, const AvahiEntryGroupState theState, void * theUserData) {
	AvahiServiceAdvertiser * advertiser = reinterpret_cast<AvahiServiceAdvertiser *>(theUserData);
	Q_ASSERT(theGroup);
	Q_ASSERT(advertiser);

	try {	
		switch(theState) {
		case AVAHI_ENTRY_GROUP_ESTABLISHED:
			qDebug() << "Group state AVAHI_ENTRY_GROUP_ESTABLISHED. Service is successfully advertised!";
			emit advertiser->serviceAdvertisingOkInternal();
			break;
		case AVAHI_ENTRY_GROUP_COLLISION:
		case AVAHI_ENTRY_GROUP_FAILURE:
			if(theState == AVAHI_ENTRY_GROUP_COLLISION)
				qDebug() << "Group state AVAHI_ENTRY_GROUP_COLLISION. Unadvertising service";
			else {
				AvahiClient * const client = avahi_entry_group_get_client(theGroup);
				qDebug() << QString("State AVAHI_ENTRY_GROUP_FAILURE: %1. Unadvertising service").
					arg(client ? avahi_strerror(avahi_client_errno(client)) : "<failed to resolve client>");
			}

			advertiser->unadvertiseService();
			advertiser->abruptAvahiPoll();
			break;
		default: 
			qDebug() << "Group state ord." << static_cast<int>(theState);
			break;
		}
	} catch(const std::exception & theE) {
		qDebug() << QString("Failed to handle group state: %1. Closing").arg(theE.what());
		advertiser->unadvertiseService();
		advertiser->abruptAvahiPoll();
	}
}

void AvahiServiceAdvertiser::advertiseService(AvahiClient * theClient) {
	Q_ASSERT(theClient);

	try {
		if(!group_) {
			group_ = avahi_entry_group_new(theClient, groupCallback, this);

			if(!group_)
				throw std::runtime_error(QString("Failed to create entry group: %1").arg(avahi_strerror(avahi_client_errno(theClient))).toStdString());

			qDebug() << "Entry group created";
		}

		if(!avahi_entry_group_is_empty(group_)) {
			qDebug() << "Entry group is already populated";
			return;
		}

		qDebug() << QString("Advertising service \"%1\"").arg(serviceName_);
		int result = avahi_entry_group_add_service(group_, 
							   AVAHI_IF_UNSPEC, 
							   AVAHI_PROTO_UNSPEC, 
							   (AvahiPublishFlags)0, 
							   serviceName_, 
							   "_myservice._tcp", 
							   0, 
							   0, 
							   port_, 
							   serviceName_, 
							   NULL);

		if(result < 0)
			throw std::runtime_error(QString("Failed to add service \"%1\" to entry group: %2").arg(serviceName_).arg(avahi_strerror(result)).toStdString());
	
		result = avahi_entry_group_commit(group_);

		if(result < 0) 
			throw std::runtime_error(QString("Failed to commit entry group: %1").arg(avahi_strerror(result)).toStdString());

		qDebug() << "Entry group committed";
		qDebug() << "Service advertised";
	} catch(...) {
		qDebug() << "Failed to advertise service, cleaning up";
		unadvertiseService();
		throw;
	}
}

void AvahiServiceAdvertiser::prepareServiceForReadvertising() {
	if(group_) {
		avahi_entry_group_reset(group_);
		qDebug() << "Entry group resetted";
	}

	qDebug() << "Service prepared for readvertising";
}

void AvahiServiceAdvertiser::unadvertiseService() {
	if(group_) {
		avahi_entry_group_free(group_);
		qDebug() << "Entry group released";
	}

	group_ = 0;
	qDebug() << "Service unadvertised";
}

void AvahiServiceAdvertiser::abruptAvahiPoll() {
	Q_ASSERT(avahiPoll_);
	avahi_simple_poll_quit(avahiPoll_);
	qDebug() << "Avahi poll abrupted";
}
