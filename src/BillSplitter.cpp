#include "Log.h"
#include "Person.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

using Bill = vector<Person>;

/*

Hampus betalar 200
Niklas betalar 1337
Magnus betalar 2
Andreas betalar 2000

200 + 1337 + 2 + 2000 = 3539

3539/4 = 885

Hampus - 885 = -685
Niklas - 885 = 452
Magnus - 885 = -883
Andreas - 885 = 1115

*/

Bill parse(const vector<string>& args) {
    Bill bill;
    for (size_t i = 0; i < args.size(); i += 2) {
        try {
            bill.push_back(Person(args[i], stod(args[i+1])));
            cout << bill.back().name_ << bill.back().amount_ << endl;
        } catch(...) {
            Log(DEBUG) << "Incorrect syntax, should be [Name Price ... Name Price]\n";
        }
    }
    return bill;
}

void split(Bill& bill) {
    double sum = 0;
    for_each(bill.begin(), bill.end(), [&sum] (auto& person) { sum += person.amount_; });
    sum /= bill.size();
    for_each(bill.begin(), bill.end(), [&sum] (auto& person) { person.amount_ -= sum; });
    Bill people_owed;
    Bill people_owing;

    for (Person person: bill) {
        if(person.amount_ >= 0){
            people_owed.push_back(person);
        } else {
            people_owing.push_back(person);
        }
    }

    sort(people_owed.begin(), people_owed.end(), [] (auto& a, auto& b) { return a.amount_ > b.amount_; });
    sort(people_owing.begin(), people_owing.end(), [] (auto& a, auto& b) { return a.amount_ < b.amount_; });

    vector<string> swishes;
    for (Person& person: people_owing) {
        for (Person& person_owed: people_owed) {
            if (person.amount_ == 0 || person_owed.amount_ == 0) {
                continue;
            }
            
            if (person_owed.amount_ <= -person.amount_) {
                person.amount_ += person_owed.amount_;
                cout << (person.name_ + " swishes " + person_owed.name_ + " " + to_string(person_owed.amount_) + " kr") << endl;
                person_owed.amount_ = 0;
            } else {
                person_owed.amount_ += person.amount_;
                cout << (person.name_ + " swishes " + person_owed.name_ + " " + to_string(-person.amount_) + " kr") << endl;
                person.amount_ = 0;
            }
        }
    }
}
/*


*/

int main(int argc, char** argv) {
    auto bill = parse(vector<string>(argv + 1, argv + argc));

    split(bill);
    return 0;
}