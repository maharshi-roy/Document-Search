d = {}
with open('words.txt','r') as f:
    str = f.read()
    for each in str:
        d[each] = True

    for keys in d:
        print(keys,ord(keys))