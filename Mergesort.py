def merge(left , right) :
    v=[]
    i = 0
    j = 0
    while (i < len(left) and j <len(right)) :
        if left[i] <= right[j] :
            v.append(left[i])
            i+=1

        else :
            v.append(right[j])
            j+=1

    if i == len(left) :
        v = v+right[j:len(right)]
        return v

    if j == len(right) :
        v = v+left[i:len(left)]
    print("merge 함수의 V : ",v)
    print("merge V의 id",id(v))
    return v

def merge_sort(v):
    if len(v) <=1 :
        print("v : ",v)
        return v
    m = len(v)//2
    left = merge_sort(v[0:m])
    right = merge_sort(v[m:len(v)])
    print("left : ",left , "right : ",right)
    return merge(left, right)



def main ():
    S = [8,7,6,5,4,3,2,1]
    print("Merged : ",merge_sort(S))

if __name__ == "__main__":
    main()