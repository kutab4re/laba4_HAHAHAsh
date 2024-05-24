#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class hash_table {
private:
    int table_size;
    vector<list<pair<string, string>>> table;
    int num_elements;

    // Hash function for strings
    int hash_function(const string& key) const {
        const unsigned long p = 16777619;
        const unsigned long h = 2166136261U;
        unsigned long hash = h;

        for (char ch : key) {
            hash ^= ch;
            hash *= p;
        }
        return hash % table_size;
    }

    // Re-hashing and increasing the table size
    void rehash() {
        int old_size = table_size;
        table_size = next_prime(2 * table_size);
        vector<list<pair<string, string>>> new_table(table_size);

        for (int i = 0; i < old_size; ++i) {
            for (const auto& kv : table[i]) {
                int new_index = hash_function(kv.first);
                new_table[new_index].emplace_back(kv);
            }
        }

        table = std::move(new_table);
        cout << "Rehashed to new table size: " << table_size << endl;
    }

    // Check for a prime number
    bool is_prime(int n) const {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    // Get the next prime number
    int next_prime(int n) const {
        while (!is_prime(n)) {
            n++;
        }
        return n;
    }

public:
    // Constructor for initialising a hash table of a given size
    hash_table(int size) : table_size(next_prime(size)), table(table_size), num_elements(0) {}

    // Inserting an item into the hash table
    void insert_item(const string& key, const string& value) {
        if (num_elements > table_size * 0.7) { // 70% fill rate
            rehash();
        }
        int index = hash_function(key);
        table[index].emplace_back(key, value);
        num_elements++;
    }

    // Deleting an item from the hash table by key
    void delete_item(const string& key) {
        int index = hash_function(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                table[index].erase(it);
                num_elements--;
                cout << "Key \"" << key << "\" deleted." << endl;
                return;
            }
        }
        cout << "Key \"" << key << "\" not found." << endl;
    }

    // Search for an element in the hash table by key
    string search_item(const string& key) const {
        int index = hash_function(key);
        for (const auto& kv : table[index]) {
            if (kv.first == key) {
                return kv.second;
            }
        }
        return "Key not found";
    }

    // Display the contents of the hash table
    void display_hash_table() const {
        for (int i = 0; i < table_size; ++i) {
            cout << "Index " << i << ":";
            for (const auto& kv : table[i]) {
                cout << " -> (" << kv.first << ", " << kv.second << ")";
            }
            cout << endl;
        }
    }

    // Filling the hash table with random elements
    void fill_random(int numItems) {
        srand(time(0));
        for (int i = 0; i < numItems; ++i) {
            string key = generate_random_string(10);
            string value = "value" + to_string(i);
            insert_item(key, value);
        }
    }

    // Generation of a random string of the specified length
    string generate_random_string(size_t length) const {
        const string feature = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        string random_string;
        for (size_t i = 0; i < length; ++i) {
            random_string += feature[rand() % feature.length()];
        }
        return random_string;
    }
};

// Menu display function
void show_menu() {
    cout << "\nMenu:\n";
    cout << "1. Insert item\n";
    cout << "2. Delete item\n";
    cout << "3. Search item\n";
    cout << "4. Display table\n";
    cout << "5. Fill with random items\n";
    cout << "6. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    int size;
    while (true) {
        cout << "Enter size of hash table (1-1000): ";
        cin >> size;
        if (size >= 1 && size <= 1000) {
            break;
        }
        else {
            cout << "Invalid size. Please enter a number between 1 and 1000." << endl;
        }
    }

    hash_table ht(size);
    int choice, num_items;
    string key, value;

    while (true) {
        show_menu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter key: ";
            cin >> key;
            cout << "Enter value: ";
            cin >> value;
            ht.insert_item(key, value);
            break;
        case 2:
            cout << "Enter key to delete: ";
            cin >> key;
            ht.delete_item(key);
            break;
        case 3:
            cout << "Enter key to search: ";
            cin >> key;
            cout << "Value: " << ht.search_item(key) << endl;
            break;
        case 4:
            ht.display_hash_table();
            break;
        case 5:
            cout << "Enter number of random items to insert: ";
            cin >> num_items;
            ht.fill_random(num_items);
            break;
        case 6:
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
