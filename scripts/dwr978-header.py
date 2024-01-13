#! /usr/bin/env python3

import binascii, sys

# input file is assumed to be xor'ed with 29944A25120984C2 already
infile_path, outfile_path = sys.argv[1], sys.argv[2]

header_template = bytes.fromhex(
    "03 1D 61 29 07 44 57 52 2D 39 37 38 00 00 00 00"
    "00 00 00 00 00 30 31 2E 30 30 2E 30 30 33 37 00"
    "00 00 00 00 00 62 35 32 32 61 35 30 63 00 62 37"
    "38 36 31 34 31 33 62 37 33 30 34 63 65 35 61 36"
    "61 61 33 34 65 39 64 38 62 33 65 38 61 64 00 32"
    "30 32 32 2D 30 37 2D 30 34 00 00 40 00 00 00 02"
    "20 03 00 02 00 00")

with open(infile_path, 'rb') as infile:
    header = bytearray(header_template)
    source = bytearray(infile.read())
    header += len(source).to_bytes(4, byteorder = 'big')    # data size
    header += binascii.crc32(source).to_bytes(4, byteorder = 'big') # data crc
    header += bytes([0] * 6)
    header[0x6e:0x72] = binascii.crc32(header).to_bytes(4, byteorder = 'big') # header crc

    with open(outfile_path, 'wb+') as outfile:
        outfile.write(header)
        outfile.write(source)
