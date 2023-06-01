#!/usr/bin/env python3
#
# encrypt factory images for D-Link COVR-X1860
#
# requires pyCryptodome (`pip install pycryptodome`)
#
# License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl-2.0.html>
#

import base64, os, sys

from Crypto.Cipher import AES
from Crypto.Hash import SHA512
from Crypto.Signature import PKCS1_PSS, PKCS1_v1_5
from Crypto.PublicKey import RSA


if len(sys.argv) != 2:
    raise TypeError("dlink-covr-x-image.py: no input file given")

input_file = sys.argv[1]

HEAD_MAGIC = b"SHRS"
HEAD_MAGIC_LEN = 4
RSA_KEY_BITS = 4096
SHA512_DIGEST_LENGTH = 64
ES_BLOCK_SIZE = 16

inter =    ((2, 5, 7, 4, 0, 6, 1, 3),
            (7, 3, 2, 6, 4, 5, 1, 0),
            (5, 1, 6, 7, 3, 0, 4, 2),
            (0, 3, 7, 6, 5, 4, 2, 1),
            (1, 5, 7, 0, 3, 2, 6, 4),
            (3, 6, 2, 5, 4, 7, 1, 0),
            (6, 0, 5, 1, 3, 4, 2, 7),
            (4, 6, 7, 3, 2, 0, 1, 5))

def deinterleaving(enk):
    return ''.join([enk[(c // 8) * 8 + inter[c // 8][c % 8]] for c in range((len(enk) // 8) * 8)] + [enk[::-1][:len(enk) % 8]])

def dimgkey(enk):
    return deinterleaving(base64.b64decode(enk).decode()).encode()

COVR_X1860_enk = "NE1oIS1lKzkkIzZkbX49KTMsMWFkJXEybjheJiN6KjIwNjgx"
DIR_X3260_enk  = 'NF5yKy10JTl+bSkhNj1kTTIkI3FhIyUsJDU0czMyZmR6Jl4jMzI4KjA2Mg=='

salt = bytes.fromhex("67c6697351ff4aec29cdbaabf2fbe346")

sign_key = """-----BEGIN RSA PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: AES-256-CBC,34CCF1AEF0C34EAC5FFAE6BCF81ABB8D

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
-----END RSA PRIVATE KEY-----
"""

with open(input_file, "rb") as f_in:

    decoded_trunc = f_in.read()
    f_in.close()
    payload_length_before = len(decoded_trunc)

    pad_len = (16 - payload_length_before % 16)

    print("payload_length_before", payload_length_before)
    print("pad_len", pad_len)

    decoded = decoded_trunc + bytes([0x00] * pad_len)

    key = RSA.importKey(sign_key, passphrase=b'12345678')
    print(key)

    signer = PKCS1_v1_5.new(key)

    vendor_key = dimgkey(COVR_X1860_enk)[:16]
    print("vendor_key", vendor_key.hex())

    h = SHA512.new()
    h.update(decoded_trunc)
    sha512_md_before = h.digest()
    print("sha512_md_before", h.hexdigest())

    rsa_sign_before = signer.sign(h)
    print("rsa_sign_before", rsa_sign_before.hex())

    h.update(vendor_key)
    sha512_md_vendor = h.digest()
    print("sha512_md_vendor", h.hexdigest())

    cipher = AES.new(bytes(vendor_key), AES.MODE_CBC, salt)
    data = cipher.encrypt(decoded)

    h = SHA512.new()
    h.update(data)
    sha512_md_post = h.digest()
    print("sha512_md_post", h.hexdigest())

    rsa_sign_post = signer.sign(h)
    print("rsa_sign_post", rsa_sign_post.hex())

    payload_length_post = len(data)

    fullimage = bytes()
    fullimage += HEAD_MAGIC
    fullimage += payload_length_before.to_bytes(4, 'big')
    fullimage += payload_length_post.to_bytes(4, 'big')
    fullimage += salt
    fullimage += sha512_md_vendor
    fullimage += sha512_md_before
    fullimage += sha512_md_post
    fullimage += bytes([0x00] * 512) # rsa public, unused
    fullimage += rsa_sign_before
    fullimage += rsa_sign_post
    fullimage += data
    fullimage += bytes.fromhex("0000000030")

    output_file = input_file

    with open(output_file, "wb") as f_out:
        f_out.write(fullimage)
