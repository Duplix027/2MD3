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

// Copy Constructor
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
// Part (i) - OrderByScore (Bubble Sort Approach)
// =========================
void DLinkedList::OrderByScore() {
    if (header->next == trailer || header->next->next == trailer) return; // No need to sort if 0 or 1 elements

    bool swapped;
    do {
        swapped = false;
        DNode* current = header->next;

        while (current->next != trailer) { // Stop before trailer
            if (current->score < current->next->score) {
                // Swap data (names & scores)
                std::swap(current->name, current->next->name);
                std::swap(current->score, current->next->score);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped); // Repeat until no swaps occur
}

// =========================
// Part (h) - OrderByName (Bubble Sort Approach)
// =========================
void DLinkedList::OrderByName() {
    if (header->next == trailer || header->next->next == trailer) return; // No need to sort if 0 or 1 elements

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
// Part (j) - Append (Merging Two Lists Without Copying Nodes)
// =========================
void DLinkedList::Append(DLinkedList &L) {
    if (L.header->next == L.trailer) return;  // L is empty, nothing to append

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
// Part (k) - Reverse (Swap Pointers)
// =========================
void DLinkedList::Reverse() {
    if (header->next == trailer || header->next->next == trailer) return; // No need to reverse if 0 or 1 elements

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

// =========================
// Main Function to Test Sorting by Name, then by Score
// =========================
int main() {
    DLinkedList dll;

    // Adding scores (Initially in random order)
    dll.AddScoreInOrder("Charlie", 85);
    dll.AddScoreInOrder("Alice", 95);
    dll.AddScoreInOrder("Eve", 88);
    dll.AddScoreInOrder("Bob", 90);
    dll.AddScoreInOrder("David", 97);

    std::cout << "List before Sorting:" << std::endl;
    dll.Print();

    // Sorting by name
    std::cout << "\nSorting by Name (A-Z):" << std::endl;
    dll.OrderByName();
    dll.Print();

    // Sorting by score
    std::cout << "\nSorting by Score (Descending):" << std::endl;
    dll.OrderByScore();
    dll.Print();

    return 0;
}
