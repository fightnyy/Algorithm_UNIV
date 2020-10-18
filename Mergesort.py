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
        
    return v

def merge_sort(v):
    if len(v) <=1 :
        return v
    m = len(v)//2
    left = merge_sort(v[0:m])
    right = merge_sort(v[m:len(v)])
    return merge(left, right)



def main ():
    S = [8,7,6,5,4,3,2,1]
    q = [10,11,9]
    print("Merged : ",merge_sort(S+q))

if __name__ == "__main__":
    main()