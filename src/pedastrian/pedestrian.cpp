#include "pedestrian.h"
#include "lib/nlohmann/json.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>


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
				edges.push_back(point1);
				
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
	string line;
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

	// chia tỉ lệ :500=160+190+150

	// chia nhỏ:75+95+100+90+113+37 
	Pesonality open, neurotic;
    open.setLambda(double(inputData["personalityDistribution"]["distribution"]["open"]["lambda"]));
    open.setPositiveEmotionThreshold(double(inputData["personalityDistribution"]["distribution"]["open"]["positiveEmotionThreshold"]));
    open.setNegativeEmotionThreshold(double(inputData["personalityDistribution"]["distribution"]["open"]["negativeEmotionThreshold"]));
    neurotic.setLambda(double(inputData["personalityDistribution"]["distribution"]["neurotic"]["lambda"]));
    neurotic.setPositiveEmotionThreshold(double(inputData["personalityDistribution"]["distribution"]["neurotic"]["positiveEmotionThreshold"]));
    neurotic.setNegativeEmotionThreshold(double(inputData["personalityDistribution"]["distribution"]["neurotic"]["negativeEmotionThreshold"]));


	double NoDisabilityNoOvertaking_velocity = double(inputData["walkability"]["distribution"]["noDisabilityNoOvertaking"]["velocity"]) * lechChuan;
    double NoDisabilityOvertaking_velocity = double(inputData["walkability"]["distribution"]["noDisabilityOvertaking"]["velocity"]) * lechChuan;
    double Crutches_velocity = double(inputData["walkability"]["distribution"]["crutches"]["velocity"]) * lechChuan;
    double Sticks_velocity = double(inputData["walkability"]["distribution"]["sticks"]["velocity"]) * lechChuan;
    double Wheelchair_velocity = double(inputData["walkability"]["distribution"]["wheelchair"]["velocity"]) * lechChuan;
    double Blind_velocity = double(inputData["walkability"]["distribution"]["blind"]["velocity"]) * lechChuan;
	int tiLePesdetrian[]={160,190,150};

	
				
				while(ID++<=160){
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
				}
				while(ID++<350){
					Visitor visitor;
                    vector<Event> events;
                    visitor.setID(ID);

                    visitor.setAge(ages[randomInt(0, ages.size() - 1)]);
                    visitor.setPersonality(ID <= 250 ? open : (visitor.getAge() < 11 ? open : neurotic));
                    visitor.setVelocity(ID <= 280 ? (ID <= 180 ? NoDisabilityOvertaking_velocity : Crutches_velocity) : Sticks_velocity);
					for( int i=0; i<20;i++){
						 Event event = events[randomInt(0, events.size() - 1)];
						 events.push_back(event);
					}
					visitor.setEvents(events);
                   pedestrians.push_back(visitor);
				}
				while(ID++<500){
					
				    Patient patient; 
				    vector<Event> events;
				    patient.setID(ID); 

				    patient.setAge(ages[randomInt(0, ages.size() - 1)]); 
                    patient.setPersonality(patient.getAge() < 11 ? open : neurotic);  
                    patient.setVelocity(ID <= 113 ? Wheelchair_velocity : Blind_velocity); 
                    

                    for (int k = 0; k < 20; k++) {
					    Event event = events[randomInt(0, events.size() - 1)];
					    events.push_back(event);
				    }
				    patient.setEvents(events); 
				    pedestrians.push_back(patient);

				}
				 ofstream outf("data/pedestrian.txt", ios::app);
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
    return;
}
					


		

	
	








	



