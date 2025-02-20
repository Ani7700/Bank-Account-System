# Bank-Account-System

## Overview
This project is a **Finance Management System** implemented in C++. It allows users to track income and expenses, maintain financial records, and perform various financial operations such as inserting, searching, updating, and deleting records.

## Features
- **Insert Income and Expenses**: Users can add financial records with details such as date, amount, and purpose.
- **Search by Date**: Retrieve financial records based on a specific date.
- **Update Records**: Modify existing financial records.
- **Delete Records**: Remove specific financial records from the system.
- **Balance Calculation**: Automatically compute and display total balance.
- **Persistent Storage**: Financial records are saved to a file (`st.txt`) for future retrieval.

## File Structure
- `Finance.cpp`: The main implementation file containing all functionalities.
- `Finance.h`: Header file (if included) for class declarations.
- `st.txt`: Storage file where financial data is saved and loaded from.

## How to Build and Run
### **Compilation**
To compile the project using `g++`:
```bash
g++ -o finance_manager Finance.cpp
```
### **Execution**
To run the compiled program:
```bash
./finance_manager
```

## Usage Guide
Upon running the program, you will be presented with a menu:
```
Money Manager

<1> Insert income records
<2> Insert expenditure records
<3> Display income records
<4> Display expenditure records
<5> Display balance
<6> Search records by date
<7> Change records by date
<8> Delete records by date
<0> Exit
```
Select an option by entering the corresponding number.

### **Example Commands**
1. **Insert a new income record**
   - Select `1`
   - Enter the date (year, month, day)
   - Enter the amount and purpose
2. **Search a record by date**
   - Select `6`
   - Input the specific date to retrieve records
3. **Update an existing record**
   - Select `7`
   - Enter the date of the record to modify
   - Provide new details
4. **Delete a record**
   - Select `8`
   - Enter the date of the record to remove

## Data Storage
Financial records are stored in the `st.txt` file. Each transaction entry includes:
- Date (Year, Month, Day)
- Income amount
- Expenditure amount
- Balance
- Purpose of transaction
