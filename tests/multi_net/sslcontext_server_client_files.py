# Simple test creating an SSL connection and transferring some data

try:
    import binascii
    import socket
    import ssl
    import sys
except ImportError:
    print("SKIP")
    raise SystemExit


# Asyncio + TLS in 32-bit systems fails.
# Reason: UNKNOWN
# Possible cause:
#   Incompatible ciphersuites + streams.
#   i.e. error OSError: (-29568, 'MBEDTLS_ERR_SSL_NO_CIPHER_CHOSEN')
# Action: SKIP for now.
if sys.maxsize == 2147483647:
    print("SKIP")
    raise SystemExit

PORT = 8000


# This self-signed key/cert pair is randomly generated and to be used for
# testing/demonstration only.  You should always generate your own key/cert.

# To generate a new self-signed key/cert pair with openssl do:
# $ openssl req -x509 -newkey rsa:4096 -keyout rsa_key.pem -out rsa_cert.pem
# -days 365 -nodes
# In this case CN is: micropython.local
#
# Convert them to DER format:
# $ openssl rsa -in rsa_key.pem -out rsa_key.der -outform DER
# $ openssl x509 -in rsa_cert.pem -out rsa_cert.der -outform DER
#
# Then convert to hex format, eg using binascii.hexlify(data).


cert = cafile = "multi_net/rsa_cert.der"

key = "multi_net/rsa_key.der"


# Server
def instance0():
    multitest.globals(IP=multitest.get_network_ip())
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(socket.getaddrinfo("0.0.0.0", PORT)[0][-1])
    s.listen(1)
    multitest.next()
    s2, _ = s.accept()
    server_ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    server_ctx.load_cert_chain(cert, keyfile=key)
    s2 = server_ctx.wrap_socket(s2, server_side=True)
    print(s2.read(16))
    s2.write(b"server to client")
    s2.close()
    s.close()


# Client
def instance1():
    multitest.next()
    s = socket.socket()
    s.connect(socket.getaddrinfo(IP, PORT)[0][-1])
    client_ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
    client_ctx.verify_mode = ssl.CERT_REQUIRED
    client_ctx.load_verify_locations(cafile=cafile)
    s = client_ctx.wrap_socket(s, server_hostname="micropython.local")
    s.write(b"client to server")
    print(s.read(16))
    s.close()
