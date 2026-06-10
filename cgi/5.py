#!/usr/bin/env python3
import sys
import os

print("Content-Type: text/plain")
print()

content_length = int(os.environ.get("CONTENT_LENGTH", 0))
if content_length > 0:
    post_data = sys.stdin.read(content_length)
else:
    post_data = ""

print("Received POST data length:", content_length)
print("Data:")
print(post_data)
