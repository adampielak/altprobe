# Alertflex Collector (Altprobe)

Altprobe is a component of the Alertflex project. 

## Functionalities of Altprobe

* Reads events in JSON format from Suricata NIDS, Wazuh HIDS, Modsecurity WAF, Elastic Metricbeat
* Based on filtering policies, Collector retrieves high priority events from data streams created by security sensors, makes aggregation and normalization for these events. This allows to simplify the management of alerts and incidents, reduces noise from minor events.
* High priority events (alerts) are immediately sent to the central node.
* All log events, host metrics, statistics are sent to the Controller inside of pre-accumulated and compressed data set, this implements the "Anti-flooding" algorithm to prevent large bursts of events on the controller side.
* Alerts and log events (NetFlow, DNS and SSH sessions, minor priority events) can be redirected to the Log Management platform via Controller
* The Collector saves various statistics about IDS and NetFlow events and send it to the Controller.
* In case of loss of communication between remote and central nodes, the Collector saves all alerts locally in a file
* Altprobe generates alerts if the network traffic thresholds have been reached the limits (Users can configure the thresholds inside filtering policies of the collector)
* Generates alerts if the metrics thresholds for hosts (CPU, Memory, HDD, Swap) have been reached the limits
* Creates reports about network activities of application processes (based on events from Sysmon for Windows and Auditd for Linux). This allows determining the name of the process associated with suspicious network connections.

## Type of events, which are generated by Altprobe

* Alert from Alertflex collector/controller

* Alert from OSSEC/Wazuh HIDS

* Alert from OSSEC/Wazuh FIM

* Alert from Suricata NIDS

* DNS event from Suricata NIDS

* SSH event from Suricata NIDS

* Netflow event from Suricata NIDS

* Alert from ModSecurity WAF

* Network activity of linux process from Auditd

* Network activity of windows process from Sysmon

## Screenshots
In tandem with Alertflex Controller, Altprobe can integrate a Wazuh Host IDS (OSSEC fork) and Suricata Network IDS with log management platform Graylog and threat intelligence platform MISP. Below, screenshot of Graylog web forms for events from Altprobe

![](https://github.com/olegzhr/altprobe/blob/master/img/graylog.jpg)

## Requirements

Altprobe was tested under Ubuntu version 16.04 with Wazuh HIDS (OSSEC fork) version 3.2, ModSecurity 3.0 and Nginx, Suricata NIDS version 4.0.3

## Installation instructions

Create user ``alertflex`` and log in under this user

Download installation files

    $ git clone git://github.com/olegzhr/Altprobe.git
    cd ./Altprobe

Fill in collector specific parameters in file ``env.sh`` and start installation		
    
    $ chmod u+x install_ubuntu16.sh
    ./install_ubuntu16.sh
    
After the finish of installation Altprobe, make a copy of file ``Broker.pem`` to dir /etc/alertflex/ of Collector node from Controller node install dir (file Broker.pem should be created automatically during installation of Controller).

NOTE:

* Installation and configuration of IDS OSSEC/Wazuh and Suricata IDS are not parts of Alertflex solution. Although install script for Altprobe includes these procedures, it comes with NO WARRANTY!

* For enabling events from Sysmon via Wazuh IDS, please, change level of ``rule_id 185001`` instead 0  to other value. See file ``/var/ossec/ruleset/rules0330-sysmon_rules.xml``

* For enabling an network activities events from Auditd, please, use the command: ``auditctl -a exit,always -F arch=b64 -S connect -k linux-connects``, key value ``linux-connects`` is important!

* For integration Metricbeat with Altprobe 

in the file ``/etc/metricbeat/metricbeat.yml`` set next parameters:

    $ output.redis:
    hosts: ["altprobe_host_or_ip"]
    key: "altprobe_metrics"

## Post-install

For advanced configuration of Altprobe use altprobe config file and filtering policies, see samples in dir: [/src/etc/](https://github.com/olegzhr/Altprobe/blob/master/src/etc/)

Below some useful parameters of filtering policies:
* for recognition IP address space, please describers your network in home_net, if sub-parameter "home_net.alert_suppress" is true, alerts from this network will not be sent to the controller (it is useful for suppressing alerts generated by scan tools such as Nmap

* a sub-parameter "sources.type_of_source.log", if it is true, a log info from the source, will be sent to the controller

* a sub-parameter "sources.type_of_sensor.severity.threshold" if the severity of alert >= a value of the parameter, the alert will be sent to the controller
   
	
## Troubleshooting

Collector CLI interface:

    $ altprobe-start
    altprobe-stop
    altprobe-restart
    altprobe-status

for example:

    $ root@host:~# altprobe-status
    alertflex collector isn't running

    suricata start/running, process 1797

    ossec-monitord is running...
    ossec-logcollector is running...
    ossec-remoted is running...
    ossec-syscheckd is running...
    ossec-analysisd is running...
    ossec-maild not running...
    ossec-execd is running...
    wazuh-modulesd is running...

    root@host:~# altprobe-start
    alertflex collector started with code 0
    root@host:~#

    root@host:~# altprobe-status
	
    alertflex collector is running, process 19023

    suricata start/running, process 1797

    ossec-monitord is running...
    ossec-logcollector is running...
    ossec-remoted is running...
    ossec-syscheckd is running...
    ossec-analysisd is running...
    ossec-maild not running...
    ossec-execd is running...
    wazuh-modulesd is running...

how to see a collector errors:

    $ cat /var/log/syslog | grep altprobe


## Support

Please [open an issue on GitHub](https://github.com/olegzhr/altprobe/issues), if you'd like to report a bug or request a feature 


## Old version of Altprobe 

Previous version of altprobe (single package with support Ntop nProbe, ZeroMQ as sources and output to MySQL) is available under branch ``old_version``


