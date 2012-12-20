**********************************************************************
MAUI CDB validation utility
**********************************************************************
What this demo does:
  -  Checks for errors in CDB modules.

**********************************************************************
System requirements:
  - A working MAUI environment is assumed.
  - ALL of the CDB modules for this port are in memory.

*********************************************************************
Directories: 
  - Source code 
      $MWOS/SRC/MAUI/DEMOS/CDBVAL
  - Object code
      $MWOS/<OS>/<PROCESSOR>/CMDS/MAUIDEMO/cdbval

*********************************************************************
To make this demo:
  - From the source file directory type:
      % os9make

*********************************************************************
Operation: 
  - This utility allows the user three different options:

	1) "$ cdbval -d"
	   Dump out all CDB data in system memory.

	2) "$ cdbval -p"
	   Print out all registered CDB types, parameters, and values
	   that will be used when checking all CDB data in the system.

  	3) "$ cdbval -s=<modname>"
  	   Examine a single CDB module for general structure integrity
	   using the "-s=<modname>" option.

	4) "$ cdbval"
	   Examine all CDB data in the system (default behavior).

*********************************************************************
 
