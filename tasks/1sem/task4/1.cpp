#include <iostream>

using std::cin;
using std::cout;
using std::string;


// HashTable v0.0.1
class HashTable{
public:
    HashTable();
    ~HashTable();

    bool        insert(string key);
    bool        contains(string key);
    bool        remove(string key);

    void        print();

private:
    struct Node{
        Node(string key);

        string  val;
        bool    deleted;
    };

    void        rehash();

    int         hash(string key);

    int         bufferSize_;
    int         size_;

    Node **      nodes_;
};

/****************************MAIN**********************************************/

int main(){
    HashTable hashTable;

    while (!cin.eof()) {
        char   command;
        string str;

        cin >> command;
        cin >> str;

        if (str.empty()) break;

        switch(command){
            case '+':
                if(hashTable.insert(str))
                    cout << "OK\n";
                else
                    cout << "FAIL\n";
                break;
            case '-':
                if(hashTable.remove(str))
                    cout << "OK\n";
                else
                    cout << "FAIL\n";
                break;
            case '?':
                if(hashTable.contains(str))
                    cout << "OK\n";
                else
                    cout << "FAIL\n";
                break;
            default:
                break;
        }
    }
    return 0;
}

/****************************HASHTABLE*****************************************/

// Public:

HashTable::HashTable():
    bufferSize_(8),
    size_(0)
{
    nodes_ = new Node*[bufferSize_];
    for(size_t i = 0; i < bufferSize_; i++)
        nodes_[i] = nullptr;
}



HashTable::~HashTable(){
    for (int i = 0; i < bufferSize_; ++i)
        if (nodes_[i] != nullptr)
            delete nodes_[i];
    delete[] nodes_;
}



bool HashTable::insert(string key){
    if(contains(key))
        return false;

    if(3*bufferSize_ <= 4*(size_ + 1))
        rehash();

    int hashed = hash(key);
    int i = 0;

    while(i < bufferSize_){
        if(nodes_[hashed] == NULL){
            nodes_[hashed] = new Node(key);
            size_++;
            return true;
        }
        else if(nodes_[hashed]->deleted){
            nodes_[hashed]->val = key;
            nodes_[hashed]->deleted = false;
            size_++;
            return true;
        }
        i++;
        hashed = (hashed + i) % bufferSize_;
    }
    return false;
}



bool HashTable::contains(string key){
    int hashed = hash(key);
    int i = 0;
    while(nodes_[hashed] != NULL && i < bufferSize_){
        if(!nodes_[hashed]->deleted && nodes_[hashed]->val.compare(key) == 0)
            return true;
        i++;
        hashed = (hashed + i) % bufferSize_;
    }
    return false;
}



bool HashTable::remove(string key){
    if(!contains(key))
        return false;

    int hashed = hash(key);
    int i = 0;

    while(i < bufferSize_){
        if(!nodes_[hashed]->deleted && nodes_[hashed]->val.compare(key) == 0){
            nodes_[hashed]->deleted = true;
            size_--;
            return true;
        }
        i++;
        hashed = (hashed + i) % bufferSize_;
    }
    return false;
}




void HashTable::print(){
    cout << "Alfa: " << (double) size_ / bufferSize_
         << "; Size: " << size_
         << "; BufferSize: " << bufferSize_ << '\n';
    for(int i = 0; i < bufferSize_; i++)
        if(nodes_[i] == NULL)
            cout << "NULL \n";
        else
            cout << "KEY: " << nodes_[i]->val
                 << "; DELETED: " << nodes_[i]->deleted
                 << "; ADDR: " << nodes_[i] << '\n';
}

// Private:

void HashTable::rehash(){
    Node ** tmp = nodes_;
    bufferSize_ *= 2;
    size_ = 0;
    nodes_ = new Node*[bufferSize_];

    for(int i = 0; i < bufferSize_; i++)
        nodes_[i] = NULL;

    for(int i = 0; i < bufferSize_ / 2; i++)
        if(tmp[i] != NULL){
            if(!tmp[i]->deleted)
                insert(tmp[i]->val);
            delete tmp[i];
        }

    delete[] tmp;
}



int HashTable::hash(string key){
    int hashed = 0;
    for (size_t i = 0; i < key.length(); i++) {
        hashed = (hashed + 127 * key[i]) % bufferSize_;
    }
    return hashed;
}

/****************************HASHTABLE::NODE***********************************/

HashTable::Node::Node(string key) :
    val(key),
    deleted(false)
{}
