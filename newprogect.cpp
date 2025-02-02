#include <iostream>
using namespace std;
// Struct for users
struct User {
    string name;
    int ID;
    User* next;
};
// Struct for books
struct Book {
    int id;
    string title;
    string author;
    bool available;
    Book* next;
    User* waitingListFront; // Queue of waiting users for the book
    User* waitingListRear;
}; 
// Stack of books (for the library)
Book* top = NULL;
// Head of the user list
User* userHead = NULL;
// Add a user to the system
void addUser(int ID, string name) {
    User* newUser = new User;
    newUser->ID = ID;
    newUser->name = name;
    newUser->next = userHead;
    userHead = newUser;
    cout << "User added: " << name << " with ID " << ID << endl;
}
// Add a book to the library
void addBook(int id, string title, string author) {
    Book* newBook = new Book;
    newBook->id = id;
    newBook->title = title;
    newBook->author = author;
    newBook->available = true;
    newBook->next = top;
    newBook->waitingListFront = newBook->waitingListRear = NULL; // Initialize waiting list
    top = newBook;

    cout << "Book: " << title << " added by " << author << endl;
}
// Search for a book by ID
Book* searchBookById(int id) {
    Book* current = top;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
// Add a user to the waiting list for a book
void addToWaitingList(Book* book, User* user) {
    User* newUser = new User;
    newUser->ID = user->ID;
    newUser->name = user->name;
    newUser->next = NULL;

    if (book->waitingListRear == NULL) {
        book->waitingListFront = book->waitingListRear = newUser;
    }
    else {
        book->waitingListRear->next = newUser;
        book->waitingListRear = newUser;
    }
}
// Borrow a book
void borrowBook(int userId, int bookId) {
    Book* book = searchBookById(bookId);
    if (book == NULL) {
        cout << "Book not found." << endl;
        return;
    }
    if (book->available) {
        book->available = false;
        cout << "User " << userId << " borrowed the book: " << book->title << endl;
    }
    else {
        cout << "Book is currently unavailable. Adding to the waiting list." << endl;
        // Find the user
        User* user = userHead;
        while (user != NULL && user->ID != userId) {
            user = user->next;
        }
        if (user == NULL) {
            cout << "User not found." << endl;
            return;
        }

        addToWaitingList(book, user);
        cout << "User " << userId << " is now on the waiting list for the book: " << book->title << endl;
    }
}
// Delete a book
void deleteBook(int id) {
    if (top == NULL) {
        cout << "Library is empty." << endl;
        return;
    }
    Book* current = top;
    Book* prev = NULL;
    while (current != NULL && current->id != id) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        cout << "Book not found." << endl;
        return;
    }

    if (prev == NULL) {
        top = current->next; // Delete the top book
    }
    else {
        prev->next = current->next;
    }
    // Free waiting list
    User* temp;
    while (current->waitingListFront != NULL) {
        temp = current->waitingListFront;
        current->waitingListFront = current->waitingListFront->next;
        delete temp;
    }
    delete current;
    cout << "Book with ID: " << id << " deleted." << endl; }
// Return a book and notify waiting users
void returnBook(int bookId) {
    Book* book = searchBookById(bookId);
    if (book == NULL) {
        cout << "Book not found." << endl;
        return;
    }
    book->available = true;
    cout << "Book with ID " << bookId << " returned." << endl;

    if (book->waitingListFront != NULL) {
        User* waitingUser = book->waitingListFront;
        book->waitingListFront = book->waitingListFront->next;
        if (book->waitingListFront == NULL) {
            book->waitingListRear = NULL;
        }

        cout << "User " << waitingUser->ID << " can now borrow the book: " << book->title << endl;
        delete waitingUser;
    }
}
int main() {
    addBook(1, "The Great Gatsby", "F. Scott Fitzgerald");
    addBook(2, "To Kill a Mockingbird", "Harper Lee");
    addBook(3, "1984", "George Orwell");
    //loop for the user registeration 
    int id;
    string name;
    char Y = 'y', N = 'n', a;
    do {
        cout << "Enter user ID: ";
        cin >> id;

        cout << "Enter user's name: ";
        cin >> name;

        addUser(id, name);
        cout << "Do you want to enter a user (y/n): ";
        cin >> a;
    } while (a != N);
    //loop to make the users borrow a book
    char b, c;
    bool cont_loop = true;
    cout << "Do you want to borrow a book (y/n) : ";
    cin >> c;
    if (c == Y) {
        while (cont_loop) {
            int id1, id2;
            cout << "please enter your id: ";
            cin >> id1;
            cout << "please enter the id of the book you want to borrow: ";
            cin >> id2;
            borrowBook(id1, id2);
            cout << "Do you want to borrow another book (y/n): ";
            cin >> b;
            if (b == N) {
                cont_loop = false;
            }
        }
    }
    //loop to make the users return the book they borrowed
    char d, e;
    cont_loop = true;
    cout << "Do you want to return a book (y/n): ";
    cin >> d;
    if (d == Y) {
        while (cont_loop) {
            int id3;
            cout << "please enter the id of the book you want to return: ";
            cin >> id3;
            returnBook(id3);
            cout << "Do you want to return another book (y/n): ";
            cin >> e;
            if (e == N) {
                cont_loop = false;
            }
        }
    }
    // Delete a book
    deleteBook(2); // Delete book with ID 2
    return 0;
}