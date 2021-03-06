/* 
 * File:   main.h
 * Author: Oleg Zharkov
 *
 */

#ifndef MAIN_H
#define	MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <execinfo.h>
#include <wait.h>
#include <wchar.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <signal.h>
#include <yaml.h>
#include <netdb.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BOOST_SPIRIT_THREADSAFE
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <memory>

/* Size limit control */
#define OS_PAYLOAD_SIZE      20480    /* Size for logs, sockets, etc */
#define OS_MAXSTR_SIZE       10240    /* Size for logs, sockets, etc */
#define OS_LONG_BUFFER_SIZE  2048    /* Size of long  buffers */
#define OS_BUFFER_SIZE       1024    /* Size of general buffers */
#define OS_STRING_SIZE       512     /* Comment */
#define OS_LONG_HEADER_SIZE  256     /* Maximum log header size */
#define OS_HEADER_SIZE       128     /* Maximum header size */
#define OS_UUID_SIZE         37      /* DATETIME size */
#define OS_DATETIME_SIZE     32      /* DATETIME size */
#define MAC_SIZE             20      /* MAC size */
#define IP_SIZE              32      /* IP Address size */
#define PORT_SIZE            8       /* Port Address size */

#define DELIM "."

#define MSG_VERSION 0

#define ZDATALEN 1024 * 1024

#define BLACKLIST_SIZE       100

#define LOG_QUEUE_SIZE 200000
#define AGG_QUEUE_SIZE 200000
#define STAT_QUEUE_SIZE 200000
#define NET_QUEUE_SIZE 200000
#define IDS_QUEUE_SIZE 200000

#define CONFIG_FILE "/etc/altprobe/altprobe.yaml"
#define FILTERS_FILE "/etc/altprobe/filters.json"
#define PID_FILE "/var/run/altprobe.pid"
#define DAEMON_NAME "altprobe"

#define OSSEC_CONFIG "ossec.conf"
#define SURI_CONFIG "suricata.yaml"
#define MODSEC_CONFIG "main.conf"
#define FALCO_CONFIG "falco.yaml"

#define WAZUH_IPREP "hids-black.list"
#define SURI_IPREP "nids-black.list"
#define MODSEC_IPREP "lists/waf-black.list"

#define WAZUH_RULES "rules/"
#define WAZUH_DECODERS "decoders/"

#define EOF_COUNTER 100

#endif	/* MAIN_H */