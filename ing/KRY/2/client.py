#!/usr/bin/env python3

import socket
import sys
import argparse
from common import *
from ec import curve_secp256r1
import time

"""
Implementation of a simple client for the key-exchange.

Supports both Elliptic Curve (ECDH) and Ordinary Diffie-Hellman (DH).
The client should connect to the server and perform the key-exchange.

The client writes the used keys to specific files:
- private key to "client.priv"
- public key to "client.pub"
- shared key (the SHA-256 hash) to "client.shared"

The shared key is the *SHA-256 hash of the shared value*:
- In case of ECDH: Hash of the x-coordinate (first coord.) of the shared point
- In case of DH: Hash of the hexadecimal notation (without the '0x' prefix!) of shared value

The client accepts the following command-line arguments:
- --ec: Use Elliptic Curve Diffie-Hellman (ECDH) instead of Ordinary Diffie-Hellman (DH).
- --port: The port to connect to the server.

Example:
$ python3 client.py --ec --port 12345

The client should *not* output anything to the standard output, only to the aforementioned files.
"""


def perform_classic_exchange(conn: socket):

    # Generate private and public key, write them to files.
    debug_message("[Client]: Generating private and public keys...")

    start_time = time.perf_counter()
    client_private_key = generate_private_key()
    client_public_key = calculate_public_key(client_private_key)
    end_time = time.perf_counter()

    debug_message(
        f"[Client]: Private key creation and public key calculation took: {(end_time - start_time) * 1000:.4f} ms")

    output_key(client_private_key, "client.priv")
    output_key(client_public_key, "client.pub")

    # Recieve servers' calculated public key, convert to readable form.
    debug_message("[Client]: Waiting for server public key...")
    server_public_key_bytes = connection.recv(CLASSIC_KEY_LENGTH_BYTES)
    debug_message("[Client]: Got server public key.")

    if not server_public_key_bytes or len(server_public_key_bytes) != CLASSIC_KEY_LENGTH_BYTES:
        print(
            f"Error: Invalid key recieved: {server_public_key_bytes.hex()}", file=sys.stderr)
        exit(-1)
    server_public_key = int.from_bytes(
        server_public_key_bytes, byteorder="big", signed=False)

    # Send calculated public key to server.
    debug_message("[Client]: Sending public key to server.")
    conn.sendall(client_public_key.to_bytes(
        length=CLASSIC_KEY_LENGTH_BYTES, byteorder="big", signed=False))

    # Calcualate shared key, hash it and output it.
    debug_message("[Client]: Calculating shared key...")

    start_time = time.perf_counter()
    shared_key = calculate_shared_key(server_public_key, client_private_key)
    end_time = time.perf_counter()

    debug_message(
        f"[Client]: Shared key calculation took: {(end_time - start_time) * 1000:.4f} ms")

    debug_message("[Client]: Writing the shared key to a file...")
    hash_and_output(shared_key, "client.shared")


def perform_ec_exchange(conn: socket):

    # Generate private and public keys, write them to file.
    debug_message("[Client]: Generating private and public keys...")

    start_time = time.perf_counter()
    client_private_key, client_public_key_point = curve_secp256r1.generate_keys()
    end_time = time.perf_counter()

    debug_message(
        f"[Client]: EC private key creation and public key calculation took: {(end_time - start_time) * 1000:.4f} ms")

    output_key(client_private_key, "client.priv")
    output_key(client_public_key_point, "client.pub", ec=True)

    # Get server public key. Check if the public key was correctly transmitted.
    expected_server_key_len = 2 * EC_COORD_LENGTH_BYTES
    debug_message("[Client]: Waiting for servers public key...")
    server_public_key_bytes = conn.recv(expected_server_key_len)
    debug_message("[Client]: Got servers public key.")

    if not server_public_key_bytes or len(server_public_key_bytes) != expected_server_key_len:
        print(
            f"Error: Invalid key recieved. Expected {expected_server_key_len} bytes, got {len(server_public_key_bytes)}.", file=sys.stderr)
        if server_public_key_bytes:
            print(
                f"Received data (hex): {server_public_key_bytes.hex()}", file=sys.stderr)
        exit(-1)

    # Prepare the point for transmission and send it to server.
    client_public_key_bytes = serialize_point(client_public_key_point)
    debug_message("[Client]: Sending public key to server.")
    conn.sendall(client_public_key_bytes)

    # Deserialize the recieved point, calculate shared key and output it.
    server_public_key_point = deserialize_point(server_public_key_bytes)
    debug_message("[Client]: Calculating shared key...")

    start_time = time.perf_counter()
    shared_secret_point = curve_secp256r1.point_mul(
        client_private_key, server_public_key_point)
    end_time = time.perf_counter()

    debug_message(
        f"[Client]: EC shared key calculation took: {(end_time - start_time) * 1000:.4f} ms")

    if shared_secret_point == None:
        print("Error: Shared key is infinity (EC)", file=sys.stderr)
        exit(-1)

    x, _ = shared_secret_point
    hash_and_output(x, "client.shared", ec=True)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", nargs=1, default=-1, required=True, type=int)
    parser.add_argument("--ec", action="store_true")
    args = parser.parse_args()
    port = args.port[0]

    if port < 1024 or port > 65535:
        print("Error: Invalid port number", file=sys.stderr)
        exit(-1)

    with socket.create_connection(("localhost", port)) as connection:
        debug_message(f"[Client]: Connected to server on localhost:{port}.")

        if args.ec:
            perform_ec_exchange(connection)
        else:
            perform_classic_exchange(connection)

    debug_message("[Client]: Connection closed.")
