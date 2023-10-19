/* Recursion.h - Implementation for examples on recursion from lecture
 *
 * Copyright 2019 - Michael Galle
 *
 */
#include <stdio.h>


 // Recursive Add example
int RecursiveAdd(int iNumber) {
	int nextNumber;
	if (iNumber <= 1) {
		return(1);		// Termination condition
	} else {
		nextNumber = iNumber - 1;
		return (iNumber + RecursiveAdd(nextNumber));
	}
}

// Factorial example
int factorial(int N) {
	if (N == 0) return(1);
	return(N * factorial(N - 1));
}

// Greatest common denominator
int gcd(int m, int n) {
	if (n == 0) return(m);
	return(gcd(n, m % n));
}

