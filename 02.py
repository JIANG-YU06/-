a={}

with open("三国.txt",'r',encoding='ANSI') as f1:
    read = f1.readline()#读取全文
    while(read):
        for i in read:
            if i=='\n':
                i='\\n'
            a[i] = a.get(i,0)+1
        read = f1.readline()

#print(a)
list_n = list(a.items())
#print(list_n)
#list_n.sort(key=lambda x:x[1],reverse=False) #lambda匿名函数 reverse=True为从大到小
print(list_n)


with open("权值.txt",'w',encoding='ANSI') as f2:
    for i in range(len(list_n)):
        f2.write(f'{list_n[i][0]}: {list_n[i][1]}\n')

