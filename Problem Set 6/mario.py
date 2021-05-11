from cs50 import get_int

while True:
    height = get_int("height: ")
    if height < 9 and height > 0:
        break


count = 1
for h in range(height):
    print(" " * (height-count), end="")
    print("#" * count)
    count += 1
