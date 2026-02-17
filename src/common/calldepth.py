import sys
import re
from collections import defaultdict

if len(sys.argv) < 2:
    print("Usage: python3 script.py callgraph.dot")
    sys.exit(1)

filename = sys.argv[1]

id_to_name = {}
edges = defaultdict(list)

with open(filename) as f:
    for line in f:
        # Match node definition
        m = re.search(r'(Node0x[0-9a-fA-F]+)\s+\[.*label="\{(.+?)\}"', line)
        if m:
            id_to_name[m.group(1)] = m.group(2)

        # Match edges
        m = re.search(r'(Node0x[0-9a-fA-F]+)\s+->\s+(Node0x[0-9a-fA-F]+)', line)
        if m:
            edges[m.group(1)].append(m.group(2))

if not id_to_name:
    print("No nodes found.")
    sys.exit(1)

visited = {}

def depth(n):
    if n in visited:
        return visited[n]
    if n not in edges or not edges[n]:
        visited[n] = 1
        return 1
    visited[n] = 1 + max(depth(c) for c in edges[n])
    return visited[n]

max_depth = max(depth(n) for n in id_to_name.keys())

print("Functions:", len(id_to_name))
print("Edges:", sum(len(v) for v in edges.values()))
print("Max call depth:", max_depth)
