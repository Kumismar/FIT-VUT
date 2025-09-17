#!/usr/bin/env python3
import argparse
import socket
import sys
from common import *
from ec import *
import time

"""
Implementation of a simple server for the key-exchange.

Supports both Elliptic Curve (ECDH) and Ordinary Diffie-Hellman (DH).
The server should wait for client connection, perform the key-exchange, close the connection and exit.

The server writes the used keys to specific files:
- private key to "server.priv"
- public key to "server.pub"
- shared key (the SHA-256 hash) to "server.shared"

The shared key is the *SHA-256 hash of the shared value*:
- In case of ECDH: Hash of the x-coordinate (first coord.) of the shared point
- In case of DH: Hash of the hexadecimal notation (without the '0x' prefix!) of shared value

The server accepts the following command-line arguments:
- --ec: Use Elliptic Curve Diffie-Hellman (ECDH) instead of Ordinary Diffie-Hellman (DH).
- --port: The port to listen on.

Example:
$ python3 server.py --ec --port 12345

The server should *not* output anything to the standard output, only to the aforementioned files.
"""


def perform_classic_exchange(conn: socket):

    # Generate private and public key, write them to files.
    debug_message("[Server]: Generating private and public keys...")

    start_time = time.perf_counter()
    server_private_key = generate_private_key()
    server_public_key = calculate_public_key(server_private_key)
    end_time = time.perf_counter()

    debug_message(
        f"[Server]: Private key creation and public key calculation took: {(end_time - start_time) * 1000:.4f} ms")

    output_key(server_private_key, "server.priv")
    output_key(server_public_key, "server.pub")

    # Send calculated public key to client.
    debug_message("[Server]: Sending public key to client.")
    conn.sendall(server_public_key.to_bytes(
        length=CLASSIC_KEY_LENGTH_BYTES, byteorder="big", signed=False))

    # Recieve clients' calculated public key, convert to readable form.
    debug_message("[Server]: Waiting for clients public key...")
    client_public_key_bytes = connection.recv(CLASSIC_KEY_LENGTH_BYTES)
    debug_message("[Server]: Got clients public key.")

    if not client_public_key_bytes or len(client_public_key_bytes) != CLASSIC_KEY_LENGTH_BYTES:
        print(
            f"Error: Invalid key recieved: {client_public_key_bytes.hex()}", file=sys.stderr)
        exit(-1)
    client_public_key = int.from_bytes(
        client_public_key_bytes, byteorder="big", signed=False)

    # Calcualate shared key, hash it and output it.
    debug_message("[Server]: Calculating shared key...")

    start_time = time.perf_counter()
    shared_key = calculate_shared_key(client_public_key, server_private_key)
    end_time = time.perf_counter()
    debug_message(
        f"[Server]: Shared key calculation took: {(end_time - start_time) * 1000:.4f} ms")

    debug_message("[Server]: Writing the shared key to a file...")
    hash_and_output(shared_key, "server.shared", ec=False)


def perform_ec_exchange(conn: socket):

    # Generate private and public keys, write them to file.
    debug_message("[Server]: Generating private and public keys...")
    start_time = time.perf_counter()
    server_private_key, server_public_key_point = curve_secp256r1.generate_keys()
    end_time = time.perf_counter()

    debug_message(
        f"[Server]: EC private key creation and public key calculation took: {(end_time - start_time) * 1000:.4f} ms")
    output_key(server_private_key, "server.priv")
    output_key(server_public_key_point, "server.pub", ec=True)

    # Prepare the point for transmission and send it to client.
    server_public_key_bytes = serialize_point(server_public_key_point)
    debug_message("[Server]: Sending public key to client.")
    conn.sendall(server_public_key_bytes)

    # Get clients public key. Check if the public key was correctly transmitted.
    expected_client_key_len = 2 * EC_COORD_LENGTH_BYTES
    debug_message("[Server]: Waiting for clients public key...")
    client_public_key_bytes = conn.recv(expected_client_key_len)
    debug_message("[Server]: Got clients public key.")

    if not client_public_key_bytes or len(client_public_key_bytes) != expected_client_key_len:
        print(
            f"Error: Invalid key received. Expected {expected_client_key_len} bytes, got {len(client_public_key_bytes)}.", file=sys.stderr)
        if client_public_key_bytes:
            print(
                f"Received data (hex): {client_public_key_bytes.hex()}", file=sys.stderr)
        exit(-1)

    # Deserialize the recieved point, calculate shared key and output it.
    client_public_key_point = deserialize_point(client_public_key_bytes)
    debug_message("[Server]: Calculating shared key...")

    start_time = time.perf_counter()
    shared_secret_point = curve_secp256r1.point_mul(
        server_private_key, client_public_key_point)
    end_time = time.perf_counter()
    debug_message(
        f"[Server]: EC shared key calculation took: {(end_time - start_time) * 1000:.4f} ms")

    if shared_secret_point == None:
        print("Error: Shared key is infinity (EC)", file=sys.stderr)
        exit(-1)

    x, _ = shared_secret_point
    hash_and_output(x, "server.shared", ec=True)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", nargs=1, default=-1, required=True, type=int)
    parser.add_argument("--ec", action="store_true")
    args = parser.parse_args()
    port = args.port[0]

    if port < 1024 or port > 65535:
        print("Error: Invalid port number", file=sys.stderr)
        exit(-1)

    with socket.create_server(("localhost", port)) as server:
        debug_message("[Server]: Waiting for client connection...")
        connection, addr = server.accept()
        debug_message("[Server]: Client successfully connected.")

        if args.ec:
            perform_ec_exchange(connection)
        else:
            perform_classic_exchange(connection)

        connection.close()
        debug_message("[Server]: Connection closed.")
