// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#ifndef MULTI_CHANNEL_H
#define MULTI_CHANNEL_H

#include <string>
#include <vector>
#include <map>

#include "boost/python/object.hpp"
#include "boost/python/list.hpp"
#include "boost/python/dict.hpp"

#include "pv/pvaClient.h"
#include "pv/pvaClientMultiChannel.h"

#include "PvaClient.h"
#include "CaClient.h"
#include "PvObject.h"
#include "PvProvider.h"
#include "PvaPyLogger.h"
#include "PvaConstants.h"

class MultiChannel 
{
public:
    static const double DefaultMonitorPollPeriod;

    MultiChannel(const boost::python::list& channelNames, PvProvider::ProviderType providerType=PvProvider::PvaProviderType);
    MultiChannel(const MultiChannel& multiChannel);
    virtual ~MultiChannel();

    virtual PvObject* get(const std::string& requestDescriptor);
    virtual PvObject* get();

    virtual void put(const boost::python::list& pyList);

    virtual void monitor(const boost::python::object& pySubscriber);
    virtual void monitor(const boost::python::object& pySubscriber, double pollPeriod);
    virtual void monitor(const boost::python::object& pySubscriber, double pollPeriod, const std::string& requestDescriptor);
    virtual void stopMonitor();

private:
    static void monitorThread(MultiChannel* multiChannel);
    static const double ShutdownWaitTime;

    static PvaPyLogger logger;
    static PvaClient pvaClient;
    static CaClient caClient;
    static epics::pvaClient::PvaClientPtr pvaClientPtr;

    void notifyMonitorThreadExit();
    void waitForMonitorThreadExit(double timeout);
    void callSubscriber(PvObject& pvObject);

    epics::pvaClient::PvaClientMultiChannelPtr multiChannelPtr;
    epics::pvaClient::PvaClientNTMultiMonitorPtr multiMonitorPtr;

    epics::pvData::Mutex monitorMutex;
    epicsEvent monitorThreadExitEvent;
    double monitorPollPeriod;
    bool monitorThreadRunning;
    bool monitorActive;
    boost::python::object pySubscriber;
    
};

#endif
