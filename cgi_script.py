import os

method = os.environ.get('REQUEST_METHOD', 'GET')
query = os.environ.get('QUERY_STRING', '')

print("Content-Type: text/html")
print()
print(f"<h1>CGI Test</h1>")
print(f"<p>Method: {method}</p>")
print(f"<p>Query string: {query}</p>")
