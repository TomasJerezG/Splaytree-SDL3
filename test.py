import sys
from collections import defaultdict, deque

lines = sys.stdin.read().strip().splitlines()

N = int(lines[-1].strip())

flight_lines = lines[:-1]

graph = defaultdict(list)
countries = set()
for line in flight_lines:
    src, rhs = line.split("->", 1)
    src = src.strip()
    countries.add(src)
    for dst in rhs.strip().split():
        dst = dst.strip()
        countries.add(dst)
        graph[src].append(dst)


start = "CO"
dist = { c: -1 for c in countries }
dist[start] = 0
queue = deque([start])

while queue:
    u = queue.popleft()
    for v in graph.get(u, []):
        if dist[v] == -1:
            dist[v] = dist[u] + 1
            queue.append(v)

reachable = sum(1 for c, d in dist.items() if c != start and d <= N)
print(reachable)
