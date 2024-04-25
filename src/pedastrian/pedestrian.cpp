#include "pedestrian.h"
#include "C:\\Users\\ADMIN\\OneDrive - Hanoi University of Science and Technology\\Tài liệu\\GitHub\\BTL-IT3100-nh-m-3T2D\\lib\\nlohmann\\json.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <queue>


using namespace std;
using json = nlohmann::json;

double randomDouble(double min, double max) {
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<double> dis(min, max);
	return dis(gen);
}

int randomInt(int min, int max) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}   

double calcX(double coor, double width, bool left) {
    return left ? coor - width / 2 : coor + width / 2;
}


vector<Ward> genWard(){
	vector<Ward> wards;
	ifstream file("data/hospital.txt");
	string line;
	if(file.is_open()){
			getline(file,line);
			for (int i=2;i<11;i++){
				vector<Point> edges;
				Point gate1,gate2,point1,point2,point3,point4;
				double width;
				string ward_name;
				if(!getline(file,line)){
					break;
				}
				istringstream iss(line);
				iss>>gate1.x >>gate1.y;
				iss>>gate2.x >> gate2.y;
				iss>> width;
				iss>> ward_name;
				point1.x=calcX( gate1.x ,width,true);
				point1.y=gate1.y;
				edges.push_back(point1);


				point2.x=calcX(gate2.x,width, true);
				point2.y=gate1.y;
				edges.push_back(point2);

				point3.x=calcX( gate2.x ,width,true);
				point3.y=gate2.y;
				edges.push_back(point3);
				
				point4.x=calcX( gate2.x ,width,true);
				point4.y=gate2.y;
				edges.push_back(point4);
				wards.push_back(Ward(ward_name,gate1,gate2,edges));

				


				

			};
			file.close();


	}
	else cout<< "File not found";
	return wards;




}

vector<Event> genEvent(){
	vector<Event> allEvents;
	vector<double> allTimeDistance;
	ifstream f1("data/timeBetweenEventDistribution");
	string line;
	while(getline(f1,line)){
		allTimeDistance.push_back(stod(line));
	}

	
	f1.close();
	


	ifstream file("data/eventDistribution");
	if(file.is_open()){
		int i=0;
		while(getline(file,line)){
			Event event;
			istringstream iss(line);
			vector<double> instensity;
			double value;
			while(iss>>value)	instensity.push_back(value);
			event.setIntensity(instensity);
			event.setTime(allTimeDistance[i]);
			i++;
			
			allEvents.push_back(event);

		}
		file.close();
	}
	else cout<<"File not found";
	return allEvents;

}



vector<Pedestrians> genPedestrians(){
	
	ifstream file("data/input.json");
	json inputData= json::parse(file);
	int ID=0;
	  double lechChuan = randomDouble(1 - (double)inputData["experimentalDeviation"]["value"] / 100,
	   1 + (double)inputData["experimentalDeviation"]["value"] / 100);
	vector<double> ages;
	ifstream f1("data/ageDistribution");
	string line;
	while(getline(f1,line)){
		ages.push_back(stod(line));

	};
	f1.close();

	vector<Pedestrians> pedestrians;
	vector<Event> events= genEvent();
	vector<Ward> allWards= genWard();

	// chia tỉ lệ :500=160+190+150

	// chia nhỏ:75+95+100+90+113+37 
	Pesonality open, neurotic;
    open.setLambda(1);
    open.setPositiveEmotionThreshold(0.3);
    open.setNegativeEmotionThreshold(-0.7);
    neurotic.setLambda(4);
    neurotic.setPositiveEmotionThreshold(0.6);
    neurotic.setNegativeEmotionThreshold(-0.4);


	double NoDisabilityNoOvertaking_velocity = double(1.24) * lechChuan;
    double NoDisabilityOvertaking_velocity = double(2.48) * lechChuan;
    double Crutches_velocity = double(0.94) * lechChuan;
    double Sticks_velocity = double(0.81) * lechChuan;
    double Wheelchair_velocity = double(0.69) * lechChuan;
    double Blind_velocity = double(0.52) * lechChuan;
	//int tiLePesdetrian[]={160,190,150};
//A:13 33 13 41 3 37 25 37
			


				
				while(ID++<=160){
				vector<Ward> journey;
				vector<Ward> ward;
			  	Personnel personnel;
                vector<Event> events;
                personnel.setID(ID);
				int age;
                while (age <= 23 || age >= 61)  age = ages[randomInt(0, ages.size() - 1)];
                personnel.setAge(age);
				personnel.setPersonality(open);
			 	personnel.setVelocity(ID <= 75 ? NoDisabilityNoOvertaking_velocity 
			 									: NoDisabilityOvertaking_velocity);
				pedestrians.push_back(personnel);
				for( int i=0; i<20;i++){
						 Event event = events[randomInt(0, events.size() - 1)];
						 events.push_back(event);
					}
				ward.push_back(allWards[randomInt(0,9)]);
				personnel.setJourney(ward);
				personnel.setEvents(events);
				personnel.setStart(allWards[8]);
				personnel.setEnd(allWards[8]);

                pedestrians.push_back(personnel);
				}
				while(ID++<350){
					Visitor visitor;
					vector<Ward> journey;
					vector<Ward> ward;
                    vector<Event> events;
                    visitor.setID(ID);

                    visitor.setAge(ages[randomInt(0, ages.size() - 1)]);
                    visitor.setPersonality(ID <= 250 ? open : (visitor.getAge() < 11 ? open : neurotic));
                    visitor.setVelocity(ID <= 280 ? (ID <= 180 ? NoDisabilityOvertaking_velocity : Crutches_velocity) : Sticks_velocity);
					for( int i=0; i<20;i++){
						 Event event = events[randomInt(0, events.size() - 1)];
						 events.push_back(event);
					}
				int i= randomInt(0,7);
				ward.push_back(allWards[i]);
				int j = randomInt(i,8);
				ward.push_back(allWards[j]);
				ward.push_back(allWards[randomInt(j,9)]);
				visitor.setJourney(ward);
				visitor.setStart(allWards[8]);
				visitor.setEnd(allWards[8]);

					visitor.setEvents(events);
                   pedestrians.push_back(visitor);
				}
				while(ID++<500){
					
				    Patient patient; 
				    vector<Event> events;
					vector<Ward> journey;
					vector<Ward> ward;
				    patient.setID(ID); 

				    patient.setAge(ages[randomInt(0, ages.size() - 1)]); 
                    patient.setPersonality(patient.getAge() < 11 ? open : neurotic);  
                    patient.setVelocity(ID <= 113 ? Wheelchair_velocity : Blind_velocity); 
                    ward.push_back(allWards[randomInt(0,9)]);
					patient.setJourney(ward);
					patient.setEvents(events);
					patient.setStart(allWards[8]);
				 	patient.setEnd(allWards[8]);

                   pedestrians.push_back(patient);

                    for (int k = 0; k < 20; k++) {
					    Event event = events[randomInt(0, events.size() - 1)];
					    events.push_back(event);
				    }
				    patient.setEvents(events); 
				    pedestrians.push_back(patient);

				}
				 ofstream outf("C:\\Users\\ADMIN\\OneDrive - Hanoi University of Science and Technology\\Tài liệu\\GitHub\\BTL-IT3100-nh-m-3T2D\\data\pedestrian.txt", ios::app);
    if (!outf.is_open()) {
        cout << "File creation failed" << endl;
        return;
    }
    for (int i = 0; i < pedestrians.size(); i++) {
        json j;
        j["ID"] = pedestrians[i].getID();
        j["age"] = pedestrians[i].getAge();
        j["velocity"] = pedestrians[i].getVelocity();
        j["personality"]["name"] = pedestrians[i].getPersonality().getLambda() == 1 ? "open" : "neurotic";
        j["personality"]["lambda"] = pedestrians[i].getPersonality().getLambda(); 
        j["personality"]["positiveEmotionThreshold"] = pedestrians[i].getPersonality().getPositiveEmotionThreshold();
        j["personality"]["negativeEmotionThreshold"] = pedestrians[i].getPersonality().getNegativeEmotionThreshold(); 

        j["emotion"]["pleasure"] = pedestrians[i].getEmotion().getPleasure();
        j["emotion"]["surprise"] = pedestrians[i].getEmotion().getSurprise();
        j["emotion"]["anger"] = pedestrians[i].getEmotion().getAnger(); 
        j["emotion"]["fear"] = pedestrians[i].getEmotion().getFear(); 
        j["emotion"]["hate"] = pedestrians[i].getEmotion().getHate(); 
        j["emotion"]["sad"] = pedestrians[i].getEmotion().getSad();
		
        j["events"] = pedestrians[i].getEvents();

        outf << j << endl;
	}
    outf.close();

    cout << "Upload complete!" << endl;
   return pedestrians;
};
	


//Bai4

vector<pair<Ward,int>> checkWard( vector<Ward> allWards, int triple, int single){
    vector<pair<Ward,int>> res;
    int sum= triple*3 + single ;
    double mean = 0.0; // Mean of normal distribution
  double std_dev = 50.0;
   
random_device rd;
  mt19937 gen(rd());
  normal_distribution<double> distribution(mean, std_dev);

  // Generate random values from normal distribution
  vector<double> random_values;
  for (int i = 0; i < allWards.size(); ++i) {
    random_values.push_back(distribution(gen));
  }

 
  vector<int> integer_values;
  for (double value : random_values) {
    // Round to the nearest integer
    integer_values.push_back(static_cast<int>(round(value)));
  }

 
  double current_sum = 0.0;
  for (int value : integer_values) {
    current_sum += value;
  }

  double scaling_factor = sum / current_sum;
  for (int& value : integer_values) {
    value = static_cast<int>(round(value * scaling_factor));
  }

for( int i=0 ;i<allWards.size();i++){
   res.push_back(make_pair(allWards[i],integer_values[i]));
}

    return res;
                    }



//Bai5:
void mappingPesdestrian(vector<Pedestrians> pedestrians,vector<Ward> ward,vector<pair<Ward,int>> checkWard){
	 map<int,int> dict;
	 
	 //1:B 2:E 3:F 4:G 5:K 6:L 7:M 8:N 9:W
	 dict[1]=0;
	 dict[2]=0;
	 dict[3]=0;
	 dict[4]=0;
	 dict[5]=0;
	 dict[6]=0;
	 dict[7]=0;
	 dict[8]=0;
	 dict[9]=0;
	
		for(pair<Ward,int> i: checkWard ){
			
				if(i.first.getNameOfWard()=="B") dict[1]+=i.second;
				if(i.first.getNameOfWard()=="E") dict[2]+=i.second;
				if(i.first.getNameOfWard()=="F") dict[3]+=i.second;
				if(i.first.getNameOfWard()=="G") dict[4]+=i.second;
				if(i.first.getNameOfWard()=="K") dict[5]+=i.second;
				if(i.first.getNameOfWard()=="L") dict[6]+=i.second;
				if(i.first.getNameOfWard()=="M") dict[7]+=i.second;
				if(i.first.getNameOfWard()=="N") dict[8]+=i.second;
				if(i.first.getNameOfWard()=="W") dict[9]+=i.second;
			}


while(dict.size()!=0){
	for(Pedestrians pes: pedestrians){
			if(pes.getJourney().size()==1){
				vector<Ward> ward;
				 auto it = max_element(dict.begin(), dict.end(),   // tim ra vi tri cua ward con xuat hien nhieu nhat
                        [](const auto& a, const auto& b) {
                          return a.second < b.second;
                        });
			ward.push_back(checkWard[it->first].first); // cho ward do vao 
			pes.setJourney(ward);
			dict[it->first]-=1;   
			if(dict[it->first]==0) dict.erase(it->first);//kiem tra xem da het chua
		
			

    }
	else{
		map<int,int> ward;
		vector<Ward> res;
		ward[0]=0;
		ward[1]=0;
		ward[2]=0;
		for(auto d :dict){   // lay ra 3 phan tu co value lon nhat trong day
			int value= d.second;
			if(value>dict[ward[2]]){
				if(value>dict[ward[1]]){
					if(value>dict[ward[0]]){
						ward[2]=ward[1];    //so sanh gia tri va dua vi tri vao trong mang
						ward[1]=ward[0];
						ward[0]=d.first;
					}else{
						ward[2]=ward[1];
						ward[1]=d.first;
					}
				}else{
					ward[2]=d.first;
				}
			};

		}
		res.push_back(checkWard[ward[0]].first);
		res.push_back(checkWard[ward[1]].first);
		res.push_back(checkWard[ward[2]].first);
		if(dict[ward[0]]==0) dict.erase(ward[0]);
		if(dict[ward[1]]==0) dict.erase(ward[1]);
		if(dict[ward[2]]==0) dict.erase(ward[2]);

		pes.setJourney(res);
	}
			}
	}
	}



		
		


		
	
	
	
	
	



int main(){
	vector<Pedestrians> allPedestrians= genPedestrians();
	vector<Ward> allWards= genWard();
	mappingPesdestrian(allPedestrians,allWards,checkWard(allWards,190,310));
}
	
	








	


