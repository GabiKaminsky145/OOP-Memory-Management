#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "../include/Dish.h"
#include "../include/Table.h"
#include "../include/Action.h"


class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<Table*>& getTables() const;
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    std::vector<std::string>* parseText(const std::string& line);//getting a text in a pattern of "word1,word2,word3,...", and returns a pointer to a vector of strings that each cell contains one word from the text 'line'
    std::vector<int>* countAppearences(char c, std::string s);//counting the number of times the char c appears in the string s, and returns a pointer to a vector of integers with the indexes that the char appears in the string
    void close();
    Restaurant(const Restaurant& other);//copy constructor
    virtual ~Restaurant();//destructor
    Restaurant& operator=(const Restaurant& other);//copy assignment operator
    Restaurant(Restaurant&& other);//move constructor
    Restaurant& operator=(Restaurant&& other);//move assignment operator
    void makeAction(BaseAction* ba);//start the action 'ba' and push it the the actions log
    int actOpenTable(const std::string& input, std::vector<int>* spaces, std::string& action, int numOfCustomers);//set up an Open Table action by parsing the 'input' text, and after that start the action and push it the actions log
    const std::vector<std::string>& getActionsDetails() const;
    void setTables(Table* t);
    void setMenu(Dish& d);
    void setActionsLog(BaseAction* baseAction);
    void setActionDetails(std::string s);
    void setOpen(bool flag);
    bool isOpen();
private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    std::vector<std::string> actionsDetails;
};

#endif