(unwatch all)
(clear)
(setgen 1)
(load "instance.clp")
(dribble-on "Actual//instance.out")
(batch "instance.bat")
(dribble-off)
(clear)
(open "Results//instance.rsl" instance "w")
(load "compline.clp")
(printout instance "instance.clp differences are as follows:" crlf)
(compare-files "Expected//instance.out" "Actual//instance.out" instance)
(close instance)