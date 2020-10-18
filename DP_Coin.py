d = [500, 160, 100, 50, 10]
n = int(input("거스름돈 액수 : "))
C=[]
C.append(0)
for q in range(1, n+1):
    C.append(float('inf'))
   


for j in range (1, n+1):
    for i in range (len(d)):
        if d[i]<= j and C[j-d[i]]+1 < C[j]:
            C[j] = C[j-d[i]]+1

print(C[n])
# for i in range(len(d)):
#     print("d[i]의 값 : ",d[i])
# n= int(input(": "))
# for j in range (1, n+1):
#     print("j: ",j)