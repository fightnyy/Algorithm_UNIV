
list_of_num = []
def findA(list_of_num,start,end):
    length=len(list_of_num)
    half = (start+end)//2
    
    if end-start==0 :
        print("ans: " ,list_of_num[start],"index: ",start)
    elif end-start ==2:
        if list_of_num[half] == list_of_num[half-1]:
            ans= list_of_num[half+1]
            print("ans1 : ",ans, "index : ",half+1)
            
        elif list_of_num[half] == list_of_num[half+1]:
            ans = list_of_num[half-1]
            print("ans2 : ",ans,half-1)

    
    else :
        if list_of_num[half] == list_of_num[half-1]:
            if len(list_of_num[0:half-1])%2 == 1:
                end=half-2
                findA(list_of_num,start,end)
                
            else:
                start=half+1
                findA(list_of_num,start,end)

        elif list_of_num[half] == list_of_num[half+1]:
            if len(list_of_num[0:half])%2 == 1:
                end=half-1
                findA(list_of_num,start,end)
            else:
                start=half+2
                findA(list_of_num,start,end)

        else:
            print("ans3 : ",list_of_num[half],half)                    

def main():
    length=int(input("length : "))
    for k in range(length):
        num_Of_Array=int(input("put any num : "))
        list_of_num.append(num_Of_Array)

    
    findA(list_of_num,0,length-1)    
    

if __name__ == "__main__":
    main()        