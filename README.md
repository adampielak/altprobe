# Altprobe

Altprobe is a component of the Alertflex project, it has functional of a collector according to SIEM/Log Management terminologies. 

## Advantages of using Altprobe
* Based on filtering policies, Altprobe extracts events with high priority from flows of data generated by Wazuh HIDS and Suricata NIDS, makes for these events aggregation and normalization. It allows to simplify alerts and incidents management, reduces noise from a minor events.
* Threat hunting based on Indicator of Compromise from The Open Source Threat Intelligence Platform MISP. Alertflex controller generates alert in case of matching an IOC with IDS event data.
* Active response (blocking of a suspicious source or destination IP via firewalld) based on the local collector filtering policies or based on a remote message from the controller if IP match to IOC from MISP DB. So, an on-premises node can work as a security gateway.
* Reports about a network activities and IP addresses reputation for processes on a hosts (based on events from Sysmon for Windows and Auditd for Linux, received via Wazuh IDS). It is useful for finding of process name connected with suspicious network connections.
* Recognition a Host IDS agents name space inside Alerts and Netflow events, generated by Network IDS 
* SSL protocol with two-way authentication is used for secure of connections between remote and central nodes
* All logs are sent in an accumulated state inside compressed data blocks to prevents of events overflow
* In a case of loss of connection between remote and central nodes, the collector persists all alerts locally in file

## Integration with GrayLog and MISP
In tandem with Alertflex controller (see AlertflexCtrl repository on this GitHub profile), 
Altprobe can integrate a Wazuh Host IDS (OSSEC fork) and Suricata Network IDS
with Log Management platform Graylog and Threat Intelligence Platform MISP. 

Below, a diagram of configuration Altprobe and Alertflex controller for working with GrayLog and MISP

![](https://github.com/olegzhr/altprobe/blob/master/img/arch.png)

## Type of events (GELF format), that are generated by Altprobe

* "short_message":"alert-flex", "full_message":"Alert from Alertflex collector/controller"

* "short_message":"alert-hids", "full_message":"Alert from OSSEC/Wazuh HIDS"

* "short_message":"alert-fim", "full_message":"Alert from OSSEC/Wazuh FIM"

* "short_message":"alert-nids", "full_message":"Alert from Suricata NIDS"

* "short_message":"dns-nids", "full_message":"DNS event from Suricata NIDS"

* "short_message":"ssh-nids", "full_message":"SSH event from Suricata NIDS"

* "short_message":"netflow-nids", "full_message":"Netflow event from Suricata NIDS"

* "short_message":"alert-waf", "full_message":"Alert from ModSecurity/NGINX"

* "short_message":"process-linux", "full_message":"Network activity of linux process from Auditd"

* "short_message":"process-win", "full_message":"Network activity of windows process from Sysmon"

## Documentation (early version, include an installation instructions)
see web page: <http://alertflex.org/doc/>

NOTE:
For enabling an events from Sysmon via Wazuh IDS, please, change level of ``rule_id 185001`` instead 0  to other value.

For enabling an network activities events from Auditd, please, use the command: 
``auditctl -a exit,always -F arch=b64 -S connect -k linux-connects``,
key value ``linux-connects`` is important!

For advanced configuration of Altprobe, please, see file: [filters.json](https://github.com/olegzhr/Altprobe/blob/master/src/etc/filters.json)

## Screenshots
Below, a screenshots of Graylog dashboards for IDS events from Altprobe

![](https://github.com/olegzhr/altprobe/blob/master/img/graylog1.jpg)
Normalized and aggregated alerts from Host and Network IDS

![](https://github.com/olegzhr/altprobe/blob/master/img/graylog2.jpg)
Simple statistics about IDS alerts categories, applications protocols and Geo IP netflow map

## Requirements
Altprobe was tested under Ubuntu version 14.04 with Wazuh HIDS (OSSEC fork) version 3.2 and Suricata NIDS version 4.0.3

## Support
Please [open an issue on GitHub](https://github.com/olegzhr/altprobe/issues) or send an email to <info@alertflex.org>,
if you'd like to report a bug or request a feature 


## Old version of Altprobe 
Previous version of altprobe (single package with support Ntop nProbe, ZeroMQ as sources and output to MySQL) is available under branch ``old_version``


