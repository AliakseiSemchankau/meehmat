import random

def pow(a, d, n):
	cur = a
	res = 1
	while d > 0:
		if d % 2 == 1:
			res = (res * cur) % n
		cur = (cur * cur) % n
		d //= 2
	return res

def miller_rabin_check(a, n, s, d):
	cur = pow(a, d, n)
	if cur == 1:
		return True
	for i in range(s):
		if cur == n-1:
			return True
		cur = (cur * cur) % n
	return False

def miller_rabin_test(n, r=1):
	if n == 1:
		raise Exception("1 is neither Prime or Composite")
	s = 0
	d = n - 1
	while d % 2 == 0:
		s += 1
		d //= 2
	for i in range(r):
		a = random.randint(1, n - 1)
		if not miller_rabin_check(a, n, s, d):
			return False
	return True

def straight_test(n):
	p = 2
	while p * p <= n:
		if n % p == 0:
			return False
		p += 1
	return True

def main():
	r = 10
	for n in range(2, 10000):
		if miller_rabin_test(n, r) != straight_test(n):
			print('test failed for n = {}. its primeness is {}'.format(n, straight_test(n)))
	for n in range(10000000, 10010000):
		if miller_rabin_test(n, r) != straight_test(n):
			print('test failed for n = {}. its primeness is {}'.format(n, straight_test(n)))

	p = 10**6 + 3
	q = 10**9 + 7
	s = 10**9 + 9

	numbers = [p, q, s, s ** 2, p ** 2, q ** 2, p * q * s, p * q * s ** 3, p ** 10, p ** 5 * q ** 7 * s ** 3]

	for num in numbers:
		print('number {} is prime? {}'.format(num, miller_rabin_test(num)))


if __name__ == '__main__':
	main()
		