import time
import pandas as pd

import test_integer
import seaborn as sns
import matplotlib.pyplot as plt

def evaluate_add_time(a, b):
    start = time.time()
    test_integer.test_add(a, b)
    end = time.time()
    return end - start

def evaluate_minus_time(a, b):
    start = time.time()
    test_integer.test_minus(a, b)
    end = time.time()
    return end - start

def test():
    key_add, key_minus= [], []
    value_add, value_minus = [], []
    
    num1, num2 = "2", "1"
    for i in range(20):
        print("Nums in calculation are ", num1, num2)
        key_add.append(len(num1))
        value_add.append(evaluate_add_time(num1, num2))
        
        key_minus.append(len(num1))
        value_minus.append(evaluate_minus_time(num1, num2))
        
        num1 += "0"
        num2 += "0"
    
    print(key_add, value_add)
    return {'x': key_add, 'y':value_add}, {'x': key_minus, 'y':value_minus}

dict_add, dict_minus = test()
df_add = pd.DataFrame(dict_add)
df_minus = pd.DataFrame(dict_minus)

sns.lineplot(x='x', y='y', data=df_add)
sns.lineplot(x='x', y='y', data=df_minus)

plt.savefig('output.png')
    
