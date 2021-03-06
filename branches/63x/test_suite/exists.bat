(clear)      ; Test exists
(watch activations)
(watch facts)

(defrule test
   (exists (a ?))
   =>)
(reset)
(assert (a 1))
(assert (a 2))
(assert (a 3))
(retract 1)
(retract 3)
(retract 2)
(assert (a 4))
(clear)

(defrule test
   (a ?x)
   (exists (b ?x ?))
   =>)
(reset)
(assert (b 1 1))
(assert (b 1 2))
(assert (a 1))
(assert (a 2))
(retract 1)
(retract 2)
(assert (b 2 2))
(assert (b 1 2))
(assert (b 2 3))
(assert (b 1 3))
(retract 7)
(retract 5)
(retract 3)
(unwatch facts)
(unwatch activations)
(clear)
(defrule foo (a) (exists (a) (a)) =>)
(reset)
(assert (a))
(agenda)
(retract 1)
(agenda)
(clear) ; 6.3 Issue
(unwatch all)   

(defrule detect
   (b ?b)
   (exists (a))
   (test (eq ?b 2))
   =>
   (printout t "Rule Fired" crlf))
(assert (a))
(assert (b 1))
(run)
(reset)
(assert (b 1))
(assert (a))
(run)
(reset)
(assert (a))
(assert (b 2))
(run)
(reset)
(assert (b 2))
(assert (a))
(run)
(clear) ; 6.3 Issue

(deffacts xy
    (SAD T 1 T07 "12345678"))

(defrule x
   (exists (SAD T ?ix T07 ?str)
       (test (>= (str-length ?str) 0))) 
   (test (=  1 1))
   =>)
(reset)
(agenda)
(clear)
