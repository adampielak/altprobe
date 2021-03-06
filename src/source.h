/* 
 * File:   source.h
 * Author: Oleg Zharkov
 *
 */

#ifndef SOURCE_H
#define	SOURCE_H

#include <mutex>

#include "hiredis.h"
#include "sinks.h"
#include "ids.h"
#include "filters.h"
#include "config.h"

using namespace std;

class Source : public CollectorObject {
public:
    string sensor;
    
    int status;
    int redis_status;
    string config_key;
    string redis_key;
    static int config_flag;
    
    std::mutex m_monitor_counter;
    unsigned long events_counter;
    unsigned long alerts_counter;
    
    redisReply *reply;
    redisContext *c;
    
    //JSON strings for regex and log output 
    string jsonPayload;
    string report;
    
    // interfaces
    Sinks sk;
    FiltersSingleton fs;
    
    Source () {
        config_key = "indef";
        status = 1;
        redis_status = 1;
        events_counter = 0;
        alerts_counter = 0;
    }
    
    Source (string ckey) {
        config_key = ckey;
        status = 1;
        redis_status = 1;
        events_counter = 0;
        alerts_counter = 0;
    }
    
    virtual int GetConfig();
    
    virtual int GetStatus() {
        return status;
    }
    
    long ResetEventsCounter();
    void IncrementEventsCounter();
    void SendAlertMultiple(int type);
    string GetAgent(string ip);
    bool SuppressAlert(string ip);
    
};

#endif	/* SOURCE_H */

