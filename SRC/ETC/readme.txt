
These files are samples of the various databases used by the OS-9/OS-9000 ISP, 
NFS, and OS-9 ESP software.  They need to be processed by the IDBGEN utility 
to generate the "inetdb" data module.  Following is a brief discussion of 
each file.  For more details see the appropriate section of the OS-9 or 
OS-9000 ISP, NFS, or OS-9 ESP manual for detailed information.


HOST.CONF:
	Not currently implemented. 
	Options for the name resolution algorithms.  These 
	options control the manor in which host names are resolved.  This file 
	should be updated to reflect the type of host name resolution desired on 
	the target sytem.

HOSTS:
	Local list of host names known to your system.  If you are not using 
	the resolve trap handler/subroutine module or library, you will need to 
	add an entry for each of hosts (including itself) to the file.

HOST.EQUIV:
	Equivalent hosts, security-wise.  For NFS use.  Not currently implemented.

INETD.CONF:
	Internet daemon (inetd) services list indicating the service to be 
	provided, the corresponding socket type and protocol to be used for 
	this service, the user under which to run this service, and the path 
	to the program to provide the service and any arguements for that 
	program.

NETWORKS:
	List of networks names analogous to the HOSTS files.  It should be 
	updated to include any local network names.

PROTOCOLS:
	Locally defined protocol names and their corresponding protocol numbers.  
	This file probably does not need to be changed.

RESOLV.CONF:
	General host name resolution configurations parameters, including local 
	domain name, nameserver lists, and domain search lists.

SERVICES:
	Locally defined internet services and their corresponding 
	port number / protocol name bindings.  This file probably does not 
	need to be changed unless additional local services are being added 
	which need to be named.

ROUTES.CONF
	(SPF only): Define list of static routes to be configured on the system.

INTERFACES.CONF
	(SPF only): Define hostname and list of interfaces to be initialized
	when the stack is brought up.
