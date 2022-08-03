// Author: shr-go
#include "Reflection.h"
#include "SQLCore.h"
#include <sqlite3.h>

#include <string>
#include <iostream>
#include <array>
#include <tuple>

struct account {
    double dollar;
    double yen;
};
REFLECT(account, dollar, yen);

struct person {
    std::string name;
    int age;
    account money;
};
REFLECT(person, name, money, age);

int main() {
//    person p {"Jack", 20, {100, 200}};
//    auto sql = chanchan::createTable<person>();
//    std::cout << sql << std::endl;
//    sql = chanchan::insertRecord(p);
//    std::cout << sql << std::endl;
//
//    std::vector<person> vp {{"Jack", 20, {100.3, 200.6}}, {"Mary", 18, {999.99, 0.12}}};
//    sql = chanchan::insertRecord(vp);
//    std::cout << sql << std::endl;
//    sql = chanchan::updateRecord(p);
//    std::cout << sql << std::endl;
//    sql = chanchan::deleteRecord<person>();
//    std::cout << sql << std::endl;
//    sql = chanchan::deleteRecord<person>("`name`=`jack`");
//    std::cout << sql << std::endl;
//    sql = chanchan::queryRecord<person>();
//    std::cout << sql << std::endl;
//    sql = chanchan::queryRecord<person>("WHERE `name`=`jack`");
//    std::cout << sql << std::endl;
}
