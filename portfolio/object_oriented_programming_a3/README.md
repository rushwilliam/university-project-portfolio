CIS*2430 F24 Assignment 3
Name: William Rush
Student ID: 1267781
Instructor: F. Song
Date: November 28, 2024

DESCRIPTION

This program stores a database of investments in a list, and provides useful functionalities to the user through a visual GUI. Abstraction and inheritance is used to make the code more efficient and maintainable.

Actions that the user can perform:
 - Buy an investment (Can be a new or existing investment)
 - Sell an existing investment
 - Update all prices
 - Calculate the total gain
 - Search for a specific investment
 - Quit

USER GUIDE

Compiling:

javac ePortfolio/Portfolio.java
javac ePortfolio/DataManager.java
javac ePortfolio/Investment.java
javac ePortfolio/Stock.java
javac ePortfolio/MutualFund.java

or

javac ePortfolio/*.java

Running:

java ePortfolio/Portfolio

TEST PLAN

Menu
 - Clicking a menu option should hide the current visible page
 - Quit should end the program

Intro
 - The program should begin on the intro page

Buy and sell
 - the symbol should be one token
 - symbols should be converted to uppercase
 - symbol search should be case-insensitive
 - if the symbol does not already exist, a new one should be created
 - price and quantity should be numbers
 - price and quantity should not be negative
 - when selling, quantity cannot be greater than an existing quantity
 - the current book value should be given to the user
 - prices should be updated when buying or selling
 - prices should not be updated if there is an error with another part of the transaction
 - the investment should be removed if there are no units left

Update
 - All investments should appear on the update screen
 - previous and next buttons should be disabled if there are no investments before or after the current investment
 - price should be a number
 - price given cannot be negative
 - price changes should be reflected in gain and in selling investments

Get gain
 - gain should be calculated correctly
 - gain should be 0 if there are no investments in the list

Search
 - symbol should be case-insensitive
 - if no symbol is entered, all symbols should be matched
 - keyword matching should be case-insensitive
 - investments should only be matched if all keywords are found
 - keywords should be matched as whole words
 - keyword checking should be tested after removing investments, to verify keywords are removed properly
 - if no keywords are entered, all names should be matched
 - price ranges should match for min-max, min- -max, and exact value
 - price min and max values should be inclusive
 - min cannot be higher than max
 - if no range is entered, all prices should be matched

POSSIBLE IMPROVEMENTS

Output given to the user could be more clear, and contain more information.
If I could completely rewrite the assignment, some aspects of the code would be better organized now that I have experience with GUI.