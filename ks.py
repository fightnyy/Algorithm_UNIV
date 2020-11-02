def knapsack(capacity, n): 
    if capacity == 0 or n == 0: 
        return 0 
    if size[n-1] > capacity: 
        return knapsack(capacity, n-1) 
    else: return max(value[n-1] + knapsack(capacity-size[n-1], n-1), knapsack(capacity, n-1)) 
    
    
    
    
size = [3, 4, 7, 8, 9] 
value = [4, 5, 10, 11, 13] 
print(knapsack(10, 5)) # 14

