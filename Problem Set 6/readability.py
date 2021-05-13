from cs50 import get_string
import sys
import re


def main():
    text = get_string("Text: ")
    words = text.split(" ")
    wordCount = len(words)
    letterCount = 0
    sentenceCount = 0
    pattern = re.compile("[A-Za-z]+")
    for word in words:
        for char in word:
            if char == "!" or char == "." or char == "?":
                sentenceCount += 1
            else:
                test = pattern.fullmatch(char)
                if test != None:
                    letterCount += 1
    averageLetters = (letterCount / wordCount) * 100.00
    averageSentences = (sentenceCount / wordCount) * 100.00
    grade = calcGrade(averageLetters, averageSentences)
    if grade >= 16:
        print("Grade 16+")
        sys.exit()
    if grade < 1:
        print("Before Grade 1")
        sys.exit()
    print("Grade", grade)


def calcGrade(L, S):
    return round(0.0588 * L - 0.296 * S - 15.8)


main()