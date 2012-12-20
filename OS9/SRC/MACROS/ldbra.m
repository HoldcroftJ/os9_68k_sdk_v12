********************************
* ldbra macro - extend dbra loop to long register count
* arguments: \1=Dn, register to decrement
*            \2=(short) destination address
ldbra macro
 dbra \1,\2
 addq.w #1,\1
 subq.l #1,\1
 bcc.s \2
 endm
