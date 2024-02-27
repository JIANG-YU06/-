
a={}

with open("三国.txt",'r',encoding='ANSI') as f1:
    read = f1.readline()#读取全文
    while(read):
        for i in range(0,len(read),2):
            cut = read[i:(i+2)]
            a[cut] = a.get(cut,0)+1
        read = f1.readline()

#print(a)
list_n = list(a.items())
#print(list_n)
list_n.sort(key=lambda x:x[1],reverse=False) #lambda匿名函数 reverse=True为从大到小
print(list_n)


with open("权重.txt",'w',encoding='ANSI') as f2:
    for i in range(len(list_n)):
        if list_n[i][0] == "\n\n":
            f2.write(f'\\n\\n: {list_n[i][1]}\n')
        else:
            if  "\n" in list_n[i][0] :
                list1 = list_n[i][0].strip("\n")
                f2.write(f'{list1}\\n: {list_n[i][1]}\n')
            else:
                f2.write(f'{list_n[i][0]}: {list_n[i][1]}\n')

