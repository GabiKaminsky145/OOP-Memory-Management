#include "../include/Restaurant.h"
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

/*constructors*/
Restaurant::Restaurant():open(false),tables{}, menu{}, actionsLog{}, actionsDetails{} {}

Restaurant::Restaurant(const std::string &configFilePath):Restaurant(){
    std::ifstream infile(configFilePath);
    std::string line;
    std::getline(infile, line);
    while(line.find('#')!=std::string::npos){
        std::getline(infile, line);
    }
    std::istringstream convert(line);
    int numOfTables;
    convert >> numOfTables;
    std::getline(infile, line);
    while(line.find('#')!=std::string::npos){
        std::getline(infile, line);
    }
    std::vector<std::string>* capa=parseText(line);
    for(std::size_t i=0, max=(*capa).size();i<max;i++){
        std::istringstream convert2((*capa)[i]);
        int tableCapacity;
        convert2 >> tableCapacity;
        Table* t=new Table(tableCapacity);
        tables.push_back(t);
    }
    delete capa;
    std::getline(infile, line);
    while(line.find('#')!=std::string::npos){
        std::getline(infile, line);
    }
    int id(0);
    bool flag=false;
    while(!flag){
        const std::vector<std::string>* dish=parseText(line);
        std::string dishname=(*dish)[0];
        std::string typedish=(*(dish))[1];
        std::istringstream convert3((*dish)[2]);
        int dishprice;
        convert3 >> dishprice;
        DishType dt;
        dt=VEG;
        if(typedish=="SPC")
            dt=SPC;
        if(typedish=="ALC")
            dt=ALC;
        if(typedish=="BVG")
            dt=BVG;
        Dish d(id, dishname, dishprice, dt);
        menu.push_back(d);
        id++;
        delete dish;
        if(!std::getline(infile, line)){
            flag=true;
        }
    }
}

/*copy constructor*/
Restaurant::Restaurant(const Restaurant &other):open(other.open), tables{}, menu(other.menu), actionsLog{}, actionsDetails(other.actionsDetails) {
    for(std::size_t i=0, max=other.tables.size();i<max;i++){
        tables.push_back(other.tables[i]->clone());
    }
    for(std::size_t j=0, max2=other.actionsLog.size();j<max2;j++){
        actionsLog.push_back(other.actionsLog[j]->clone());
    }
}

/*copy assignment operator*/
Restaurant& Restaurant::operator=(const Restaurant &other) {
    if(this!=&other){
        open=other.open;
        menu.clear();
        for(std::size_t d=0, max1=other.menu.size();d<max1;d++){
            menu.push_back(other.menu[d]);
        }
        actionsDetails.clear();
        for(std::size_t i=0, max2=tables.size();i<max2;i++){
            if(tables[i] != nullptr) {
                delete tables[i];
            }
        }
        tables.clear();
        for(std::size_t j=0, max3=other.tables.size();j<max3;j++){
            tables.push_back(other.tables[j]->clone());
        }
        for(std::size_t k=0, max4=actionsLog.size();k<max4;k++){
            if(actionsLog[k] != nullptr) {
                delete actionsLog[k];
            }
        }
        actionsLog.clear();
        for(std::size_t m=0, max5=other.actionsLog.size();m<max5;m++){
            actionsLog.push_back(other.actionsLog[m]->clone());
            actionsDetails.push_back(other.actionsDetails[m]);
        }
    }
    return *this;
}

/*destructor*/
Restaurant::~Restaurant() {
    for(std::size_t i=0, max1=tables.size();i<max1;i++){
        delete tables[i];
    }
    tables.clear();
    for(std::size_t j=0, max2=actionsLog.size();j<max2;j++){
        delete actionsLog[j];
    }
    actionsLog.clear();
    menu.clear();
    actionsDetails.clear();
}

/*move constructor*/
Restaurant::Restaurant(Restaurant &&other): open(other.open), tables(other.tables), menu(other.menu), actionsLog(other.actionsLog), actionsDetails(other.actionsDetails){
    for(std::size_t i=0, max=other.tables.size();i<max;i++){
        other.tables[i]= nullptr;
    }
    other.tables.clear();
    for(std::size_t j=0, max1=other.actionsLog.size();j<max1;j++){
        other.actionsLog[j]= nullptr;
    }
    other.actionsLog.clear();
}

/*move assignment operator*/
Restaurant& Restaurant::operator=(Restaurant &&other) {
    if(this!=&other){
        open=other.open;
        tables=other.tables;
        actionsDetails=other.actionsDetails;
        for(std::size_t k=0, max=other.menu.size();k<max;k++){
            menu.push_back(other.menu[k]);
        }
        actionsLog=other.actionsLog;
        for(std::size_t i=0, max2=other.tables.size();i<max2;i++){
            other.tables[i]= nullptr;
        }
        other.tables.clear();
        for(std::size_t j=0, max3=other.actionsLog.size();j<max3;j++){
            other.actionsLog[j]= nullptr;
        }
        other.actionsLog.clear();
    }
    return *this;
}

std::vector<int>* Restaurant::countAppearences(char c, std::string s) {//counting the number of times the char c appears in the string s, and returns a pointer to a vector of integers with the indexes that the char appears in the string
    std::vector<int>* count=new std::vector<int>();
    for(std::size_t i=0, max=s.size();i<max;i++){
        if(s.at(i)==c)
            count->push_back(i);
    }
    return count;
}

std::vector<std::string>* Restaurant::parseText(const std::string &line) {//getting a text in a pattern of "word1,word2,word3,...", and returns a pointer to a vector of strings that each cell contains one word from the text 'line'
    std::vector<std::string>* v=new std::vector<std::string>();
    std::vector<int>* commas=countAppearences(',', line);
    int j(0);
    for(std::size_t i=0, max1=(*commas).size();i<=max1;i++){
        if(i!=max1){
            (*v).push_back(line.substr(j, ((*commas)[i])-j));
            j=((*commas)[i])+1;
        } else{
            (*v).push_back(line.substr(j));
        }
    }
    delete commas;
    return v;
}

void Restaurant::start(){
    open=true;
    std::cout << "Restaurant is now open!" << std::endl;
    int numOfCustomers=0;
    std::string input;
    getline (std::cin, input);
    while(input!="closeall"){
        actionsDetails.push_back(input);
        std::vector<int>* spaces=countAppearences(' ', input);
        if(spaces->size()==0){
            if(input=="menu"){//Action "Print Menu"
                BaseAction* ba1=new PrintMenu();
                makeAction(ba1);
            }
            if(input=="log"){//Action "Print Actions Log"
                BaseAction* ba2=new PrintActionsLog();
                makeAction(ba2);
            }
            if(input=="backup"){//Action "Backup Restaurant"
                BaseAction* ba3=new BackupRestaurant();
                actionsLog.push_back(ba3);
                ba3->act(*this);
            }
            if(input=="restore"){//Action "Restore Restaurant"
                BaseAction* ba4=new RestoreResturant();
                ba4->act(*this);
                actionsLog.push_back(ba4);
                actionsDetails.push_back(input);
            }
        }
        else {
                std::string action = input.substr(0, (*spaces)[0]);
                if (action == "open") {//Action "Open Table"
                    numOfCustomers=actOpenTable(input, spaces, action, numOfCustomers);
                }
                if(action == "order") {//Action "Order"
                    action=input.substr((*spaces)[0]+1);
                    std::istringstream convert(action);
                    int tableNum;
                    convert >> tableNum;
                    BaseAction* ba6=new Order(tableNum);
                    makeAction(ba6);
                }
                if(action == "move"){//Action "Move Customer"
                    action=input.substr((*spaces)[0]+1, (*spaces)[1]-((*spaces)[0] + 1));
                    std::istringstream convertSrc(action);
                    int src;
                    convertSrc >> src;
                    action = input.substr((*spaces)[1]+1, (*spaces)[2]-((*spaces)[1] + 1));
                    std::istringstream convertDst(action);
                    int dst;
                    convertDst >> dst;
                    action = input.substr((*spaces)[2]+1);
                    std::istringstream convertCustomerId(action);
                    int customerId;
                    convertCustomerId >> customerId;
                    BaseAction* ba7=new MoveCustomer(src, dst, customerId);
                    makeAction(ba7);
                }
                if(action == "close"){//Action "Close"
                    action = input.substr((*spaces)[0]+1);
                    std::istringstream convert(action);
                    int tableId;
                    convert >> tableId;
                    BaseAction* ba8=new Close(tableId);
                    makeAction(ba8);
                }
                if(action == "status"){//Action "Print Table Status"
                    action = input.substr((*spaces)[0]+1);
                    std::istringstream convert(action);
                    int tableId;
                    convert >> tableId;
                    BaseAction* ba9=new PrintTableStatus(tableId);
                    makeAction(ba9);
                }
        }
        delete spaces;
        getline(std::cin, input);
    }
    BaseAction* ba10=new CloseAll();
    ba10->act(*this);
    delete ba10;
    close();
    for(std::size_t i=0, max1=this->tables.size();i<max1;i++){
        delete tables[i];
    }
    this->tables.clear();
    for(std::size_t j=0, max2=this->actionsLog.size();j<max2;j++){
        delete actionsLog[j];
    }
    this->actionsLog.clear();
    this->menu.clear();
    this->actionsDetails.clear();
}

int Restaurant::actOpenTable(const std::string& input, std::vector<int>* spaces, std::string& action, int numOfCustomers) {//set up an Open Table action by parsing the 'input' text, and after that start the action and push it the actions log
    action = input.substr((*spaces)[0] + 1, (*spaces)[1]-((*spaces)[0] + 1));
    std::istringstream convert(action);
    int tableNum;
    convert >> tableNum;
    std::vector<Customer*> customersList;
    for(std::size_t i=2, max=spaces->size();i<=max;i++){
        if(i==spaces->size()){
            action = input.substr((*spaces)[i-1]+1);
        }
        else{
            action = input.substr((*spaces)[i-1]+1, (*spaces)[i]-((*spaces)[i-1] + 1));
        }
        std::vector<std::string>* parseAction=parseText(action);
        std::string cName=(*parseAction)[0];
        std::string cType=(*parseAction)[1];
        if(cType=="veg") {
            customersList.push_back(new VegetarianCustomer(cName, numOfCustomers));
        }
        if(cType=="chp") {
            customersList.push_back(new CheapCustomer(cName, numOfCustomers));
        }
        if(cType=="spc") {
            customersList.push_back(new SpicyCustomer(cName, numOfCustomers));
        }
        if(cType=="alc") {
            customersList.push_back(new AlchoholicCustomer(cName, numOfCustomers));
        }
        numOfCustomers++;
        delete parseAction;
    }
    BaseAction* ba5=new OpenTable(tableNum, customersList);
    makeAction(ba5);
    for(std::size_t k=0;k<customersList.size();k++){
        delete customersList[k];
    }
    return numOfCustomers;
}

void Restaurant::makeAction(BaseAction *ba) {//start the action 'ba' and push it the the actions log
    ba->act(*this);
    actionsLog.push_back(ba);
}

int Restaurant::getNumOfTables() const{
    return tables.size();
}

Table* Restaurant::getTable(int ind){
    if(((std::size_t)(ind))<tables.size()) {
        return tables[ind];
    }
    return nullptr;
}

const std::vector<BaseAction*>& Restaurant::getActionsLog() const {// Return a reference to the history of actions
    return actionsLog;
}

std::vector<Dish>& Restaurant::getMenu() {
    return menu;
}
void Restaurant::close() {
    open=false;
}

const std::vector<Table*>& Restaurant::getTables() const {
    return tables;
}

const std::vector<std::string>& Restaurant::getActionsDetails() const {
    return actionsDetails;
}

void Restaurant::setTables(Table* t) {
    tables.push_back(t);
}

void Restaurant::setMenu(Dish& d) {
    menu.push_back(d);
}
void Restaurant::setActionsLog(BaseAction* baseAction) {
       actionsLog.push_back(baseAction);
}
void Restaurant::setActionDetails(std::string s) {
    actionsDetails.push_back(s);
}
void Restaurant::setOpen(bool flag) {
    open=flag;
}
bool Restaurant::isOpen() {
    return open;
}