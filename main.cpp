
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

struct Product{
    int id;
    string name;
    double price;
    int stock;
};

struct CartItem{
    Product product;
    int qty;
};

vector<Product> products;
vector<CartItem> cart;

void loadProducts(){
    products.clear();
    ifstream fin("products.txt");
    string line;
    while(getline(fin,line)){
        stringstream ss(line);
        Product p; string t;
        getline(ss,t,','); p.id=stoi(t);
        getline(ss,p.name,',');
        getline(ss,t,','); p.price=stod(t);
        getline(ss,t,','); p.stock=stoi(t);
        products.push_back(p);
    }
}

void saveProducts(){
    ofstream fout("products.txt");
    for(auto &p:products)
        fout<<p.id<<","<<p.name<<","<<p.price<<","<<p.stock<<"\n";
}

bool loginUser(string u,string p){
    ifstream fin("users.txt");
    string user,pass;
    while(fin>>user>>pass)
        if(user==u && pass==p) return true;
    return false;
}

void registerUser(){
    string u,p;
    cout<<"Username: "; cin>>u;
    cout<<"Password: "; cin>>p;
    ofstream fout("users.txt",ios::app);
    fout<<u<<" "<<p<<"\n";
    cout<<"Registered Successfully\n";
}

void viewProducts(){
    cout<<"\nID\tName\t\tPrice\tStock\n";
    for(auto &p:products)
        cout<<p.id<<"\t"<<p.name<<"\t\t"<<p.price<<"\t"<<p.stock<<"\n";
}

void searchProduct(){
    string key;
    cout<<"Enter product name: ";
    cin>>key;
    for(auto &p:products)
        if(p.name.find(key)!=string::npos)
            cout<<p.id<<" "<<p.name<<" "<<p.price<<"\n";
}

void addToCart(){
    int id,qty;
    cout<<"Product ID: "; cin>>id;
    cout<<"Quantity: "; cin>>qty;

    for(auto &p:products){
        if(p.id==id && p.stock>=qty){
            cart.push_back({p,qty});
            p.stock-=qty;
            saveProducts();
            cout<<"Added to cart\n";
            return;
        }
    }
    cout<<"Invalid selection\n";
}

void removeFromCart(){
    int id;
    cout<<"Product ID to remove: ";
    cin>>id;
    for(size_t i=0;i<cart.size();i++){
        if(cart[i].product.id==id){
            cart.erase(cart.begin()+i);
            cout<<"Removed\n";
            return;
        }
    }
}

void viewCart(){
    double total=0;
    cout<<"\nCart:\n";
    for(auto &c:cart){
        double cost=c.product.price*c.qty;
        total+=cost;
        cout<<c.product.name<<" x "<<c.qty<<" = "<<cost<<"\n";
    }
    cout<<"Total = "<<total<<"\n";
}

void checkout(string user){
    if(cart.empty()){
        cout<<"Cart Empty\n";
        return;
    }

    double total=0;
    cout<<"\n===== BILL =====\n";
    for(auto &c:cart){
        double cost=c.product.price*c.qty;
        total+=cost;
        cout<<c.product.name<<" x "<<c.qty<<" = "<<cost<<"\n";
    }

    double gst=0.18*total;
    double finalAmount=total+gst;

    cout<<"Subtotal: "<<total<<"\n";
    cout<<"GST: "<<gst<<"\n";
    cout<<"Final: "<<finalAmount<<"\n";

    ofstream fout("orders.txt",ios::app);
    fout<<user<<" "<<finalAmount<<"\n";

    cart.clear();
}

void orderHistory(string user){
    ifstream fin("orders.txt");
    string u; double amt;
    while(fin>>u>>amt){
        if(u==user)
            cout<<"Order Amount: "<<amt<<"\n";
    }
}

void adminPanel(){
    string u,p;
    cout<<"Admin Username: "; cin>>u;
    cout<<"Admin Password: "; cin>>p;

    if(u!="admin" || p!="admin123"){
        cout<<"Invalid Admin Login\n";
        return;
    }

    int ch;
    do{
        cout<<"\n1.View Inventory\n2.Add Product\n3.Update Stock\n4.Exit\nChoice: ";
        cin>>ch;

        if(ch==1) viewProducts();

        else if(ch==2){
            Product pr;
            cout<<"Enter ID Name Price Stock:\n";
            cin>>pr.id>>pr.name>>pr.price>>pr.stock;
            products.push_back(pr);
            saveProducts();
        }

        else if(ch==3){
            int id,stock;
            cout<<"Enter ID and New Stock:\n";
            cin>>id>>stock;
            for(auto &p:products)
                if(p.id==id) p.stock=stock;
            saveProducts();
        }

    }while(ch!=4);
}

int main(){
    loadProducts();

    int mainChoice;
    do{
        cout<<"\n1.Register\n2.Login\n3.Admin\n4.Exit\nChoice: ";
        cin>>mainChoice;

        if(mainChoice==1) registerUser();

        else if(mainChoice==2){
            string user,pass;
            cout<<"Username: "; cin>>user;
            cout<<"Password: "; cin>>pass;

            if(!loginUser(user,pass)){
                cout<<"Invalid Login\n";
                continue;
            }

            int ch;
            do{
                cout<<"\n1.View Products\n2.Search Product\n3.Add To Cart\n4.Remove From Cart\n5.View Cart\n6.Checkout\n7.Order History\n8.Logout\nChoice: ";
                cin>>ch;

                switch(ch){
                    case 1:viewProducts();break;
                    case 2:searchProduct();break;
                    case 3:addToCart();break;
                    case 4:removeFromCart();break;
                    case 5:viewCart();break;
                    case 6:checkout(user);break;
                    case 7:orderHistory(user);break;
                }
            }while(ch!=8);
        }

        else if(mainChoice==3) adminPanel();

    }while(mainChoice!=4);

    return 0;
}
