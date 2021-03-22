#include "../include/Customer.h"
#include <climits>
#include <iostream>

/*constructor*/
Customer::Customer(std::string c_name, int c_id): name(c_name), id(c_id){}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

Customer::~Customer() {}

/*VegetarianCustomer constructor*/
VegetarianCustomer::VegetarianCustomer(std::string name, int id): Customer(name, id){}

VegetarianCustomer::VegetarianCustomer(const VegetarianCustomer &other): VegetarianCustomer(other.getName(), other.getId()){}

VegetarianCustomer::~VegetarianCustomer() {}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> ord;
    int minVegId=INT_MAX;
    int maxBvgPrice=0;
    int idOfMaxBvg=-1;
    bool isVeg=false;
    for (std::size_t i=0, max=menu.size();i<max;i++){
        if ((menu[i].getType()==VEG)&&(menu[i].getId()<minVegId)){
            minVegId=menu[i].getId();
            isVeg=true;
        }
        if(((menu[i].getType()==BVG)&&(menu[i].getPrice()>maxBvgPrice))||((menu[i].getType()==BVG)&&(menu[i].getPrice()==maxBvgPrice)&&(menu[i].getId()<idOfMaxBvg))){
            maxBvgPrice=menu[i].getPrice();
            idOfMaxBvg=menu[i].getId();
        }
    }
    if(isVeg) {
        ord.push_back(minVegId);
        if (idOfMaxBvg!=-1)
            ord.push_back(idOfMaxBvg);
    }
    return ord;
}

std::string VegetarianCustomer::toString() const {
    std::string s;
    s="Name: "+(*this).getName()+", ID: "+std::to_string((*this).getId())+", Type: VEG";
    return s;
}

Customer* VegetarianCustomer::clone() const {
    return new VegetarianCustomer(*this);
}

/*CheapCustomer constructor*/
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name, id), orderCount(0){}

CheapCustomer::CheapCustomer(const CheapCustomer &other): CheapCustomer(other.getName(), other.getId()) {}

CheapCustomer::~CheapCustomer() {}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> ord;
    if(orderCount==0){
        int minPrice=INT_MAX;
        int idOfMinPrice=-1;
        for(std::size_t i=0, max=menu.size();i<max;i++){
            if ((menu[i].getPrice()<minPrice)||((menu[i].getPrice()==minPrice)&&(menu[i].getId()<idOfMinPrice))) {
                minPrice = menu[i].getPrice();
                idOfMinPrice=menu[i].getId();
            }
        }
        if(idOfMinPrice!=-1) {
            ord.push_back(idOfMinPrice);
            orderCount++;
        }
    }
    return ord;
}

std::string CheapCustomer::toString() const {
    std::string s;
    s="Name: "+(*this).getName()+", ID: "+std::to_string((*this).getId())+", Type: CHP";
    return s;
}

Customer* CheapCustomer::clone() const {
    return new CheapCustomer(*this);
}

/*SpicyCustomer constructor*/
SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name, id),orderCount(0) {}

SpicyCustomer::SpicyCustomer(const SpicyCustomer &other): SpicyCustomer(other.getName(), other.getId()) {}

SpicyCustomer::~SpicyCustomer() {}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> ord;
    if(orderCount==0){
        int maxPrice=0;
        int idOfMaxPrice=-1;
        for (std::size_t i=0, max=menu.size();i<max;i++){
            if (((menu[i].getType()==SPC)&&(menu[i].getPrice()>maxPrice))||((menu[i].getType()==SPC)&&(menu[i].getPrice()==maxPrice)&&(menu[i].getId()<idOfMaxPrice))){
                maxPrice=menu[i].getPrice();
                idOfMaxPrice=menu[i].getId();
            }
        }
        if(idOfMaxPrice!=-1) {
            ord.push_back(idOfMaxPrice);
            orderCount++;
        }
    }
    else{
        int minPrice=INT_MAX;
        int idOfMinPrice=-1;
        for(std::size_t i=0, max=menu.size();i<max;i++){
            if (((menu[i].getType()==BVG)&&(menu[i].getPrice()<minPrice))||((menu[i].getType()==BVG)&&(menu[i].getPrice()==minPrice)&&(menu[i].getId()<idOfMinPrice))) {
                minPrice = menu[i].getPrice();
                idOfMinPrice=menu[i].getId();
            }
        }
        if(idOfMinPrice!=-1) {
            ord.push_back(idOfMinPrice);
        }
    }
    return ord;
}

std::string SpicyCustomer::toString() const {
    std::string s;
    s="Name: "+((*this).getName())+", ID: "+std::to_string((*this).getId())+", Type: SPC";
    return s;
}

Customer* SpicyCustomer::clone() const {
    return new SpicyCustomer(*this);
}

/*AlchoholicCustomer constructor*/
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):Customer(name, id), priceOfLastOrder(0), idOfLastOrder(-1) {}

AlchoholicCustomer::AlchoholicCustomer(const AlchoholicCustomer &other): AlchoholicCustomer(other.getName(), other.getId()){}

AlchoholicCustomer::~AlchoholicCustomer() {}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> ord;
    int min=INT_MAX;
    int idOfMin=-1;
    for (std::size_t i=0, max=menu.size();i<max;i++) {
        if(menu[i].getId()!=idOfLastOrder){
            if((menu[i].getType() == ALC) && (idOfMin==-1) && (menu[i].getPrice() == priceOfLastOrder) && (menu[i].getId()!=idOfLastOrder)){
                min = menu[i].getPrice();
                idOfMin = menu[i].getId();
            }
            if ((menu[i].getType() == ALC) && (menu[i].getPrice() >= priceOfLastOrder) && (menu[i].getPrice() < min)) {
                min = menu[i].getPrice();
                idOfMin = menu[i].getId();
            }
            if ((menu[i].getType() == ALC) && (menu[i].getPrice() == min)) {
                if (menu[i].getId() < idOfMin) {
                    min = menu[i].getPrice();
                    idOfMin = menu[i].getId();
                }
            }
        }
    }
    if(idOfMin!=-1){
        priceOfLastOrder=min;
        idOfLastOrder=idOfMin;
        ord.push_back(idOfMin);
    }
    return ord;
}

std::string AlchoholicCustomer::toString() const {
    std::string s;
    s="Name: "+(*this).getName()+", ID: "+std::to_string((*this).getId())+", Type: ALC";
    return s;
}

Customer* AlchoholicCustomer::clone() const {
    return new AlchoholicCustomer(*this);
}