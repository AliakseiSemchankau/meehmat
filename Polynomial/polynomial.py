from fractions import Fraction
import itertools 

def x_to_n(n): # returns x^n
	return Polynomial([0 for i in range(n)] + [1])

class Polynomial(object):

	def __init__(self, coefficients):
		self.coefficients = [Fraction(c) for c in coefficients]
		# polynomial is interpreted as c_0 + c_1 * x + c_2 * x * x + ...
		if len(self.coefficients) == 0:
			self.coefficients = [0]
		self.update() # if highest coefficients are zeros then reduce actual degree of polynomial

	def is_zero(self): 
		return len(self.coefficients) == 1 and self.coefficients[0] == 0

	def __repr__(self):
		s = ""
		for d, c in enumerate(self.coefficients):
			# format is: {+-} {coefficient} {xxxxxx}
			s += "{}{}{}".format(" + " if c >= 0 and d > 0 else " - " if d > 0 else "",  #
								 c if d == 0 else abs(c) if abs(c) != 1 else "",
								 "x" * d)
		return s

	def degree(self):
		return len(self.coefficients) - 1

	def update(self):
		while len(self.coefficients) > 1 and self.coefficients[-1] == 0:
			self.coefficients = self.coefficients[:-1]

	def __add__(self, other): # returns self + other
		# we use itertools.zip_longest and fillvalue because polynomials may have different degrees
		return Polynomial([c1 + c2 for c1, c2 in itertools.zip_longest(self.coefficients, other.coefficients, fillvalue=0)])

	def __sub__(self, other): # returns self - other
		# we use itertools.zip_longest and fillvalue because polynomials may have different degrees
		return Polynomial([c1 - c2 for c1, c2 in itertools.zip_longest(self.coefficients, other.coefficients, fillvalue=0)])

	def __call__(self, x): # calculates value of P(x) for given x
		s = 0
		for c in self.coefficients[::-1]:
			s = (s * x + c)
		return s

	def __mul__(self, other): # returns self * other
		mult_coef = [0 for i in range(self.degree() + other.degree() + 1)]
		for i in range(0, self.degree() + 1):
			for j in range(0, other.degree() + 1):
				mult_coef[i + j] += self.coefficients[i] * other.coefficients[j]
		return Polynomial(mult_coef)

	def __truediv__(self, other): # returns (self / other, self % other)
		if other.degree() == 0 and other.coefficients[0] == 0:
			raise Exception("{} / {} - division by zero".format(self, other))
		div_coef = [0 for i in range(self.degree() - other.degree() + 1)]
		p1 = self
		p2 = other
		while p1.degree() >= other.degree(): # standard Gorner's scheme
			i = p1.degree() - other.degree()
			div_coef[i] = p1.coefficients[-1] / p2.coefficients[-1]
			p1 = p1 - Polynomial([div_coef[i]]) * x_to_n(p1.degree() - p2.degree()) * p2
		pd = Polynomial(div_coef)
		pr = self - pd * other

		return pd, pr

	def gcd(self, other):
		if self.is_zero() or other.is_zero():
			return self + other
		if self.degree() > other.degree():
			pd, pr = self / other
			return other.gcd(pr)
		else:
			pd, pr = other / self
			return self.gcd(pr)

	def derivative(self):
		return Polynomial([i * c for i, c in enumerate(self.coefficients) if i > 0])

	def reduce_multiple_roots(self):
		gcd = self.gcd(self.derivative())
		return (self / gcd)[0]

def main():
	p10 = Polynomial([1, 0, 5, 0, 6])
	print("p10 is {}".format(p10))
	p11 = Polynomial([1, 0, 2])
	print("p11 is {}".format(p11))
	pd, pr = p10 / p11
	print("p10 / p11 is {}, p10 % p11 is {}".format(pd, pr))
	
	print("pd derivative is {}".format(pd.derivative()))
	p1 = Polynomial([1, 2, 1])
	p2 = Polynomial([1, 1, 1, 1])
	print("p1 is {}".format(p1))
	print("p2 is {}".format(p2))
	print("gcd(p1, p2) = {}".format(p1.gcd(p2)))

	p3 = Polynomial([1, 2, 3]) * Polynomial([1, 2, 3]) * Polynomial([1, 3])
	print("p3 = {}".format(p3))
	print("p3 with no multiple roots = {}".format(p3.reduce_multiple_roots()))

if __name__ == '__main__':
	main()