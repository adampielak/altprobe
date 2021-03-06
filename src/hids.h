/* 
 * File:   hids.h
 * Author: Oleg Zharkov
 *
 * Created on January 6, 2015, 3:34 PM
 */

#ifndef HIDS_H
#define	HIDS_H

#include "hiredis.h"

#include "sinks.h"
#include "ids.h"
#include "filters.h"
#include "config.h"
#include "source.h"

using namespace std;

class OssecRule
{
public:
    int id;  /* id attribute -- required*/
    int level;  /* level attribute --required */
    string desc; /* description in the xml */
    string info;
        
    std::vector<string> list_cats;
        
    void Reset() {
        id = 0;  /* id attribute -- required*/
        level = 0;  /* level attribute --required */
        desc.clear();
        info.clear();
                
        list_cats.clear();
    }
    
};

class OssecFile
{
public:
    string filename;
    string md5;
    string sha1;
    string sha256;
    
    void Reset() {
        filename.clear();
        md5.clear();
        sha1.clear();
        sha256.clear();
    }
    
};

//  OSSEC record                              
class OssecRecord {
public:
    /* rule that generated it */
    OssecRule rule;
    /* file info */
    OssecFile file;
    
    string ref_id;
    /* Extracted from the event */
    string location;
    string agent;
    string sensor;
    string user;
    unsigned int process_id;
    string process_name;
    
    
    /* Extracted from the decoders */
    string srcip;
    string dstip;
    unsigned int  srcport;
    unsigned int  dstport;
            
    string timestamp;    
    
    
    void Reset() {
        //reset rule class object
        rule.Reset();
        //reset rule class object
        file.Reset();
        
        ref_id.clear();
        /* Extracted from the event */
        agent.clear();
        user.clear();
        location.clear();
        sensor.clear();
        process_id = 0;
        process_name.clear();
        /* Extracted from the decoders */
        srcip.clear();
        dstip.clear();
                
        /* Additional parameters */
        timestamp.clear();
    }
};

namespace bpt = boost::property_tree;

class Hids : public Source {
public:
    
    FILE *fp;
    struct stat buf;
    unsigned long file_size;
    int ferror_counter;
    char file_payload[OS_PAYLOAD_SIZE];
    
    //OSSEC record
    OssecRecord rec;
    
    bpt::ptree pt, pt1, groups_cats, pcidss_cats, gdpr_cats, hipaa_cats, nist_cats, mitre_cats;
    stringstream ss, ss1;
    
    Hids (string skey) : Source(skey) {
        ClearRecords();
        ResetStreams();
        ferror_counter = 0;
    }
    
    void ResetStreams() {
        ss.str("");
        ss.clear();
        ss1.str("");
        ss1.clear();
    }
    
    void ResetJsontree() {
        pt.clear();
        pt1.clear();
        groups_cats.clear();
        pcidss_cats.clear();
    }
    
    int Open();
    void Close();
    int ReadFile();
    void IsFileModified();
    int Go();
    
    int ParsJson ();
    
    GrayList* CheckGrayList();
    void CreateLog();
    void SendAlert (int s, GrayList* gl);
    int PushRecord(GrayList* gl);
        
    void ClearRecords() {
	rec.Reset();
        jsonPayload.clear();
        ResetJsontree();
    }
    
};

extern boost::lockfree::spsc_queue<string> q_logs_hids;
extern boost::lockfree::spsc_queue<string> q_reports;

#endif	/* HIDS_H */

