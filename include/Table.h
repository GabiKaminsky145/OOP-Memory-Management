#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "../include/Customer.h"
#include "../include/Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    Table(const Table& other);//copy constructor
    virtual ~Table();//destructor
    Table& operator=(const Table& other);//copy assignment operator
    Table(Table&& other);//move constructor
    Table& operator=(Table&& other);//move assignment operator
    Table* clone() const;
    int getNumOfNewOrders();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
    std::size_t numOfNewOrders;
};


#endif