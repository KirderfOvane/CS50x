from cs50 import get_int
from sys import exit

card = get_int("Number: ")


def Validate(card):
    def split(word):
        return [char for char in word]

    stringCard = str(card)[::-1]
    multipliedDigits = []
    for d in range(1, len(str(card)), 2):
        multipliedDigits.append(int(stringCard[d]) * 2)

    splitDigits = []
    for w in multipliedDigits:
        splitDigits.append(split(str(w)))

    sumDigits = 0
    for s in splitDigits:
        for k in s:
            sumDigits += int(k)

    notMultipliedDigitsSum = 0
    for c in range(0, len(str(card)), 2):
        notMultipliedDigitsSum += int(stringCard[c])

    # extracts the last digit in the sum and checks if its zero
    if int(str(notMultipliedDigitsSum + sumDigits)[len(str(notMultipliedDigitsSum + sumDigits))-1]) == 0:
        return True
    return False


cardLength = (len(str(card)))
firstTwoDigits = int(str(str(card)[0] + str(card)[1]))
firstDigit = int(str(card)[0])

if Validate(card):
    if cardLength == 15 and firstTwoDigits == 34 or firstTwoDigits == 37:
        print("AMEX")
        exit()
    if cardLength == 16 and firstTwoDigits in [51, 52, 53, 54, 55]:
        print("MASTERCARD")
        exit()
    if cardLength == 13 or cardLength == 16 and firstDigit == 4:
        print("VISA")
        exit()
print("INVALID")