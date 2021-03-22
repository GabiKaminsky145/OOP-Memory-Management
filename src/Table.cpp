#include "../include/Table.h"
#include <sstream>
#include <string>
#include <iostream>

/* constructor */
Table::Table(int t_capacity):capacity(t_capacity), open(false), customersList{}, orderList{}, numOfNewOrders(0){}

/*copy constructor*/
Table::Table(const Table &other):capacity(other.capacity),open(other.open), customersList{}, orderList{}, numOfNewOrders(other.numOfNewOrders){
    for(std::size_t i(0);i<other.customersList.size();i++){
        customersList.push_back(other.customersList[i]->clone());
    }
    for(std::size_t j=0;j<other.orderList.size();j++){
        OrderPair op=other.orderList[j];
        orderList.push_back(op);
    }
}

/*copy assignment operator*/
Table& Table::operator=(const Table &other) {
    if(this!=&other){
        capacity=other.capacity;
        open=other.open;
        numOfNewOrders=other.numOfNewOrders;
        for(std::size_t i=0;i<customersList.size();i++){
            delete customersList[i];
        }
        customersList.clear();
        for(std::size_t j=0;j<other.customersList.size();j++){
            Customer* c=other.customersList[j]->clone();
            customersList.push_back(c);
        }
        orderList.clear();
        for(std::size_t m=0;m<other.orderList.size();m++){
            OrderPair op=other.orderList[m];
            orderList.push_back(op);
        }
    }
    return *this;
}

/*destructor*/
Table::~Table() {
    for(std::size_t i=0;i<customersList.size();i++){
        delete customersList[i];
    }
    customersList.clear();
    orderList.clear();
}

/*move constructor*/
Table::Table(Table &&other): capacity(other.capacity), open(other.open), customersList(other.customersList), orderList(other.orderList), numOfNewOrders(other.numOfNewOrders){
    for(std::size_t i=0;i<other.customersList.size();i++){
        other.customersList[i]=nullptr;
    }
    other.customersList.clear();
    other.orderList.clear();
}

/*move assignment operator*/
Table& Table::operator=(Table &&other) {
    if(this!=&other){
        capacity=other.capacity;
        open=other.open;
        customersList=other.customersList;
        numOfNewOrders=other.numOfNewOrders;
        for(std::size_t j=0;j<other.customersList.size();j++){
            other.customersList[j]= nullptr;
        }
        for(std::size_t i=0;i<other.orderList.size();i++){
            orderList.push_back(other.orderList[i]);
        }
        other.customersList.clear();
        other.orderList.clear();
    }
    return *this;
}

Table* Table::clone() const {
    return new Table(*this);
}

int Table::getCapacity() const{
    return capacity;
}

void Table::addCustomer(Customer* customer){
        bool find = false;
        for (std::size_t i = 0; i < customersList.size(); i++) {
            if (customersList[i]->getId() == customer->getId())
                find = true;
        }
        if ((!find) && (customersList.size() < ((std::size_t)capacity))) {
            customersList.push_back(customer);
        }
}

void Table::removeCustomer(int id){
    bool flag(false);
    for(std::size_t i(0);!flag && i<customersList.size();i++){
        if(customersList[i]->getId()==id){
            delete customersList[i];
            customersList.erase(customersList.begin()+i);
            flag=true;
        }
    }
}

Customer* Table::getCustomer(int id){
    for(std::size_t i(0);i<customersList.size();i++){
        if(customersList[i]->getId()==id){
            return customersList[i];
        }
    }
    return nullptr;
}

std::vector<Customer*>& Table::getCustomers(){
    return customersList;
}

std::vector<OrderPair>& Table::getOrders(){
    return orderList;
}

void Table::order(const std::vector<Dish> &menu){
    std::size_t oldOrderListSize=orderList.size();
    for(std::size_t i(0);i<customersList.size();i++){
        std::vector<int> ord=(*(customersList[i])).order(menu);
        std::vector<Dish> dishes;
        bool flag=false;
        for(std::size_t k(0);k<ord.size();k++){
            for(std::size_t j(0);!flag&&j<menu.size();j++){
                if(menu[j].getId()==ord[k]){
                    Dish d(menu[j].getId(), menu[j].getName(), menu[j].getPrice(), menu[j].getType());
                    dishes.push_back(d);
                    flag=true;
                }
            }
            flag=false;
        }
        for(std::size_t m(0);m<dishes.size();m++){
            OrderPair p((*(customersList[i])).getId(), (dishes[m]));
            orderList.push_back(p);
        }
    }
    numOfNewOrders=orderList.size()-oldOrderListSize;
}

void Table::openTable(){
    open=true;
}
void Table::closeTable(){
    open=false;
    delete this;
    customersList.clear();
    orderList.clear();
}
int Table::getBill(){
    int sum(0);
    for(std::size_t i(0);i<orderList.size();i++){
        sum+=orderList[i].second.getPrice();
    }
    return sum;
}
bool Table::isOpen(){
    return open;
}

int Table::getNumOfNewOrders() {
    return numOfNewOrders;
}