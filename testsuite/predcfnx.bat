(numberp)                          ; 10.2.1
(numberp 3 a)                      ; 10.2.1
(numberp 8)                        ; 10.2.1
(numberp 8.1)                      ; 10.2.1
(numberp "hit")                    ; 10.2.1
(floatp)                           ; 10.2.2
(floatp 3 a)                       ; 10.2.2
(floatp 8)                         ; 10.2.2
(floatp 8.1)                       ; 10.2.2
(floatp "hit")                     ; 10.2.2
(integerp)                         ; 10.2.3
(integerp 3 a)                     ; 10.2.3
(integerp 8)                       ; 10.2.3
(integerp 8.1)                     ; 10.2.3
(integerp "hit")                   ; 10.2.3
(lexemep)                          ; 10.2.4
(lexemep a 3)                      ; 10.2.4
(lexemep b)                        ; 10.2.4
(lexemep "a w")                    ; 10.2.4
(lexemep [this])                   ; 10.2.4
(stringp)                          ; 10.2.5
(stringp a 3)                      ; 10.2.5
(stringp b)                        ; 10.2.5
(stringp "a w")                    ; 10.2.5
(stringp [this])                   ; 10.2.5
(symbolp)                          ; 10.2.6
(symbolp a 3)                      ; 10.2.6
(symbolp b)                        ; 10.2.6
(symbolp "a w")                    ; 10.2.6
(symbolp [this])                   ; 10.2.6
(evenp)                            ; 10.2.7
(evenp 3 a)                        ; 10.2.7
(evenp 8)                          ; 10.2.7
(evenp 9)                          ; 10.2.7
(evenp "hit")                      ; 10.2.7
(oddp)                             ; 10.2.8
(oddp 3 a)                         ; 10.2.8
(oddp 8)                           ; 10.2.8
(oddp 9)                           ; 10.2.8
(oddp "hit")                       ; 10.2.8
(multifieldp)                      ; 10.2.9
(multifieldp (create$ x) a)        ; 10.2.9
(multifieldp (create$))            ; 10.2.9
(multifieldp (create$ a 3 4.1))    ; 10.2.9
(multifieldp "hit")                ; 10.2.9
(pointerp)                         ; 10.2.10
(pointerp 3 a)                     ; 10.2.10
(pointerp 3)                       ; 10.2.10
(pointerp x)                       ; 10.2.10
(eq)                               ; 10.2.11
(eq a)                             ; 10.2.11
(eq a a b)                         ; 10.2.11
(eq a a a)                         ; 10.2.11
(eq a a)                           ; 10.2.11
(eq a b)                           ; 10.2.11
(eq a "a")                         ; 10.2.11
(eq 3 3)                           ; 10.2.11
(eq 3 3.0)                         ; 10.2.11
(eq 3 3.1)                         ; 10.2.11
(eq a (create$ a))                 ; 10.2.11
(eq 3 (create$ a))                 ; 10.2.11
(eq a (create$ b))                 ; 10.2.11
(eq (create$ a b) (create$ c d))   ; 10.2.11
(eq (create$) (create$))           ; 10.2.11
(eq (create$ a) (create$ a))       ; 10.2.11
(eq (create$ a) (create$ b))       ; 10.2.11
(eq (create$ a b) (create$ a b))   ; 10.2.11
(eq (create$ a b) (create$ a))     ; 10.2.11
(neq)                              ; 10.2.12
(neq a)                            ; 10.2.12
(neq a a b)                        ; 10.2.12
(neq a b c)                        ; 10.2.12
(neq a a a)                        ; 10.2.12
(neq a a)                          ; 10.2.12
(neq a b)                          ; 10.2.12
(neq a "a")                        ; 10.2.12
(neq 3 3)                          ; 10.2.12
(neq 3 3.0)                        ; 10.2.12
(neq 3 3.1)                        ; 10.2.12
(neq a (create$ a))                ; 10.2.12
(neq 3 (create$ a))                ; 10.2.12
(neq a (create$ b))                ; 10.2.12
(neq (create$ a b) (create$ c d))  ; 10.2.12
(neq (create$) (create$))          ; 10.2.12
(neq (create$ a) (create$ a))      ; 10.2.12
(neq (create$ b) (create$ a))      ; 10.2.12
(neq (create$ a b) (create$ a b))  ; 10.2.12
(neq (create$ a b) (create$ a))    ; 10.2.12
(=)                                ; 10.2.13
(= 3)                              ; 10.2.13
(= 3 3)                            ; 10.2.13
(= 13.7 13.7)                      ; 10.2.13
(= 3 3.0)                          ; 10.2.13
(= 3 3.0 3)                        ; 10.2.13
(= 3 3.0 4)                        ; 10.2.13
(= 3.0 3 4)                        ; 10.2.13
(= 3 a)                            ; 10.2.13
(= 3 ?x)                           ; 10.2.13
(<>)                               ; 10.2.14
(<> 3)                             ; 10.2.14
(<> 3 3)                           ; 10.2.14
(<> 13.7 13.7)                     ; 10.2.14
(<> 3 3.0)                         ; 10.2.14
(<> 3 3.0 3)                       ; 10.2.14
(<> 3 3.0 4)                       ; 10.2.14
(<> 3 4.0 4)                       ; 10.2.14
(<> 3.0 3 4)                       ; 10.2.14
(<> 3 a)                           ; 10.2.14
(<> 3 ?x)                          ; 10.2.14
(>)                                ; 10.2.15
(> 3)                              ; 10.2.15
(> 3 3)                            ; 10.2.15
(> 3.3 3.3)                        ; 10.2.15
(> 3 3.0)                          ; 10.2.15
(> 3.0 3)                          ; 10.2.15
(> 4 3)                            ; 10.2.15
(> 3 4)                            ; 10.2.15
(> 4.1 3.3)                        ; 10.2.15
(> 3.3 4.1)                        ; 10.2.15
(> 3.1 3)                          ; 10.2.15
(> 4 3.1)                          ; 10.2.15
(> 3.4 4)                          ; 10.2.15
(> 3 4.1)                          ; 10.2.15
(> 3 4 5)                          ; 10.2.15
(> 3.0 5.3 4)                      ; 10.2.15
(> 4 3.0 5.3)                      ; 10.2.15
(> 4 3.2 2)                        ; 10.2.15
(> 4 4 3 3)                        ; 10.2.15
(> 4 4 5 5)                        ; 10.2.15
(>=)                               ; 10.2.16
(>= 3)                             ; 10.2.16
(>= 3 3)                           ; 10.2.16
(>= 3.3 3.3)                       ; 10.2.16
(>= 3 3.0)                         ; 10.2.16
(>= 3.0 3)                         ; 10.2.16
(>= 4 3)                           ; 10.2.16
(>= 3 4)                           ; 10.2.16
(>= 4.1 3.3)                       ; 10.2.16
(>= 3.3 4.1)                       ; 10.2.16
(>= 3.1 3)                         ; 10.2.16
(>= 4 3.1)                         ; 10.2.16
(>= 3.4 4)                         ; 10.2.16
(>= 3 4.1)                         ; 10.2.16
(>= 3 4 5)                         ; 10.2.16
(>= 3.0 5.3 4)                     ; 10.2.16
(>= 4 3.0 5.3)                     ; 10.2.16
(>= 4 3.2 2)                       ; 10.2.16
(>= 4 4 3 3)                       ; 10.2.16
(>= 4 4 5 5)                       ; 10.2.16
(<)                                ; 10.2.17
(< 3)                              ; 10.2.17
(< 3 3)                            ; 10.2.17
(< 3.3 3.3)                        ; 10.2.17
(< 3 3.0)                          ; 10.2.17
(< 3.0 3)                          ; 10.2.17
(< 4 3)                            ; 10.2.17
(< 3 4)                            ; 10.2.17
(< 4.1 3.3)                        ; 10.2.17
(< 3.3 4.1)                        ; 10.2.17
(< 3.1 3)                          ; 10.2.17
(< 4 3.1)                          ; 10.2.17
(< 3.4 4)                          ; 10.2.17
(< 3 4.1)                          ; 10.2.17
(< 3 4 5)                          ; 10.2.17
(< 3.0 5.3 4)                      ; 10.2.17
(< 4 3.0 5.3)                      ; 10.2.17
(< 4 3.2 2)                        ; 10.2.17
(< 4 4 3 3)                        ; 10.2.17
(< 4 4 5 5)                        ; 10.2.17
(<=)                               ; 10.2.18
(<= 3)                             ; 10.2.18
(<= 3 3)                           ; 10.2.18
(<= 3.3 3.3)                       ; 10.2.18
(<= 3 3.0)                         ; 10.2.18
(<= 3.0 3)                         ; 10.2.18
(<= 4 3)                           ; 10.2.18
(<= 3 4)                           ; 10.2.18
(<= 4.1 3.3)                       ; 10.2.18
(<= 3.3 4.1)                       ; 10.2.18
(<= 3.1 3)                         ; 10.2.18
(<= 4 3.1)                         ; 10.2.18
(<= 3.4 4)                         ; 10.2.18
(<= 3 4.1)                         ; 10.2.18
(<= 3 4 5)                         ; 10.2.18
(<= 3.0 5.3 4)                     ; 10.2.18
(<= 4 3.0 5.3)                     ; 10.2.18
(<= 4 3.2 2)                       ; 10.2.18
(<= 4 4 3 3)                       ; 10.2.18
(<= 4 4 5 5)                       ; 10.2.18
(and)                              ; 10.2.19
(and TRUE)                         ; 10.2.19        
(and TRUE 1)                       ; 10.2.19
(and TRUE FALSE)                   ; 10.2.19
(and FALSE 0)                      ; 10.2.19
(and 0 1 a)                        ; 10.2.19
(and "s" 3.4 FALSE)                ; 10.2.19
(and 0 1 TRUE a "a" [a] 3.5 (create$) (create$ a))
(or)                               ; 10.2.20
(or FALSE)                         ; 10.2.20
(or FALSE FALSE)                   ; 10.2.20
(or FALSE 1)                       ; 10.2.20
(or 0 FALSE)                       ; 10.2.20
(or FALSE a)                       ; 10.2.20
(or "s" FALSE)                     ; 10.2.20
(or FALSE 3.5)                     ; 10.2.20
(or TRUE FALSE)                    ; 10.2.20
(or FALSE TRUE)                    ; 10.2.20
(or (create$) FALSE)               ; 10.2.20
(or FALSE (create$ a b))           ; 10.2.20
(or FALSE FALSE TRUE)              ; 10.2.20
(not)                              ; 10.2.21
(not FALSE)                        ; 10.2.21
(not TRUE)                         ; 10.2.21
(not 0)                            ; 10.2.21
(not 1)                            ; 10.2.21
(not 3.5)                          ; 10.2.21
(not a)                            ; 10.2.21
(not "a")                          ; 10.2.21
(not [a])                          ; 10.2.21
(not (create$))                    ; 10.2.21
(not (create$ x y))                ; 10.2.21
(not x y)                          ; 10.2.21
