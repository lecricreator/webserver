#!/usr/bin/python3
import os
import sys
import os.path

print("Content-Type: text/html")
print()
a
print("<html><body style='text-align:center;'>")
print("PYTHON PROGRAMMING")
print("<h1 style='color: pink;'>This is the most shitty CGI !!!</h1>")

print("<pre>")
print("=== ENV VARS ===")
for k, v in os.environ.items():
    print(f"{k}={v}")


length = int(os.environ.get("CONTENT_LENGTH", 0) or 0)
data = sys.stdin.read(length) if length else ""
print("\n=== STDIN ===")
print(data)
print("\n=== RELATIVE PATH ===")
if (os.path.isfile("test.sh")):
    print("test exists")
else:
    print("test.sh doesnt exist")
print("</pre>")

print("</body></html>")
