(unwatch all)
(clear)
(dribble-on "Actual//dffctcmd.out")
(batch "dffctcmd.bat")
(dribble-off)
(clear)
(open "Results//dffctcmd.rsl" dffctcmd "w")
(load "compline.clp")
(printout dffctcmd "dffctcmd.bat differences are as follows:" crlf)
(compare-files "Expected//dffctcmd.out" "Actual//dffctcmd.out" dffctcmd)
(close dffctcmd)