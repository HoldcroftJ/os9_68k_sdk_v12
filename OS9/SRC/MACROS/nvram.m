

NVRGetB macro
 ifne \#-3
  fail NVRGetB: field, base address register, destination register required
 endc
 ifndef	LWNVR
  move.b \1(\2),\3
 else
  move.l \1*4(\2),\3
 endc
 endm


NVRGetW macro
 ifne \#-4
  fail NVRGetW: field, base address reg, dest & work register required
 endc
 ifndef	LWNVR
  move.w \1(\2),\3
 else
  move.l \1*4(\2),\3
  lsl.l #8,\3
  move.l (\1+1)*4(\2),\4
  or.b \4,\3
 endc
 endm


NVRGetL macro
 ifne \#-4
  fail NVRGetL: field, base address reg, dest & work register required
 endc
 ifndef	LWNVR
  move.l \1(\2),\3
 else
  move.l \1*4(\2),\3
  lsl.l #8,\3
  move.l (\1+1)*4(\2),\4
  or.b \4,\3
  lsl.l #8,\3
  move.l (\1+2)*4(\2),\4
  or.b \4,\3
  lsl.l #8,\3
  move.l (\1+3)*4(\2),\4
  or.b \4,\3
 endc
 endm


NVRPutB macro
 ifne \#-3
  fail NVRPutB: field, base address register, destination register required
 endc
 ifndef	LWNVR
  move.b \3,\1(\2)
 else
  move.l \3,\1*4(\2)
 endc
 endm
