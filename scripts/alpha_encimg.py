#!/usr/bin/env python3
#
# encrypt factory images for D-Link DIR-842, DIR-859
#
# Copyright (c) 2020 Sebastian Schaper <openwrt@sebastianschaper.net>
#
# License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl-2.0.html>
#

from Crypto.Cipher import AES
import os, sys

if len(sys.argv) != 5:
    raise TypeError( \
        'alpha_encimg expected 4 arguments (file, signature, key, iv), got {}' \
            .format(len(sys.argv) - 1))

input_file = sys.argv[1]
SIGNATURE, AES_KEY, AES_IV = [os.fsencode(a) for a in sys.argv[2:5]]
# Key and IV can be found in fw_sign_data.c from the official GPL release
# or extracted by running 'strings /htdocs/cgibin' on the official firmware:
# AES_KEY, AES_IV = fw_sign_data[0x20:0x40], fw_sign_data[0x60:0x70]

if len(AES_KEY) != 32:
    raise ValueError('AES_KEY must be 32 bytes')
if len(AES_IV) != 16:
    raise ValueError('AES_IV must be 16 bytes')

def alpha_xor(input_bytes, xorkey):
    out = bytearray(input_bytes)
    for i in range(len(out)):
        out[i] = (i + 1) ^ out[i]
        out[i] = xorkey[i % len(xorkey)] ^ out[i]
    return bytes(out)

AES_KEY = alpha_xor(AES_KEY, SIGNATURE)
AES_IV  = alpha_xor(AES_IV,  SIGNATURE)

cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)

with open(input_file, "rb") as fd:
    input_bytes = fd.read()

input_padding = 16 - (len(input_bytes) % 16)
input_bytes += bytes([0x00] * input_padding)

input_bytes += (len(input_bytes) - input_padding).to_bytes(4, 'big')
input_bytes += bytes([0x00] * 12)

encrypted = cipher.encrypt(input_bytes)

with open(input_file, "wb") as fd:
    fd.write(encrypted)

