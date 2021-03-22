#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "../include/Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

extern Restaurant* backup;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone() const = 0;
    virtual ~BaseAction() = 0;
    std::string getErrorMessage();
    BaseAction(const BaseAction& other);
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    OpenTable(const OpenTable& other);//copy constructor
    virtual ~OpenTable();//destructor
    OpenTable(OpenTable&& other);//move constructor
private:
    const int tableId;
    std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    Order(const Order& other);
    virtual ~Order();
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    MoveCustomer(const MoveCustomer& other);
    virtual ~MoveCustomer();
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    Close(const Close& other);
    virtual ~Close();
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    CloseAll(const CloseAll& other);
    virtual ~CloseAll();
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    PrintMenu(const PrintMenu& other);
    virtual ~PrintMenu();
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    PrintTableStatus(const PrintTableStatus& other);
    virtual ~PrintTableStatus();
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    PrintActionsLog(const PrintActionsLog& other);
    virtual ~PrintActionsLog();
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    BackupRestaurant(const BackupRestaurant& other);
    virtual ~BackupRestaurant();
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone() const;
    RestoreResturant(const RestoreResturant& other);
    virtual ~RestoreResturant();
};


#endif