def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr)//2]
    lesser_arr, equal_arr, greater_arr = [], [], []
    for num in arr:
        if num < pivot:
            lesser_arr.append(num)
        elif num > pivot:
            greater_arr.append(num)
        else :
            equal_arr.append(num)

    return quick_sort(lesser_arr)+equal_arr+quick_sort(greater_arr)


def main ():
    arr = [6,7,8,5,4,3,2,1]
    s_arr = quick_sort(arr)
    print(s_arr)



if __name__ == "__main__":
    main()