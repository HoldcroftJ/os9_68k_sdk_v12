cd ..\..\MAKETMPL\INSTALL
copy currmdos.tpl ..\currhost.tpl
copy /B alltrgts.beg+allt*.com+alltrgts.end ..\alltrgts.tpl
copy /B instrgts.beg+inst*.com+instrgts.end ..\instrgts.tpl
cd ..\..\DOS\BIN
