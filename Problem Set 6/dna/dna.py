import sys
import csv


def main():
    if len(sys.argv) <= 1:
        print("Usage: python dna.py data.csv sequence.txt")

    csvDataPath = sys.argv[1]
    sequenceDataPath = sys.argv[2]

    STR = []
    persons = []

    # Read individuals into memory from file
    with open(csvDataPath, "r") as file:
        reader = csv.reader(file)
        rowCount = 0
        for row in reader:
            if rowCount == 0:
                for col in row:
                    if col != "name":
                        STR.append(col)
                rowCount += 1
            else:
                person = {
                    "name": row[0],
                }
                c = 1
                for s in STR:
                    person[s] = row[c]
                    c += 1

                persons.append(person)

    # read sequence into memory
    with open(sequenceDataPath, "r") as txtfile:
        sequence = txtfile.read()

    # Count STRs in sequence
    def newSliceMethod(STR, seq):
        skipToIndex = 0
        consecutive = 1
        longestConsecutive = 0
        length = len(STR)
        for s in range(len(seq)):
            if skipToIndex > s:
                continue
            strSlice = seq[s:s+length]
            if(strSlice == STR):
                if s != 0 and skipToIndex == s:
                    consecutive += 1
                skipToIndex = s+length
            else:
                # consecutive match has ended
                if consecutive > longestConsecutive:
                    longestConsecutive = consecutive
                consecutive = 1
        return longestConsecutive

    STRmatchCount = []

    for i in STR:
        STRmatchCount.append(newSliceMethod(i, sequence))

    def findPersonByMatch(personArray, STRmatchCountArray):
        for person in personArray:
            strIndex = 0
            matches = 0
            for k in person:
                if k != "name":
                    if int(person[k]) == int(STRmatchCountArray[strIndex]):
                        strIndex += 1
                        if len(STRmatchCountArray) == strIndex:
                            matches += 1
                            print(person["name"])
                            sys.exit()
                            strIndex = 0
                    else:
                        strIndex = 0

    findPersonByMatch(persons, STRmatchCount)
    print("No match")


main()