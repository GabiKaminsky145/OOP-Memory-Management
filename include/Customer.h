#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "../include/Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual Customer* clone() const = 0;
    virtual ~Customer() = 0;
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    VegetarianCustomer(const VegetarianCustomer& other);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone() const;
    virtual ~VegetarianCustomer();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    CheapCustomer(const CheapCustomer& other);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone() const;
    virtual ~CheapCustomer();
private:
    int orderCount;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    SpicyCustomer(const SpicyCustomer& other);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone() const;
    virtual ~SpicyCustomer();
private:
    int orderCount;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    AlchoholicCustomer(const AlchoholicCustomer& other);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone() const;
    virtual ~AlchoholicCustomer();
private:
    int priceOfLastOrder;
    int idOfLastOrder;
};


#endif