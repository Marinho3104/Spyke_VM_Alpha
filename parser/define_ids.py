
file = open("./token_definitions.h", 'r')

info = file.read()

file.close()

info = info.split('\n')
count = 0
#print(info)

for _ in info:

    if _ == '': 
        print("")
        continue

    info_split_space = _.split(' ')

    info_split_space[2] = str(count)

    count += 1

    print(' '.join(info_split_space))