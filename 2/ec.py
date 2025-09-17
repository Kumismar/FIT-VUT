#!/usr/bin/env python3
"""Module for Elliptic Curve (EC) operations from scratch."""


from typing import Tuple, Union
import secrets


class Curve:
    """
    Class representing Elliptic curves. The curves are defined by the parameters:
    - p: The prime number that defines the field.
    - a, b: The coefficients of the curve.
    - G: The base (generator) point of the curve.
    - n: The order of the curve.
    - h: The cofactor of the curve.

    The class implements the following methods:
    - point_add(p, q): Add two points on the curve.
    - point_mul(k, p): Multiply a point by a scalar.
    - generate_keys(): Generate a private key and a public key for the Elliptic Curve Diffie-Hellman (ECDH) key exchange.
    """

    def __init__(self, p, a, b, G, n, h):
        """
        Inintialize the Elliptic Curve with the given parameters.
        """
        self.p = p
        self.a = a
        self.b = b
        self.G = G
        self.n = n
        self.h = h
        self.infinity = None

    def point_add(self, p, q) -> Union[None, Tuple[int, int]]:
        """
        Add two points on the curve.

        Args:
            p (tuple[int, int]): The first point.
            q (tuple[int, int]): The second point.

        Returns:
            None | tuple[int, int]: The result of the addition of the two points over the curve.
        """

        # Infinity is an identity element in EC arithmetic.
        if p == self.infinity:
            return q
        if q == self.infinity:
            return p

        x_p, y_p = p
        x_q, y_q = q

        # If the points are inverse, their sum is infinity.
        if x_p == x_q and (y_p + y_q) % self.p == 0:
            return self.infinity

        # Because we work over a finite field, we have to use multiplicative inversion
        # instead of classic division. The same principle applies to squaring the number.
        if p == q:
            if y_p == 0:
                return self.infinity

            # For coincident points, we use lambda = (3x_p^2 + a) / 2y_p which comes
            # from derivatives of y^2 = x^3 + ax + b by x and y.
            numerator = (3 * pow(x_p, 2, self.p) + self.a) % self.p
            denominator_inv = pow((2 * y_p) % self.p, -1, self.p)
            l = (numerator * denominator_inv) % self.p
        else:
            numerator = (y_q - y_p) % self.p
            denominator_inv = pow(x_q - x_p, -1, self.p)
            l = (numerator * denominator_inv) % self.p

        x_r = (pow(l, 2, self.p) - x_p - x_q) % self.p
        y_r = (l * (x_p - x_r) - y_p) % self.p

        return (x_r, y_r)

    def point_mul(self, k, p) -> Tuple[int, int]:
        """
        Multiply a point by a scalar.

        Args:
            k (int): The scalar.
            p (tuple[int, int]): The point.

        Returns:
            tuple[int, int]: The result of the multiplication of the point by the scalar over the curve.
        """

        if p == self.infinity or k == 0:
            return self.infinity

        # We know that k * P = (-k) * (-P). Therefore, we find inverse point for P
        # and multiply it with positive k.
        if k < 0:
            x_p, y_p = p
            neg_P = (x_p, -y_p % self.p)
            return self.point_mul(-k, neg_P)

        # We use double-and-add algorithm with linear time complexity.
        # A naive algorithm uses and exponential amount of point additions,
        # which is basically uncomputable for big k.
        result = self.infinity
        temp = p

        # Go through binary representation of k from right to left.
        # 2^i * P can be effectively implemented with gradually doubling the point p.
        # -> 2P = P + P, 4P = 2P + 2P, etc.
        k_temp = k
        while k_temp > 0:
            if k_temp & 1:
                result = self.point_add(result, temp)
            temp = self.point_add(temp, temp)
            k_temp >>= 1

        return result

    def generate_keys(self) -> Tuple[int, Tuple[int, int]]:
        """
        Generate a private key and a public key for the Elliptic Curve Diffie-Hellman (ECDH) key exchange.

        Returns:
            tuple[int, tuple[int, int]]: The private key and the public key.
        """

        private_key = secrets.randbelow(self.n - 1) + 1
        public_key = self.point_mul(private_key, self.G)

        return private_key, public_key

    def __repr__(self):
        """
        Return the string representation of the Elliptic Curve.
        """
        return f"""Elliptic Curve(
    p={self.p}
    a={self.a}
    b={self.b}
    G={self.G}
    n={self.n}
    h={self.h}
)"""


curve_secp256r1 = Curve(
    p=0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff,
    a=0xffffffff00000001000000000000000000000000fffffffffffffffffffffffc,
    b=0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b,
    G=(
        0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296,
        0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5,
    ),
    n=0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551,
    h=0x01,
)
