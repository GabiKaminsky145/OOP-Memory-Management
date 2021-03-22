#include "../include/Action.h"
#include "../include/Restaurant.h"
#include "../include/Dish.h"
#include "../include/Table.h"


BaseAction::BaseAction():errorMsg(""), status(PENDING) {}
BaseAction::BaseAction(const BaseAction &other):errorMsg(other.errorMsg), status(other.status) {}

ActionStatus BaseAction::getStatus() const { return status;}
void BaseAction::complete() {status=COMPLETED;}
void BaseAction::error(std::string errorMsg) {
    this->errorMsg="Error: "+errorMsg;
    status=ERROR;
}
std::string BaseAction::getErrorMessage() {
    return getErrorMsg();
}
std::string BaseAction::getErrorMsg() const { return errorMsg;}

BaseAction::~BaseAction() {}

//Open Table
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(),tableId(id),customers{} {
    for(std::size_t i=0;i<customersList.size();i++){
        Customer* c(customersList[i]->clone());
        customers.push_back(c);
    }
}

/*copy constructor*/
OpenTable::OpenTable(const OpenTable &other): BaseAction(other), tableId(other.tableId), customers(other.customers){
    for(std::size_t i(0);i<other.customers.size();i++){
        customers.push_back(other.customers[i]);
    }
}

/*destructor*/
OpenTable::~OpenTable() {}

void OpenTable::act(Restaurant &restaurant) {
    if (restaurant.getTable(tableId) == nullptr || restaurant.getTable(tableId)->isOpen()) {
        error("Table does not exist or is already open");
        std::cout<<getErrorMsg()<<std::endl;
    }
    else{
        for (std::size_t i = 0; i <customers.size() ; ++i) {
            restaurant.getTable(tableId)->addCustomer(customers[i]);
        }
        restaurant.getTable(tableId)->openTable();
        complete();
    }
}

std::string OpenTable::toString() const { return "Opens a given table";}

BaseAction* OpenTable::clone() const {
    return new OpenTable(*this);
}


//Order
Order::Order(int id):BaseAction(),tableId(id) {}

Order::Order(const Order &other):BaseAction(other), tableId(other.tableId) {}

Order::~Order() {}

void Order::act(Restaurant &restaurant) {
    if (restaurant.getTable(tableId) == nullptr || (!(restaurant.getTable(tableId)->isOpen()))) {
        error("Table does not exist or is not open");
        std::cout<<getErrorMsg()<<std::endl;
    }
    else{
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        bool foundCustomer= false;
        std::string name;
        for (std::size_t i = restaurant.getTable(tableId)->getOrders().size()-restaurant.getTable(tableId)->getNumOfNewOrders(); i < restaurant.getTable(tableId)->getOrders().size() ; ++i) {
            for (std::size_t j = 0; !foundCustomer && j < restaurant.getTable(tableId)->getCustomers().size(); ++j) {//loop to find name of customer
                if (restaurant.getTable(tableId)->getCustomers()[j]->getId()==restaurant.getTable(tableId)->getOrders()[i].first){
                    name=restaurant.getTable(tableId)->getCustomers()[j]->getName();
                    foundCustomer=true;
                }
            }
            std::cout<<name +" ordered " + restaurant.getTable(tableId)->getOrders()[i].second.getName()<<std::endl;
            foundCustomer = false;
        }
        complete();
    }
}

std::string Order::toString() const { return "this action take order from every customer on the table, and print the orders";}

BaseAction* Order::clone() const {
    return new Order(*this);
}


MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst),id(customerId) {}

MoveCustomer::MoveCustomer(const MoveCustomer &other):BaseAction(other), srcTable(other.srcTable), dstTable(other.dstTable), id(other.id) {}

MoveCustomer::~MoveCustomer() {}

void MoveCustomer::act(Restaurant &restaurant) {
    if ((restaurant.getTables()[srcTable] == nullptr) || (restaurant.getTables()[dstTable] == nullptr) || (!(restaurant.getTables()[srcTable]->isOpen())) || (!(restaurant.getTables()[dstTable]->isOpen())) || (restaurant.getTables()[srcTable]->getCustomer(id) == nullptr) || (restaurant.getTables()[dstTable]->getCustomers().size() >= ((std::size_t)(restaurant.getTables()[dstTable]->getCapacity())))) {
        error("Cannot move customer");
        std::cout << getErrorMsg() << std::endl;
    }
    else {
        std::vector<OrderPair> customerOrders;
        std::vector<OrderPair> source;
        for (std::size_t i = 0; i < restaurant.getTables()[srcTable]->getOrders().size(); i++) {
            if (restaurant.getTables()[srcTable]->getOrders()[i].first == id) {
                customerOrders.push_back(restaurant.getTables()[srcTable]->getOrders()[i]);
            } else {
                source.push_back(restaurant.getTables()[srcTable]->getOrders()[i]);
            }
        }
        int customerIndexInSrcTable=0;
        for(std::size_t p=0;p<restaurant.getTables()[srcTable]->getCustomers().size();p++){
            if(restaurant.getTables()[srcTable]->getCustomers()[p]->getId() == id){
                customerIndexInSrcTable = p;
            }
        }
        Customer *c = restaurant.getTables()[srcTable]->getCustomer(id);
        restaurant.getTables()[dstTable]->addCustomer(c);
        restaurant.getTables()[srcTable]->getCustomers().erase(restaurant.getTables()[srcTable]->getCustomers().begin() + customerIndexInSrcTable);
        if (restaurant.getTables()[srcTable]->getCustomers().size() == 0) {
            restaurant.getTables()[srcTable]->closeTable();
        }
        for (std::size_t j = 0; j < customerOrders.size(); j++) {
            restaurant.getTables()[dstTable]->getOrders().push_back(customerOrders[j]);
        }
        restaurant.getTables()[srcTable]->getOrders().clear();
        for (std::size_t k = 0; k < source.size(); k++) {
            restaurant.getTables()[srcTable]->getOrders().push_back(source[k]);
        }
    }
    complete();
}

std::string MoveCustomer::toString() const { return "this action move customer from one table to another";}

BaseAction* MoveCustomer::clone() const {
    return new MoveCustomer(*this);
}

//Close
Close::Close(int id):BaseAction(),tableId(id) {}

Close::Close(const Close &other):BaseAction(other), tableId(other.tableId) {}

Close::~Close() {}

void Close::act(Restaurant &restaurant) {
    if (!(restaurant.getTable(tableId)->isOpen()) || restaurant.getTable(tableId) == nullptr) {
        error("Table does not exist or is not open");
        std::cout<<getErrorMsg()<<std::endl;
    }
    std::cout<<"table " + std::to_string(tableId) + " was closed. Bill "+std::to_string(restaurant.getTable(tableId)->getBill())+" NIS"<<std::endl;
    restaurant.getTable(tableId)->closeTable();
    complete();
}
std::string Close::toString() const { return "the action close a table and print the bill";}

BaseAction* Close::clone() const {
    return new Close(*this);
}


//CloseAll
CloseAll::CloseAll():BaseAction() {}

CloseAll::CloseAll(const CloseAll &other):BaseAction(other) {}

CloseAll::~CloseAll() {}

void CloseAll::act(Restaurant &restaurant) {
    for (std::size_t i = 0; i < restaurant.getTables().size() ; ++i) {
        if (restaurant.getTables()[i]->isOpen()){
            BaseAction* b = new Close(i);
            b->act(restaurant);
            delete b;
        }
    }
}
std::string CloseAll::toString() const { return "the action closes all tables, and then close the restaurant";}

BaseAction* CloseAll::clone() const {
    return new CloseAll(*this);
}

//Print Menu
PrintMenu::PrintMenu():BaseAction() {}

PrintMenu::PrintMenu(const PrintMenu &other):BaseAction(other) {}

PrintMenu::~PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant) {
    for (std::size_t i = 0; i < restaurant.getMenu().size() ; ++i) {
        if (restaurant.getMenu()[i].getType()==DishType::SPC)
            std::cout<<restaurant.getMenu()[i].getName() + " SPC "+ std::to_string(restaurant.getMenu()[i].getPrice())+"NIS"<<std::endl;
        if (restaurant.getMenu()[i].getType()==DishType::ALC)
            std::cout<<restaurant.getMenu()[i].getName() + " ALC "+ std::to_string(restaurant.getMenu()[i].getPrice())+"NIS"<<std::endl;
        if (restaurant.getMenu()[i].getType()==DishType::BVG)
            std::cout<<restaurant.getMenu()[i].getName() + " BVG "+ std::to_string(restaurant.getMenu()[i].getPrice())+"NIS"<<std::endl;
        if (restaurant.getMenu()[i].getType()==DishType::VEG)
            std::cout<<restaurant.getMenu()[i].getName() + " VEG "+ std::to_string(restaurant.getMenu()[i].getPrice())+"NIS"<<std::endl;
    }
    complete();
}
std::string PrintMenu::toString() const { return "the action returns te menu of the restaurant";}

BaseAction* PrintMenu::clone() const {
    return new PrintMenu(*this);
}

//Print Table Status
PrintTableStatus::PrintTableStatus(int id):BaseAction(),tableId(id) {}

PrintTableStatus::PrintTableStatus(const PrintTableStatus &other):BaseAction(other), tableId(other.tableId) {}

PrintTableStatus::~PrintTableStatus() {}

void PrintTableStatus::act(Restaurant &restaurant) {
    if (!restaurant.getTable(tableId)->isOpen())
        std:: cout<<"Table " + std::to_string(tableId) + " status: closed"<<std::endl;
    else{
        std::cout<<"Table " + std::to_string(tableId) + " status: open" + "\n" + "Customers: " << std::endl;
        for (std::size_t i = 0; i < restaurant.getTable(tableId)->getCustomers().size() ; ++i) {
            std::cout << std::to_string(restaurant.getTable(tableId)->getCustomers()[i]->getId()) + " " + restaurant.getTable(tableId)->getCustomers()[i]->getName() << std::endl;
        }
        std::cout<<"Orders:"<<std::endl;
        for (std::size_t j = 0; j < restaurant.getTable(tableId)->getOrders().size() ; ++j) {
            std::cout<<restaurant.getTable(tableId)->getOrders()[j].second.getName() +" " + std::to_string(restaurant.getTable(tableId)->getOrders()[j].second.getPrice())+"NIS " + std::to_string(restaurant.getTable(tableId)->getOrders()[j].first) <<std::endl;
        }
        std::cout<<"Current Bill: " + std::to_string(restaurant.getTable(tableId)->getBill())+"NIS"<<std::endl;
    }
    complete();
}
std::string PrintTableStatus::toString() const { return "the action print the status of the table";}

BaseAction* PrintTableStatus::clone() const {
    return new PrintTableStatus(*this);
}

//Print Actions Log
PrintActionsLog::PrintActionsLog():BaseAction() {}

PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):BaseAction(other) {}

PrintActionsLog::~PrintActionsLog() {}

void PrintActionsLog::act(Restaurant &restaurant) {
    for (std::size_t i = 0; i < restaurant.getActionsLog().size() ; ++i) {
        ActionStatus as=restaurant.getActionsLog()[i]->getStatus();
        if(as==COMPLETED){
            std::cout << restaurant.getActionsDetails()[i] + " Completed" << std::endl;
        }
        if(as==ERROR){
            std::cout << restaurant.getActionsDetails()[i] + " " + restaurant.getActionsLog()[i]->getErrorMessage() << std::endl;
        }
    }
    complete();
}
std::string PrintActionsLog::toString() const { return "the action prints all the actions that the user have done";}

BaseAction* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

//Backup
BackupRestaurant::BackupRestaurant():BaseAction() {}

BackupRestaurant::BackupRestaurant(const BackupRestaurant &other):BaseAction(other) {}

BackupRestaurant::~BackupRestaurant() {}

void BackupRestaurant::act(Restaurant &restaurant) {
    delete backup;
    backup = nullptr;
    backup = new Restaurant(restaurant);
    complete();
}
std::string BackupRestaurant::toString() const { return "the action backup all the information of the restaurant";}

BaseAction* BackupRestaurant::clone() const {
    return new BackupRestaurant(*this);
}

//Restore
RestoreResturant::RestoreResturant():BaseAction() {}

RestoreResturant::RestoreResturant(const RestoreResturant &other):BaseAction(other) {}

RestoreResturant::~RestoreResturant() {}

void RestoreResturant::act(Restaurant &restaurant) {
    bool flag = false;
    for(std::size_t i=0;(!flag) && (i<restaurant.getActionsDetails().size());i++){
        if(restaurant.getActionsDetails()[i] == "backup"){
            flag = true;
            restaurant = *backup;
            complete();
        }
    }
    if(!flag){
        error("No backup available");
        std::cout << getErrorMsg() << std::endl;
    }
}
std::string RestoreResturant::toString() const { return "the action restore the backup restaurant";}

BaseAction* RestoreResturant::clone() const {
    return new RestoreResturant(*this);
}