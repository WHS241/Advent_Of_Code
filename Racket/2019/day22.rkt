#lang racket
(require math/number-theory)
(provide deal cut stack)

(define (deal f n) (with-modulus (caddr f) (list (mod* (car f) n) (mod* (cadr f) n) (current-modulus))))

(define (cut f n) (with-modulus (caddr f) (list (car f) (mod- (cadr f) n) (current-modulus))))

(define (stack f) (cut (deal f -1) 1))

(define (comp f g) (list (modulo (* (car f) (car g)) (caddr f)) (modulo (+ (* (car f) (cadr g)) (cadr f)) (caddr f)) (caddr f)))

(define (apply f n) (modulo (+ (cadr f) (* (car f) n)) (caddr f)))

(define (compound f n) (if (= n 0) (list (1 0 (cadr f)))
                           (if (= n 1)
                           f
                           (let ([doub (compound f (quotient n 2))])
                             (if (even? n)
                                 (comp doub doub)
                                 (comp (comp f doub) doub)
                                 )))))

(define (invert f) (let* ([base (caddr f)]
                         [factor (modular-inverse (car f) base)])
                     (list factor (with-modulus base (mod* (- (cadr f)) factor)) base)))