#include <iostream>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <limits>
#include <sstream>
#include <fstream>
using namespace std;

// Constants
const int MAX_STUDENTS = 100;
const int MAX_BOOKS = 100;
const int MAX_NAME_LENGTH = 50;
const int ISSUE_PERIOD = 7;
const double FINE_PER_DAY = 1.0;

// Student data arrays
int student_rolls[MAX_STUDENTS];
char student_names[MAX_STUDENTS][MAX_NAME_LENGTH];
double student_balances[MAX_STUDENTS];
int student_issued_book_index[MAX_STUDENTS];
time_t student_issue_date[MAX_STUDENTS];
int student_count = 0;

// Book data arrays
char book_titles[MAX_BOOKS][MAX_NAME_LENGTH];
char book_authors[MAX_BOOKS][MAX_NAME_LENGTH];
int book_isbns[MAX_BOOKS];
bool book_available[MAX_BOOKS];
time_t book_due_date[MAX_BOOKS];
int book_row[MAX_BOOKS];
int book_column[MAX_BOOKS];
int book_count = 0;

// Function Prototypes
int find_student(int roll);
int find_book(int isbn);
void create_account();
void display(int roll);
void deposit_amount(int roll, double amount);
void issue_item(int roll);
void return_item(int roll, int isbn);
void display_sorted();
void add_book();
void edit_book();
void view_books();
void remove_book();
void save_Data(); // NEW FUNCTION PROTOTYPE

// Helper
void initialize_sample_data() {
    student_rolls[0] = 101;
    strcpy(student_names[0], "Aniqa");
    student_balances[0] = 80;
    student_issued_book_index[0] = -1;
    student_issue_date[0] = 0;
    student_count=1;

    strcpy(book_titles[0], "C++ Programming");
    strcpy(book_authors[0], "Bjarne Stroustrup");
    book_isbns[0] = 1111;
    book_available[0] = true;
    book_row[0] = 1;
    book_column[0] = 1;
    book_due_date[0] = 0;

    strcpy(book_titles[1], "clean code");
    strcpy(book_authors[1], "Robert C. Martin");
    book_isbns[1] = 2222;
    book_available[1] = true;
    book_row[1] = 1;
    book_column[1] = 2;
    book_due_date[1] = 0;

    book_count = 2;
}

int main() {
    initialize_sample_data();
    cout << "********** Welcome to Library Management System **********\n";

    int main_option;
    string password;
    bool is_admin;

    do {
        cout << "\nLogin as:\n1. Admin\n2. Student\n0. Exit\nEnter your option: ";
        cin >> main_option;

        if (main_option == 0) break;

        is_admin = (main_option == 1);

        cout << "Enter password: ";
        cin >> password;

        if (password != "password") {
            cout << "Incorrect password.\n";
            continue;
        }

        if (is_admin) {
            int admin_option;
            do {
                cout << "\nAdmin Menu:\n1. Add Book\n2. Edit Book\n3. View Books\n4. View Students\n5. View Balance\n6. Remove Book\n0. Logout\nChoice: ";
                cin >> admin_option;
                switch (admin_option) {
                    case 1: add_book(); save_Data(); break;
                    case 2: edit_book(); save_Data(); break;
                    case 3: view_books(); break;
                    case 4: display_sorted(); break;
                    case 5: {
                        int roll;
                        cout << "Enter student roll: ";
                        cin >> roll;
                        display(roll);
                        break;
                    }
                    case 6: remove_book(); save_Data(); break;
                    case 0: cout << "Logged out.\n"; break;
                    default: cout << "Invalid option.\n"; break;
                }
            } while (admin_option != 0);
        } else {
            int roll;
            cout << "Enter your roll number: ";
            cin >> roll;

            int idx = find_student(roll);
            if (idx == -1) {
                int choice;
                cout << "Student not found. Create account? (1.Yes / 2.No): ";
                cin >> choice;
                if (choice == 1) { create_account(); save_Data(); }
                continue;
            }

            int student_option;
            do {
                cout << "\nStudent Menu:\n1. View Balance\n2. Deposit\n3. Issue Book\n4. Return Book\n0. Logout\nChoice: ";
                cin >> student_option;
                switch (student_option) {
                    case 1: display(roll); break;
                    case 2: {
                        double amt;
                        cout << "Enter amount to deposit: ";
                        cin >> amt;
                        deposit_amount(roll, amt);
                        save_Data();
                        break;
                    }
                    case 3: issue_item(roll); save_Data(); break;
                    case 4: {
                        int isbn;
                        cout << "Enter ISBN to return: ";
                        cin >> isbn;
                        return_item(roll, isbn);
                        save_Data();
                        break;
                    }
                    case 0: cout << "Logged out.\n"; break;
                    default: cout << "Invalid option.\n"; break;
                }
            } while (student_option != 0);
        }
    } while (true);

    // Save data before exiting
    save_Data();

    return 0;
}

// Function Definitions

int find_student(int roll) {
    for (int i = 0; i < student_count; i++)
        if (student_rolls[i] == roll) return i;
    return -1;
}

int find_book(int isbn) {
    for (int i = 0; i < book_count; i++)
        if (book_isbns[i] == isbn) return i;
    return -1;
}

void create_account() {
    if (student_count >= MAX_STUDENTS) {
        cout << "Student limit reached.\n";
        return;
    }

    int roll;
    cout << "Enter roll number: ";
    cin >> roll;
    if (find_student(roll) != -1) {
        cout << "Roll number already exists.\n";
        return;
    }

    cin.ignore();
    cout << "Enter name: ";
    cin.getline(student_names[student_count], MAX_NAME_LENGTH);

    double deposit;
    cout << "Enter deposit (min $50): ";
    cin >> deposit;
    if (deposit < 50) {
        cout << "Deposit must be at least $50.\n";
        return;
    }

    student_rolls[student_count] = roll;
    student_balances[student_count] = deposit - 50;
    student_issued_book_index[student_count] = -1;
    student_issue_date[student_count] = 0;
    student_count++;
    cout << "Account created successfully.\n";
}

void display(int roll) {
    int i = find_student(roll);
    if (i == -1) {
        cout << "Student not found.\n";
        return;
    }
    cout << "Roll: " << student_rolls[i] << "\nName: " << student_names[i] << "\nBalance: $" << fixed << setprecision(2) << student_balances[i] << "\n";
}

void deposit_amount(int roll, double amount) {
    int i = find_student(roll);
    if (i != -1) {
        student_balances[i] += amount;
        cout << "Updated Balance: $" << fixed << setprecision(2) << student_balances[i] << "\n";
    } else {
        cout << "Student not found.\n";
    }
}

void issue_item(int roll) {
    int i = find_student(roll);
    if (i == -1) return;

    if (student_issued_book_index[i] != -1) {
        cout << "Already issued a book.\n";
        return;
    }

    cout << "Available Books:\n";
    int count = 0;
    for (int j = 0; j < book_count; j++) {
        if (book_available[j]) {
            cout << ++count << ". " << book_titles[j] << " by " << book_authors[j] << " (ISBN: " << book_isbns[j] << ") Pos: Row " << book_row[j] << ", Col " << book_column[j] << "\n";
        }
    }

    if (count == 0) {
        cout << "No books available.\n";
        return;
    }

    int choice;
    cout << "Select book number to issue: ";
    cin >> choice;

    int selected = -1;
    count = 0;
    for (int j = 0; j < book_count; j++) {
        if (book_available[j]) {
            count++;
            if (count == choice) {
                selected = j;
                break;
            }
        }
    }

    if (selected == -1) {
        cout << "Invalid choice.\n";
        return;
    }

    if (student_balances[i] < 2) {
        cout << "Insufficient balance to issue.\n";
        return;
    }

    book_available[selected] = false;
    student_balances[i] -= 2;
    time_t now = time(0);
    book_due_date[selected] = now + (ISSUE_PERIOD * 24 * 60 * 60);
    student_issued_book_index[i] = selected;
    student_issue_date[i] = now;

    cout << "Book issued. Due Date: " << ctime(&book_due_date[selected]);
}

void return_item(int roll, int isbn) {
    int i = find_student(roll);
    int j = find_book(isbn);
    if (i == -1 || j == -1 || student_issued_book_index[i] != j) {
        cout << "Invalid return.\n";
        return;
    }

    time_t now = time(0);
    double fine = 0;
    if (now > book_due_date[j]) {
        fine = (now - book_due_date[j]) / (24 * 60 * 60) * FINE_PER_DAY;
    }

    book_available[j] = true;
    student_issued_book_index[i] = -1;
    student_issue_date[i] = 0;

    student_balances[i] -= fine;
    cout << "Book returned. Fine: $" << fixed << setprecision(2) << fine << "\n";
}

void display_sorted() {
    for (int i = 0; i < student_count - 1; i++) {
        for (int j = i + 1; j < student_count; j++) {
            if (student_rolls[i] > student_rolls[j]) {
                swap(student_rolls[i], student_rolls[j]);
                char temp[MAX_NAME_LENGTH];
                strcpy(temp, student_names[i]);
                strcpy(student_names[i], student_names[j]);
                strcpy(student_names[j], temp);
                swap(student_balances[i], student_balances[j]);
                swap(student_issued_book_index[i], student_issued_book_index[j]);
                swap(student_issue_date[i], student_issue_date[j]);
            }
        }
    }

    for (int i = 0; i < student_count; i++) {
        cout << student_rolls[i] << " - " << student_names[i] << " - $" << fixed << setprecision(2) << student_balances[i] << "\n";
    }
}

void add_book() {
    if (book_count >= MAX_BOOKS) {
        cout << "Book limit reached.\n";
        return;
    }

    cin.ignore();
    cout << "Enter title: ";
    cin.getline(book_titles[book_count], MAX_NAME_LENGTH);
    cout << "Enter author: ";
    cin.getline(book_authors[book_count], MAX_NAME_LENGTH);
    cout << "Enter ISBN: ";
    cin >> book_isbns[book_count];
    cout << "Enter row and column: ";
    cin >> book_row[book_count] >> book_column[book_count];
    book_available[book_count] = true;
    book_due_date[book_count] = 0;
    book_count++;
    cout << "Book added.\n";
}

void edit_book() {
    int isbn;
    cout << "Enter ISBN to edit: ";
    cin >> isbn;
    int i = find_book(isbn);
    if (i == -1) {
        cout << "Book not found.\n";
        return;
    }

    cin.ignore();
    cout << "Enter new title: ";
    cin.getline(book_titles[i], MAX_NAME_LENGTH);
    cout << "Enter new author: ";
    cin.getline(book_authors[i], MAX_NAME_LENGTH);
    cout << "Enter new row and column: ";
    cin >> book_row[i] >> book_column[i];
    cout << "Book updated.\n";
}

void view_books() {
    for (int i = 0; i < book_count; i++) {
        cout << book_titles[i] << " by " << book_authors[i] << " (ISBN: " << book_isbns[i]
             << ") - " << (book_available[i] ? "Available" : "Issued")
             << " Pos: Row " << book_row[i] << ", Col " << book_column[i] << "\n";
    }
}

void remove_book() {
    int isbn;
    cout << "Enter ISBN to remove: ";
    cin >> isbn;
    int i = find_book(isbn);
    if (i == -1) {
        cout << "Book not found.\n";
        return;
    }

    for (int j = i; j < book_count - 1; j++) {
        strcpy(book_titles[j], book_titles[j + 1]);
        strcpy(book_authors[j], book_authors[j + 1]);
        book_isbns[j] = book_isbns[j + 1];
        book_available[j] = book_available[j + 1];
        book_due_date[j] = book_due_date[j + 1];
        book_row[j] = book_row[j + 1];
        book_column[j] = book_column[j + 1];
    }
    book_count--;
    cout << "Book removed successfully.\n";
}
// SAVE DATA FUNCTION
// --------------------------
void save_Data() {
    // Save student data to students.txt
    ofstream student_ofs("students.txt");
    if (!student_ofs) {
        cout << "Error saving student data to students.txt\n";
    } else {
        student_ofs << "Total Students: " << student_count << "\n";
        for (int i = 0; i < student_count; i++) {
            student_ofs << "Roll: " << student_rolls[i] << "\n";
            student_ofs << "Name: " << student_names[i] << "\n";
            student_ofs << "Balance: " << student_balances[i] << "\n";
            student_ofs << "Issued Book Index: " << student_issued_book_index[i] << "\n";
            student_ofs << "Issue Date: " << student_issue_date[i] << "\n";
            student_ofs << "-----------------------------\n";
        }
        student_ofs.close();
        cout << "Student data saved to students.txt\n";
    }

    // Save book data to books.txt
    ofstream book_ofs("books.txt");
    if (!book_ofs) {
        cout << "Error saving book data to books.txt\n";
    } else {
        book_ofs << "Total Books: " << book_count << "\n";
        for (int i = 0; i < book_count; i++) {
            book_ofs << "Title: " << book_titles[i] << "\n";
            book_ofs << "Author: " << book_authors[i] << "\n";
            book_ofs << "ISBN: " << book_isbns[i] << "\n";
            book_ofs << "Available: " << (book_available[i] ? "Yes" : "No") << "\n";
            book_ofs << "Due Date: " << book_due_date[i] << "\n";
            book_ofs << "Row: " << book_row[i] << "\n";
            book_ofs << "Column: " << book_column[i] << "\n";
            book_ofs << "-----------------------------\n";
        }
        book_ofs.close();
        cout << "Book data saved to books.txt\n";
    }
}
