import secrets
import hashlib
from typing import Tuple, Union
from ec import Curve, curve_secp256r1
import sys
import json

p = 0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca18217c32905e462e36ce3be39e772c180e86039b2783a2ec07a28fb5c55df06f4c52c9de2bcbf6955817183995497cea956ae515d2261898fa051015728e5a8aaac42dad33170d04507a33a85521abdf1cba64ecfb850458dbef0a8aea71575d060c7db3970f85a6e1e4c7abf5ae8cdb0933d71e8c94e04a25619dcee3d2261ad2ee6bf12ffa06d98a0864d87602733ec86a64521f2b18177b200cbbe117577a615d6c770988c0bad946e208e24fa074e5ab3143db5bfce0fd108e4b82d120a92108011a723c12a787e6d788719a10bdba5b2699c327186af4e23c1a946834b6150bda2583e9ca2ad44ce8dbbbc2db04de8ef92e8efc141fbecaa6287c59474e6bc05d99b2964fa090c3a2233ba186515be7ed1f612970cee2d7afb81bdd762170481cd0069127d5b05aa993b4ea988d8fddc186ffb7dc90a6c08f4df435c934063199ffffffffffffffff

g = 2

DEBUG_MODE = True
CLASSIC_KEY_LENGTH_BYTES = (p.bit_length() + 7) // 8
EC_COORD_LENGTH_BYTES = (curve_secp256r1.p.bit_length() + 7) // 8


def debug_message(msg):
    if DEBUG_MODE:
        print(msg)


def calculate_public_key(private_key: int) -> int:
    return pow(g, private_key, p)


def generate_private_key() -> int:
    # We want a range from 2 to p - 1
    return secrets.randbelow(p - 2) + 2


def calculate_shared_key(public_key: int, private_key: int) -> int:
    return pow(public_key, private_key, p)


def output_key(key: Union[int, Tuple[int, int]], filename: str, ec: bool = False):
    with open(filename, "w") as f:
        if filename.endswith(".priv"):
            f.write(str(key))
        elif filename.endswith(".pub"):
            if ec:
                x, y = key
                f.write(json.dumps({"x": x, "y": y}))
            else:
                f.write(str(key))


def hash_and_output(key: int, filename: str, ec: bool = False):
    hasher = hashlib.sha256()
    if ec:
        key_bytes = key.to_bytes(
            EC_COORD_LENGTH_BYTES, byteorder="big", signed=False)
    else:
        # Convert to hex and trim "0x"
        hex_key = hex(key)[2:]
        key_bytes = hex_key.encode("utf-8")

    hasher.update(key_bytes)
    key_hex_str = hasher.hexdigest()

    with open(filename, "w") as f:
        f.write(key_hex_str)


def serialize_point(point: Tuple[int, int]) -> bytes:
    x, y = point
    xb = x.to_bytes(EC_COORD_LENGTH_BYTES, byteorder="big", signed=False)
    yb = y.to_bytes(EC_COORD_LENGTH_BYTES, byteorder="big", signed=False)
    return xb + yb


def deserialize_point(data: bytes) -> Tuple[int, int]:
    xb = data[:EC_COORD_LENGTH_BYTES]
    yb = data[EC_COORD_LENGTH_BYTES:]
    x = int.from_bytes(xb, byteorder="big", signed=False)
    y = int.from_bytes(yb, byteorder="big", signed=False)

    return (x, y)
