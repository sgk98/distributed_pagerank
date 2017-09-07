inp=raw_input().split()
N,M=int(inp[0]),int(inp[1]);
mapping={}
nodes=set()
arr=[]
for i in range(M):
    inp=raw_input().split()
    arr.append([int(inp[0]),int(inp[1])])
    nodes.add(int(inp[0]))
    nodes.add(int(inp[1]))

nodes_new=[]
for i in nodes:
    nodes_new.append(i)
nodes_new.sort()
for i in range(len(nodes_new)):
    mapping[nodes_new[i]]=i+1
print N,M
for i in arr:
    print mapping[i[0]],mapping[i[1]]




