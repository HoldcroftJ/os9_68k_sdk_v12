
********************
* OS9svc macro 
*   Direct call to OS9 service routine.
* one argument: system call code, (a6)=system global ptr
*
* WARNING:  using this call generally requires intimate knowledge
*           of how the kernel works.  It is generally useful only
*           for calls that do not return parameters (e.g. OS9 F$Send,
*           OS9 F$Sleep[0]).

OS9svc macro
 ifne \#-1  must have exactly one argument
   fail wrong number of arguments to OS9svc macro
 endc
 move.l a3,-(a7) save reg
 movea.l D_SysDis(a6),a3 get system dispatch tbl ptr
 pea OS9svc\@(pc) set return address
 move.l \1+\1+\1+\1(a3),-(a7) set OS9 service routine ptr
 movea.l \1+\1+\1+\1+256*4(a3),a3 get service routine data ptr
 rts execute service routine
OS9svc\@ movea.l (a7)+,a3 restore reg
 endm

