#include<bits/stdc++.h>
using namespace std;

struct Book {
    string title;
    string author;
    string ISBN;
    bool isAvailable;
};

struct Borrower {
    string name;
    int id;
    unordered_map<string, time_t> borrowedBooks; // ISBN -> borrow date
};

class Library {
private:
    vector<Book> books;
    unordered_map<int, Borrower> borrowers;
    double finePerDay = 1.0; // Fine amount per overdue day

public:
    void addBook(const string& title, const string& author, const string& ISBN) {
        books.push_back({title, author, ISBN, true});
    }
    
    void searchBook(const string& query) {
        for (const auto& book : books) {
            if (book.title.find(query) != string::npos || book.author.find(query) != string::npos || book.ISBN == query) {
                cout << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.ISBN << ", Available: " << (book.isAvailable ? "Yes" : "No") << endl;
            }
        }
    }
    
    void addBorrower(const string& name, int id) {
        borrowers[id] = {name, id, {}};
    }
    
    void checkoutBook(int borrowerId, const string& ISBN) {
        if (borrowers.find(borrowerId) == borrowers.end()) {
            cout << "Borrower not found." << endl;
            return;
        }
        
        for (auto& book : books) {
            if (book.ISBN == ISBN && book.isAvailable) {
                book.isAvailable = false;
                borrowers[borrowerId].borrowedBooks[ISBN] = time(0);
                cout << "Book checked out successfully." << endl;
                return;
            }
        }
        cout << "Book not available." << endl;
    }
    
    void returnBook(int borrowerId, const string& ISBN) {
        if (borrowers.find(borrowerId) == borrowers.end() || borrowers[borrowerId].borrowedBooks.find(ISBN) == borrowers[borrowerId].borrowedBooks.end()) {
            cout << "Invalid return attempt." << endl;
            return;
        }
        
        time_t borrowedTime = borrowers[borrowerId].borrowedBooks[ISBN];
        time_t currentTime = time(0);
        double daysElapsed = difftime(currentTime, borrowedTime) / (60 * 60 * 24);
        
        double fine = (daysElapsed > 14) ? (daysElapsed - 14) * finePerDay : 0;
        
        for (auto& book : books) {
            if (book.ISBN == ISBN) {
                book.isAvailable = true;
                break;
            }
        }
        
        borrowers[borrowerId].borrowedBooks.erase(ISBN);
        
        cout << "Book returned successfully." << endl;
        if (fine > 0) {
            cout << "Fine due: $" << fine << endl;
        }
    }
};

int main() {
    Library library;
    library.addBook("The Great Gatsby", "F. Scott Fitzgerald", "123456789");
    library.addBook("1984", "George Orwell", "987654321");
    
    library.addBorrower("Alice", 1);
    library.addBorrower("Bob", 2);
    
    cout << "Searching for '1984':" << endl;
    library.searchBook("1984");
    
    cout << "Checking out '1984' to Alice:" << endl;
    library.checkoutBook(1, "987654321");
    
    cout << "Returning '1984':" << endl;
    library.returnBook(1, "987654321");
    
    return 0;
}