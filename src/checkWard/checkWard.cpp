#include <C:\Users\ADMIN\OneDrive - Hanoi University of Science and Technology\Tài liệu\GitHub\BTL-IT3100-nh-m-3T2D\src\pedastrian\pedestrian.cpp>
#include <C:\Users\ADMIN\OneDrive - Hanoi University of Science and Technology\Tài liệu\GitHub\BTL-IT3100-nh-m-3T2D\src\pedastrian\pedestrian.h>

#include "C:\\Users\\ADMIN\\OneDrive - Hanoi University of Science and Technology\\Tài liệu\\GitHub\\BTL-IT3100-nh-m-3T2D\\lib\\nlohmann\\json.hpp"
#include <utility>



using namespace std;

vector<pair<Ward,int>> checkWard( vector<Ward> allWards, int triple, int single){
    vector<pair<Ward,int>> res;
    int sum= triple*3 + single ;
    double mean = 0.0; // Mean of normal distribution
  double std_dev = 50.0;
   
std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> distribution(mean, std_dev);

  // Generate random values from normal distribution
  std::vector<double> random_values;
  for (int i = 0; i < allWards.size(); ++i) {
    random_values.push_back(distribution(gen));
  }

 
  std::vector<int> integer_values;
  for (double value : random_values) {
    // Round to the nearest integer
    integer_values.push_back(static_cast<int>(std::round(value)));
  }

 
  double current_sum = 0.0;
  for (int value : integer_values) {
    current_sum += value;
  }

  double scaling_factor = sum / current_sum;
  for (int& value : integer_values) {
    value = static_cast<int>(std::round(value * scaling_factor));
  }

for( int i=0 ;i<allWards.size();i++){
   res.push_back(make_pair(allWards[i],integer_values[i]));
}

    return res;

}
