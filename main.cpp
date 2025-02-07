#include <iostream>
#include <string>

class DNode {
private:
    std::string name;
    int score;
    DNode* prev;
    DNode* next;

    friend class DLinkedList;  // Allow DLinkedList to access private members

    DNode(std::string n = "", int s = 0) : name(n), score(s), prev(nullptr), next(nullptr) {}
};


class DLinkedList {
public:
    DLinkedList();  // Constructor
    DLinkedList(const DLinkedList &source); // Copy Constructor
    ~DLinkedList(); // Destructor

    DLinkedList& operator=(const DLinkedList &source); // Assignment Operator

    int Size();  // Count nodes recursively
    void AddScoreInOrder(std::string name, int score);  // Insert scores in sorted order
    void RemoveScore(int index);  // Remove score at given index
    bool UpdateScore(std::string name, int score);  // Update a player's score and re-sort
    void Print();  // Print all scores

    void OrderByName(); // Sort list by name alphabetically
    void OrderByScore(); // Sort list by score in descending order
    void Append(DLinkedList &L); // Merge two lists
    void Reverse(); // Reverse list order

private:
    DNode* header;  // Sentinel node at the start
    DNode* trailer; // Sentinel node at the end

    int SizeRecursive(DNode* current);  // Recursive helper function for counting
    void PrintRecursive(DNode* current); // Recursive helper function for printing
};


// =========================
// Constructor and Destructor
// =========================
DLinkedList::DLinkedList() {
    header = new DNode();
    trailer = new DNode();
    header->next = trailer;
    trailer->prev = header;
}

// Destructor
DLinkedList::~DLinkedList() {
    while (header->next != trailer) {
        DNode* temp = header->next;
        header->next = temp->next;
        delete temp;
    }
    delete header;
    delete trailer;
}

//------Part J (Copy Constructor)
DLinkedList::DLinkedList(const DLinkedList &source) {
    header = new DNode();
    trailer = new DNode();
    header->next = trailer;
    trailer->prev = header;

    DNode* current = source.header->next;
    while (current != source.trailer) {
        AddScoreInOrder(current->name, current->score);
        current = current->next;
    }
}

//---------Part G (Opperator (=))
DLinkedList& DLinkedList::operator=(const DLinkedList &source) {
    if (this == &source) return *this;

    while (header->next != trailer) {
        DNode* temp = header->next;
        header->next = temp->next;
        delete temp;
    }
    header->next = trailer;
    trailer->prev = header;

    DNode* current = source.header->next;
    while (current != source.trailer) {
        AddScoreInOrder(current->name, current->score);
        current = current->next;
    }

    return *this;
}


// =========================
// Part (a) - Size Function (Recursive)
// =========================
int DLinkedList::Size() {
    return SizeRecursive(header->next);
}

int DLinkedList::SizeRecursive(DNode* current) {
    if (current == trailer) return 0;
    return 1 + SizeRecursive(current->next);
}


// =========================
// Part (b) - AddScoreInOrder (Insert in Sorted Order)
// =========================
void DLinkedList::AddScoreInOrder(std::string name, int score) {
    DNode* newNode = new DNode(name, score);
    DNode* current = header->next;

    while (current != trailer && current->score >= score) {
        current = current->next;
    }

    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
}

// =========================
// Part (c) - RemoveScore (Remove i-th Score)
// =========================
void DLinkedList::RemoveScore(int index) {
    if (header->next == trailer) {
        std::cout << "List is empty. Nothing to remove." << std::endl;
        return;
    }

    DNode* current = header->next;
    int count = 0;

    while (current != trailer && count < index) {
        current = current->next;
        count++;
    }

    if (current == trailer) {
        std::cout << "Invalid index. No node removed." << std::endl;
        return;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
}

// =========================
// Part (d) - UpdateScore (Find and Update a Player's Score)
// =========================
bool DLinkedList::UpdateScore(std::string name, int score) {
    DNode* current = header->next;

    while (current != trailer && current->name != name) {
        current = current->next;
    }

    if (current == trailer) {
        std::cout << "Player '" << name << "' not found. No update performed." << std::endl;
        return false;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    AddScoreInOrder(name, score);
    return true;
}

// =========================
// Part (e) - Print (Recursive Version)
// =========================
void DLinkedList::Print() {
    PrintRecursive(header->next);
    std::cout << std::endl;
}

void DLinkedList::PrintRecursive(DNode* current) {
    if (current == trailer) return;

    std::cout << "{" << current->name << "," << current->score << "}";
    if (current->next != trailer) std::cout << " -> ";

    PrintRecursive(current->next);
}


//-----> F and G placed above for code readbility and debugging

// =========================
// Part (h) - OrderByName (Bubble Sort Approach)
// =========================
void DLinkedList::OrderByName() {
    if (header->next == trailer || header->next->next == trailer) return;

    bool swapped;
    do {
        swapped = false;
        DNode* current = header->next;

        while (current->next != trailer) {
            if (current->name > current->next->name) {
                std::swap(current->name, current->next->name);
                std::swap(current->score, current->next->score);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

// =========================
// Part (i) - OrderByScore (Bubble Sort Approach)
// =========================
void DLinkedList::OrderByScore() {
    if (header->next == trailer || header->next->next == trailer) return;

    bool swapped;
    do {
        swapped = false;
        DNode* current = header->next;

        while (current->next != trailer) {
            if (current->score < current->next->score) {
                std::swap(current->name, current->next->name);
                std::swap(current->score, current->next->score);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

// =========================
// Part (j) - Append
// =========================

void DLinkedList::Append(DLinkedList &L) {
    if (L.header->next == L.trailer) return;

    DNode* lastNode = trailer->prev;
    DNode* firstNodeL = L.header->next;
    DNode* lastNodeL = L.trailer->prev;

    lastNode->next = firstNodeL;
    firstNodeL->prev = lastNode;

    lastNodeL->next = trailer;
    trailer->prev = lastNodeL;

    L.header->next = L.trailer;
    L.trailer->prev = L.header;
}

// =========================
// Part (k) - reverse
// =========================

void DLinkedList::Reverse() {
    if (header->next == trailer || header->next->next == trailer) return;

    DNode* current = header;
    while (current != nullptr) {
        DNode* temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        current = temp;
    }

    std::swap(header, trailer);
}



// =========================
// Main Function
// =========================
int main() {
    std::cout << "=== Creating Linked List ===\n";
    DLinkedList dll;

    std::cout << "\nAdding Scores:\n";
    dll.AddScoreInOrder("Charlie", 85);
    dll.AddScoreInOrder("Alice", 95);
    dll.AddScoreInOrder("Eve", 88);
    dll.AddScoreInOrder("Bob", 90);
    dll.AddScoreInOrder("David", 97);
    dll.Print();

    std::cout << "\nSize of List: " << dll.Size() << std::endl;

    std::cout << "\nSorting by Name (A-Z):\n";
    dll.OrderByName();
    dll.Print();

    std::cout << "\nSorting by Score (Descending):\n";
    dll.OrderByScore();
    dll.Print();

    std::cout << "\nRemoving index 2 (3rd element):\n";
    dll.RemoveScore(2);
    dll.Print();

    std::cout << "\nUpdating Score (Bob to 99):\n";
    dll.UpdateScore("Eve", 99);
    dll.Print();

    std::cout << "\nReversing List:\n";
    dll.Reverse();
    dll.Print();

    std::cout << "\n=== Creating Second List for Append Test ===\n";
    DLinkedList dll2;
    dll2.AddScoreInOrder("Zack", 75);
    dll2.AddScoreInOrder("Olivia", 93);
    dll2.Print();

    std::cout << "\nAppending Second List to First List:\n";
    dll.Append(dll2);
    dll.Print();

    std::cout << "\nSize After Append: " << dll.Size() << std::endl;

    std::cout << "\n=== Testing Copy Constructor ===\n";
    DLinkedList dllCopy(dll);
    dllCopy.Print();

    std::cout << "\n=== Testing Assignment Operator ===\n";
    DLinkedList dllAssigned;
    dllAssigned = dll;
    dllAssigned.Print();

    std::cout << "\n=== Final Test Complete ===\n";
    return 0;
}
