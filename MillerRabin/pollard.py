from fractions import gcd
from miller_rabin import *

def square(n, mod):
	return (n * n - 1) % mod

def pollard(n, func = square):
	for p in [2, 3, 5, 7, 11, 13, 17, 19]:
		if n % p == 0:
			return p
	if miller_rabin_test(n, 10):
		return n
	xi = 2
	xii = square(xi, n)
	Q = (xii - xi + n) % n
	for i in range(1, n):
		xi = square(xi, n)
		xii = square(square(xii, n), n)
		Q = (((xii - xi + n) % n ) * Q) % n
		d = gcd(Q, n)
		if d > 1 and d < n:
			return pollard(d)
	return n

def main():
	while True:
		p = int(input())
		d = pollard(p)
		if d == 1 or d == p:
			print("{} is prime".format(p))
		else:
			print("{} = {} x {}".format(p, d, p // d))

if __name__ == '__main__':
	main()

