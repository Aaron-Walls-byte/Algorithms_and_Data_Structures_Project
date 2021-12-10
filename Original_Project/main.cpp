#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int num1 = 1, num2 = 2, num3 = 1, num4 = 1, num5 = 2;
string Bob = "Bob Jones", Sarah = "Sarah Davis", Amy = "Amy Friendly", Johnny = "Johnny Smith", Carol = "Carol Spears";


/*
* validInput
*
*   This function validates user input 
*    
*
*/
int validateInput(string prompt){
    int val;
    cout<<prompt<<endl;
    while(true){
        if(cin >> val){
            break;
        }else{
            cout<<prompt<<endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
    return val;
}


int CheckUserPermissionAccess(){
    int x;
    string password = "123";
    string input;
    cout<<"Enter your username:"<<endl;
    cin>>input;
    cout<<"Enter your password:"<<endl;
    cin>>input;
    if(password.compare(input) == 0){
        x = 1;
    }else{
        x = 2;
    }
    return x;
}

void DisplayInfo(){
    cout<<"  Client's Name  ";
    cout<<"  Service Selected  (1 = Brokerage, 2 = Retirement)"<<endl;
    cout<<"1. "<<Bob<<" selected option "<<num1<<endl;
    cout<<"2. "<<Sarah<<" selected option "<<num2<<endl;
    cout<<"3. "<<Amy<<" selected option "<<num3<<endl;
    cout<<"4. "<<Johnny<<" selected option "<<num4<<endl;
    cout<<"5. "<<Carol<<" selected option "<<num5<<endl;
}

void ChangeCustomerChoice(){
    int clientNum;
    int choiceNum;
    clientNum = validateInput("Enter the number of the client that you wish to change");
    do{
      choiceNum = validateInput("Please enter the client's new service choice (1 = Brokerage, 2 = Retirement");  
    }while((choiceNum < 1)||(choiceNum > 2));       //Make sure the choice is within our defined range
    switch(clientNum){
        case 1:         //Bob
            num1 = choiceNum;
            break;
        case 2:         //Sarah
            num2 = choiceNum;
            break;
        case 3:         //Amy
            num3 = choiceNum;
            break;
        case 4:         //Johnny
            num4 = choiceNum;
            break;
        case 5:         //Carol
            num5 = choiceNum;
            break;
    }
}

int main()
{
    cout<<"Aaron J Walls"<<endl;
    cout<<"Project 1: 410 Reversing Engineering"<<endl;
    cout<<"Hello! Welocme to our Investment Company"<<endl;
    int x;
    do{
         x = CheckUserPermissionAccess();
        if(x != 1){
            cout<<"Invalid Credentials. Please try again"<<endl;
        }
    }while(x != 1);
    
    do{
        cout<<"What would you like to do?"<<endl;
        cout<<"DISPLAY the client list (enter 1)"<<endl;
        cout<<"CHANGE a client's choice (enter 2)"<<endl;
        cout<<"Exit the program.. (enter 3)"<<endl;
        
        x = validateInput("");
        cout<<"You chose "<<x<<endl;
        switch(x){
            
            case 1: 
                DisplayInfo();
                break;
            
            case 2:
                ChangeCustomerChoice();
                break;
                
            case 3: 
                break;
            
        }
    }while(x != 3);
    
    return 0;
}