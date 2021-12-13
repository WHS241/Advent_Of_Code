#lang racket
(require math/number-theory)

(define input_row 3010)
(define input_col 3019)
(define input_init 20151125)
(define input_base 252533)
(define input_mod 33554393)

(define (num_it r c) (- (/ (* (- (+ r c) 1) (+ r c)) 2) r))

(define (get_password r c initial base mod) (with-modulus mod (mod* initial (modexpt base (num_it r c)))))