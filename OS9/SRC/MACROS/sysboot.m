********************
* sysboot "C-glue" macro 
*   Glue for assembler routines to be used with 'C' technology "sysboot"
* one argument: function name
*
sysboot MACRO
 ifne \#-1  must have exactly one argument
 fail wrong number of arguments to "sysboot" macro
 endc
 ifdef CBOOT
\1: dc.l $0515B007
 else
SysBoot: equ *
\1: equ *
 endc
 endm
