#include <iostream>
#include <iterator>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <vector>


using namespace std;

class UserDataNode {
    
public:
    int key;
    string name;
    int choice;

    UserDataNode *next;
};

class MessageDigest
{
public:
    static string sha2ForString(std::string& theString, int length);
    static string messageDigestStringFromData(vector<uint8_t> data);
};

std::string MessageDigest::sha2ForString(std::string& theString, int length){
    string shaString;
    if (!theString.empty()){
        EVP_MD_CTX* context = EVP_MD_CTX_create();
        EVP_MD_CTX_init(context);
        if (context){
            EVP_MD* type = NULL;
            switch (length){
            case SHA512_DIGEST_LENGTH:
                type = (EVP_MD*)EVP_sha512();
                break;
            case SHA384_DIGEST_LENGTH:
                type = (EVP_MD*)EVP_sha384();
                break;
            case SHA256_DIGEST_LENGTH:
                type = (EVP_MD*)EVP_sha256();
                break;
            case SHA224_DIGEST_LENGTH:
                type = (EVP_MD*)EVP_sha224();
                break;     
            default:
                type = (EVP_MD*)EVP_sha512();
                break;
            
            }
            if (EVP_DigestInit_ex(context, type, NULL)){
                if (EVP_DigestUpdate(context, theString.c_str(), theString.length())){
                    unsigned char hash[EVP_MAX_MD_SIZE];
                    unsigned int lengthOfHash = 0;
                    if (EVP_DigestFinal_ex(context, hash, &lengthOfHash)){
                        std::stringstream ss;
                        for (unsigned int i = 0; i < lengthOfHash; ++i){
                            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                        }

                        shaString = ss.str();
                    }
                }
            }
            EVP_MD_CTX_destroy(context);
        }
    }
    return shaString;
}

void push(UserDataNode** head_ref, int key, string name, int choice) {
    //Allocate Node
    UserDataNode* new_node = new UserDataNode();

    //Insert Data
    new_node->key = key;
    new_node->name = name;
    new_node->choice = choice;
    //Assign the current head to nex
    new_node->next = (*head_ref);
    //Make new node the head
    (*head_ref) = new_node;
}

/*
* Updatechoice changes the choice given the head of the linked list and the numeric key. 
* 
* If the key is not unique, it will update the choice assoicatied with the first instance of the key
*/
void update_choice(UserDataNode* head, int key, int newChoice) {
    UserDataNode* current = head;
    while (current != nullptr) {
        if (current->key == key) {
            current->choice = newChoice;
        }
        current = current->next;
    }
}

/*
* validInput
*
*   This function validates user input
*
*/
int validate_input(std::string prompt) {
    int val;
    std::cout << prompt << endl;
    while (true) {
        if (cin >> val) {
            break;
        }
        else {
            cout << prompt << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
    return val;
}


int check_user_permission_access(std::string username, std::string password, std::string salt, std::string user, std::string pass) {
    int x;                                      
    std::string hashed;
    std::string unhashed = pass + salt;
    hashed = MessageDigest::sha2ForString(unhashed, 256);                     //Hash

    //Comparing Hashes
    if (password.compare(hashed) == 0 && username.compare(user) == 0) {
        x = 1;
    }
    else {
        x = 2;
    }
    return x;
}

void change_customer_choice(UserDataNode* head) {
    int clientNum;      //Associated with the Client key    (does not update client if the key does not exist)
    int choiceNum;      //Associated with the Client Choice (can only be 1 or 2)
    clientNum = validate_input("Enter the number of the client that you wish to change");
    do {
        choiceNum = validate_input("Please enter the client's new service choice (1 = Brokerage, 2 = Retirement");
    } while ((choiceNum < 1) || (choiceNum > 2));       //Make sure the choice is within our defined range
    update_choice(head, clientNum, choiceNum);
}
/*
* encrypt or decrypt a source string using the provided key
*
* source = input string to process
* key = key to use in encryption / decryption
* returns encrypted or decrypted text
*/
std::string encrypt_decrypt(const std::string& source, const std::string& key) {
    // get lengths now instead of calling the function every time.
    // this would have most likely been inlined by the compiler, but design for perfomance.
    const auto key_length = key.length();
    const auto source_length = source.length();
    // assert that our input data is good
    assert(key_length > 0);
    assert(source_length > 0);
    std::string output = source;
    // loop through the source string char by char
    for (size_t i = 0; i < source_length; ++i){ 
      // transform each character based on an xor of the key modded constrained to key length using a mod
        output[i] = source[i] ^ key[i % key_length];
    }
    // our output length must equal our source length
    assert(output.length() == source_length);
    // return the transformed string
    return output;
}

std::string read_file(const std::string& filename) {
    std::ifstream inFile;
    inFile.open(filename);                   //Open the input File
    std::stringstream strStream;
    strStream << inFile.rdbuf();             //Read File
    std::string file_text = strStream.str(); //Text of the file

    return file_text;
}
/*
* Ignores the first, second, and third lines of the file,
* breaks the subsequent strings into their parts (lines and words) and pushes the data to the linked list
*/
void load_data(const std::string& string_data,  UserDataNode** head_ref) {
    std::string delimiter = "\n";
    std::string next = "";
    int key = -1;
    int choice = -1;
    std::string fullName = "";
    std::string firstName = "";
    auto start = 0U;
    auto end = string_data.find(delimiter);
    while (end != std::string::npos) {
        stringstream ss(string_data.substr(start, end - start));
        int j = 1;
        while (ss >> next) {
            switch (j) {
            case 1:
                key = stoi(next);
                break;
            case 2:
                firstName = next;
                break;
            case 3:
                fullName = firstName + " " + next;
                break;
            case 4:
                choice = stoi(next);
                break;
            default:
                break;
            }
            if (j == 4) {
                j = 1;
            }
            else {
                j++;
            }
        }
        push(head_ref, key, fullName, choice);
        start = end + delimiter.length();
        end = string_data.find(delimiter, start);
    }
}
std::string get_data(const std::string& string_data) {
    std::string delimiter = "\n";
    std::string next = "";
    std::string data = "";
    int i = 0;
    auto start = 0U;
    auto end = string_data.find(delimiter);
    while (end != std::string::npos) {
        if (i > 2) {
            next = string_data.substr(start, end - start);
            data.append(next + "\n");
        }
        start = end + delimiter.length();
        end = string_data.find(delimiter, start);
        i++;
    }
    //std::cout << data << endl;
    return data;
}
/*
* Returns the Third line of the file
*/
std::string get_salt(const std::string& string_data) {
    std::string delimiter = "\n";
    std::string salt = "t";
    int i = 0;
    auto start = 0U;
    auto end = string_data.find(delimiter);
    while (end != std::string::npos) {
        if (i == 2) {
            salt = string_data.substr(start, end - start);
        }
        start = end + delimiter.length();
        end = string_data.find(delimiter, start);
        i++;
    }
    return salt;
}
/*
* Returns the Second line of the file
*/
std::string get_password(const std::string& string_data) {
    std::string delimiter = "\n";
    std::string password = "t";
    int i = 0;
    auto start = 0U;
    auto end = string_data.find(delimiter);
    while (end != std::string::npos) {
        if (i == 1) {
            password = string_data.substr(start, end - start);
        }
        start = end + delimiter.length();
        end = string_data.find(delimiter, start);
        i++;
    }
    return password;
}

/*
* Returns the first line of the file
*/
std::string get_username(const std::string& string_data) {
    std::string username;

    //find the first newline
    size_t pos = string_data.find('\n');
    if (pos != std::string::npos) {
        username = string_data.substr(0, pos);
    }
    return username;
}
/*Save Data file
*
* Format:
* Username
* Password      
* SALT
* Key First Last Choice
* Key First Last Choice
* etc..
*/

void save_data_file(const std::string& filename, const std::string& username, const std::string& pass, const std::string& data) {
    std::string e_data;
    std::string unhashed, hashed;
    std::string str = std::to_string(rand());
    std::ofstream newFile(filename);
    
    //hash pass with str to create password
    unhashed = pass + str;
    hashed = MessageDigest::sha2ForString(unhashed, 256);
    e_data = encrypt_decrypt(data, hashed);

    newFile << username << std::endl;
    newFile << hashed << std::endl;
    newFile << str << endl;
    newFile << e_data << std::endl;
    newFile.close();
}

void display_info(UserDataNode* n) {
    while (n != NULL) {
        cout << n->key << " " << n->name << " " << n->choice << endl;
        n = n->next;
    }
}

string save_data(UserDataNode* n) {
    string data = "";
    while (n != NULL) {
        data = data + std::to_string(n->key) + " " + n->name + " " + std::to_string(n->choice) + "\n";
        n = n->next;
    }
    return data;
}
int main(){



    UserDataNode* head = NULL;
    cout << "Aaron J Walls" << endl;
    cout << "Project 1: 410 Reversing Engineering" << endl;
    cout << "Hello! Welocme to our Investment Company" << endl;

    string file_txt = read_file("test.acc");
    string username = get_username(file_txt);           //plain text user
    string password = get_password(file_txt);           //Hashed Password (should be equal to pass+salt)
    string salt = get_salt(file_txt);                   //Salt Time stamp
    string e_data = get_data(file_txt);   
    string user;
    string pass;
    string data;
    int x;
    //std::string username, std::string password, std::string salt, std::string user, std::string pass
    do {
        std::cout << "Enter your username:" << endl;
        cin >> user;
        std::cout << "Enter your password:" << endl;
        cin >> pass;
        x = check_user_permission_access(username, password, salt, user, pass);
        if (x != 1) {
            std::cout << "Invalid Credentials. Please try again" << endl;
        }
    } while (x != 1);

    /*
    * Example load function
    */
   /* push(&head, 5, "Carol Spears", 2);
    push(&head, 4, "Johnny Smith", 1);
    push(&head, 3, "Amy Friendly", 1);
    push(&head, 2, "Sarah Davis", 2);
    push(&head, 1, "Bob Jones", 1);*/
    
   /* string data = save_data(head);
    save_data_file("test.txt", "John", "newPassword", data);*/
    data = encrypt_decrypt(e_data, password);
    load_data(data, &head);

    do {
        cout << "What would you like to do?" << endl;
        cout << "DISPLAY the client list (enter 1)" << endl;
        cout << "CHANGE a client's choice (enter 2)" << endl;
        cout << "Exit the program.. (enter 3)" << endl;

        x = validate_input("");
        cout << "You chose " << x << endl;
        switch (x) {

        case 1:
            display_info(head);
            break;

        case 2:
            change_customer_choice(head);
            break;

        case 3:
            
            save_data_file("test.acc", username, pass, save_data(head));
            break;

        }
    } while (x != 3);

    return 0;
}