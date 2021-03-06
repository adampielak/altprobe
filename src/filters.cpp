/* 
 * File:   filters.h
 * Author: Oleg Zharkov
 */
#include <sys/socket.h>
#include "filters.h"

namespace bpt = boost::property_tree;

// FS states
int FiltersSingleton::status = 0;

Filters FiltersSingleton::filter;
std::vector<Agent> FiltersSingleton::agents_list;

int FiltersSingleton::GetFiltersConfig() {
    
    if (!status) {
        
        ifstream filters_config;
        filters_config.open(FILTERS_FILE);
        stringstream strStream;
        strStream << filters_config.rdbuf();
        filters_config.close();
        
        if (ParsFiltersConfig(strStream.str())) {
            status = 1;
            return status;
        }
    
        SysLog("filters file error: error parsing config data\n");
        return 0;
    }
    
    return 1;
    
}

boost::shared_mutex FiltersSingleton::filters_update;

int FiltersSingleton::ParsFiltersConfig(string f) {
    
    boost::unique_lock<boost::shared_mutex> lock(filters_update);
    
    filter.Reset();
    
    try {
                
        stringstream ss(f);
        
        string id;
        bpt::ptree pt;
        bpt::read_json(ss, pt);
        
        filter.ref_id =  pt.get<string>("ref_id");
        filter.desc =  pt.get<string>("filter_desc");
                
        bpt::ptree home_networks = pt.get_child("home_net");
        BOOST_FOREACH(bpt::ptree::value_type &h_nets, home_networks) {
            
            Network* net = new Network();
            
            net->network = h_nets.second.get<string>("node");
            net->network = h_nets.second.get<string>("network");
            net->netmask = h_nets.second.get<string>("netmask");
            net->alert_suppress = h_nets.second.get<bool>("alert_suppress");
            
            filter.home_nets.push_back(net);
        }
        
        bpt::ptree name_alias = pt.get_child("alias");
        BOOST_FOREACH(bpt::ptree::value_type &n_alias, name_alias) {
            
            Alias* al = new Alias;
            
            al->agent_name = n_alias.second.get<string>("agent");
            al->host_name = n_alias.second.get<string>("host");
            al->container_name = n_alias.second.get<string>("container");
            al->ip = n_alias.second.get<string>("ip");
                        
            if (al->ip.compare("indef") == 0) {
                
                struct hostent *hostaddr = gethostbyname(al->host_name.c_str());
        
                if (hostaddr != NULL) {
                    
                    al->ip = inet_ntoa(*(struct in_addr *)hostaddr->h_addr_list[0]);
                    filter.alias.push_back(al);
                    
                }
            }
            else filter.alias.push_back(al);
        }
        
        bpt::ptree filters = pt.get_child("sources");
        
        // CRS
        filter.crs.log = filters.get<bool>("crs.log");
        filter.crs.severity.threshold = filters.get<int>("crs.severity.threshold");
        filter.crs.severity.level0 = filters.get<int>("crs.severity.level0");
        filter.crs.severity.level1 = filters.get<int>("crs.severity.level1");
        filter.crs.severity.level2 = filters.get<int>("crs.severity.level2");
              
        bpt::ptree crs_gray_list = filters.get_child("crs.gray_list");
        BOOST_FOREACH(bpt::ptree::value_type &crs_list, crs_gray_list) {
            
            GrayList* gl = new GrayList();
            
            gl->event = crs_list.second.get<string>("event");
            gl->host = crs_list.second.get<string>("container");
            gl->match = crs_list.second.get<string>("match");
            
            gl->agr.reproduced = crs_list.second.get<int>("aggregate.reproduced");  
            gl->agr.in_period = crs_list.second.get<int>("aggregate.in_period");  
            
            gl->rsp.profile = crs_list.second.get<string>("response.profile");
            gl->rsp.new_type = crs_list.second.get<string>("response.new_type");  
            gl->rsp.new_source = crs_list.second.get<string>("response.new_source");    
            gl->rsp.new_event = crs_list.second.get<string>("response.new_event");           
            gl->rsp.new_severity = crs_list.second.get<int>("response.new_severity");
            gl->rsp.new_category = crs_list.second.get<string>("response.new_category");
            gl->rsp.new_description = crs_list.second.get<string>("response.new_description");
            
            filter.crs.gl.push_back(gl);
        }
        
        // HIDS
        filter.hids.log = filters.get<bool>("hids.log");
        filter.hids.severity.threshold = filters.get<int>("hids.severity.threshold");
        filter.hids.severity.level0 = filters.get<int>("hids.severity.level0");
        filter.hids.severity.level1 = filters.get<int>("hids.severity.level1");
        filter.hids.severity.level2 = filters.get<int>("hids.severity.level2");
              
        bpt::ptree hids_gray_list = filters.get_child("hids.gray_list");
        BOOST_FOREACH(bpt::ptree::value_type &hids_list, hids_gray_list) {
            
            GrayList* gl = new GrayList();
            
            gl->event = hids_list.second.get<string>("event");
            gl->host = hids_list.second.get<string>("agent");
            gl->match = hids_list.second.get<string>("match");
            
            gl->agr.reproduced = hids_list.second.get<int>("aggregate.reproduced");  
            gl->agr.in_period = hids_list.second.get<int>("aggregate.in_period");  
            
            gl->rsp.profile = hids_list.second.get<string>("response.profile");
            gl->rsp.new_type = hids_list.second.get<string>("response.new_type");  
            gl->rsp.new_source = hids_list.second.get<string>("response.new_source");    
            gl->rsp.new_event = hids_list.second.get<string>("response.new_event");           
            gl->rsp.new_severity = hids_list.second.get<int>("response.new_severity");
            gl->rsp.new_category = hids_list.second.get<string>("response.new_category");
            gl->rsp.new_description = hids_list.second.get<string>("response.new_description");
            
            filter.hids.gl.push_back(gl);
        }
        
        // NIDS
        filter.nids.log = filters.get<bool>("nids.log");
        filter.nids.severity.threshold = filters.get<int>("nids.severity.threshold");
        filter.nids.severity.level0 = filters.get<int>("nids.severity.level0");
        filter.nids.severity.level1 = filters.get<int>("nids.severity.level1");
        filter.nids.severity.level2 = filters.get<int>("nids.severity.level2");
               
        bpt::ptree nids_gray_list = filters.get_child("nids.gray_list");
        BOOST_FOREACH(bpt::ptree::value_type &nids_list, nids_gray_list) {
            
            GrayList* gl = new GrayList();
            
            gl->event = nids_list.second.get<string>("event");
            gl->host = nids_list.second.get<string>("host");
            gl->match = nids_list.second.get<string>("match");
                        
            gl->agr.reproduced = nids_list.second.get<int>("aggregate.reproduced");  
            gl->agr.in_period = nids_list.second.get<int>("aggregate.in_period");  
            
            gl->rsp.profile = nids_list.second.get<string>("response.profile");
            gl->rsp.new_type = nids_list.second.get<string>("response.new_type");  
            gl->rsp.new_source = nids_list.second.get<string>("response.new_source");    
            gl->rsp.new_event = nids_list.second.get<string>("response.new_event");           
            gl->rsp.new_severity = nids_list.second.get<int>("response.new_severity");
            gl->rsp.new_category = nids_list.second.get<string>("response.new_category");
            gl->rsp.new_description = nids_list.second.get<string>("response.new_description");
            
            filter.nids.gl.push_back(gl);
        }
        
        // WAF
        filter.waf.log = filters.get<bool>("waf.log");
        filter.waf.severity.threshold = filters.get<int>("waf.severity.threshold");
        filter.waf.severity.level0 = filters.get<int>("waf.severity.level0");
        filter.waf.severity.level1 = filters.get<int>("waf.severity.level1");
        filter.waf.severity.level2 = filters.get<int>("waf.severity.level2");
               
        bpt::ptree waf_gray_list = filters.get_child("waf.gray_list");
        BOOST_FOREACH(bpt::ptree::value_type &waf_list, waf_gray_list) {
            
            GrayList* gl = new GrayList();
            
            gl->event = waf_list.second.get<string>("event");
            gl->host = waf_list.second.get<string>("host");
            gl->match = waf_list.second.get<string>("match");
            
            gl->agr.reproduced = waf_list.second.get<int>("aggregate.reproduced");  
            gl->agr.in_period = waf_list.second.get<int>("aggregate.in_period");  
            
            gl->rsp.profile = waf_list.second.get<string>("response.profile");
            gl->rsp.new_type = waf_list.second.get<string>("response.new_type");  
            gl->rsp.new_source = waf_list.second.get<string>("response.new_source");    
            gl->rsp.new_event = waf_list.second.get<string>("response.new_event");           
            gl->rsp.new_severity = waf_list.second.get<int>("response.new_severity");
            gl->rsp.new_category = waf_list.second.get<string>("response.new_category");
            gl->rsp.new_description = waf_list.second.get<string>("response.new_description");
            
            filter.waf.gl.push_back(gl);
        }
        
        pt.clear();
        
    } catch (const std::exception & ex) {
        SysLog((char*) ex.what());
        return 0;
    } 
    
    return 1;
}


boost::shared_mutex FiltersSingleton::agents_update;

void FiltersSingleton::UpdateAgentsList(string id, string ip, string name, string status, 
    string date, string version, string manager, string os_platf, string os_ver, string os_name) {
    
    boost::unique_lock<boost::shared_mutex> lock(agents_update);
    
    std::vector<Agent>::iterator i, end;   
    
    string real_ip = "indef";
    
    if (IsValidIp(ip) == -1 || ip.compare("127.0.0.1") == 0) {
        
        Alias* al = GetAliasByAgentName(name);
            
        if (al != NULL) {
            
            if (al->ip.compare("indef") != 0) real_ip = al->ip;
            else {    
                if (al->host_name.compare("indef") != 0) {
                
                    string host_name = al->host_name;
            
                    struct hostent *hostaddr = gethostbyname(host_name.c_str());
        
                    if (hostaddr != NULL) real_ip = inet_ntoa(*(struct in_addr *)hostaddr->h_addr_list[0]);
                }
            }
            
        }
    }
    else real_ip = ip;
    
    
    for (i = agents_list.begin(), end = agents_list.end(); i != end; ++i) {
        if (i->name.compare(name) == 0) {
            i->id = id;
            i->ip = real_ip;
            i->dateAdd = date;
            i->manager_host = manager;
            i->os_name = os_name;
            i->os_platform = os_platf;
            i->os_version = os_ver;
            i->version = version;
            i->status = status;
            return;
        }
    }
    
    agents_list.push_back(Agent(id, ip, name, status, date, version, manager, os_platf, os_ver, os_name));
    
}

string FiltersSingleton::GetAgentNameByIP(string ip) {
    
    boost::shared_lock<boost::shared_mutex> lock(agents_update);
    
    std::vector<Alias*>::iterator i_al, end_al;
    
    for(i_al = filter.alias.begin(), end_al = filter.alias.end(); i_al != end_al; ++i_al) {
        if ((*i_al)->ip.compare(ip) == 0) {
            return (*i_al)->agent_name; 
        }
    }
    
    std::vector<Agent>::iterator i_ag, end_ag;
    
    for(i_ag = agents_list.begin(), end_ag = agents_list.end(); i_ag != end_ag; ++i_ag) {
        if (i_ag->ip.compare(ip) == 0) {
            return i_ag->name;
        }
    }
    
    return "indef";
}

string FiltersSingleton::GetAgentIdByName(string name) {
    
    std::vector<Agent>::iterator i_ag, end_ag;
    
    for(i_ag = agents_list.begin(), end_ag = agents_list.end(); i_ag != end_ag; ++i_ag) {
        if (i_ag->name.compare(name) == 0) {
            return i_ag->id;
        }
    }
    
    
    return "";
}

Alias* FiltersSingleton::GetAliasByAgentName(string name) {
    
    std::vector<Alias*>::iterator i, end;
    
    for(i = filter.alias.begin(), end = filter.alias.end(); i != end; ++i) {
        if ((*i)->agent_name.compare(name) == 0) {
             return (*i); 
        } 
    }
    
    return NULL;
}


