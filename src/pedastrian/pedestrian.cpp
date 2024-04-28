#include "pedestrian.h"
#include "C:\\Users\\ADMIN\\OneDrive - Hanoi University of Science and Technology\\Tài liệu\\GitHub\\BTL-IT3100-nh-m-3T2D\\lib\\nlohmann\\json.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>

//#include <matplotlibcpp.h> 


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



void genPedestrians(){
	
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

				int i= randomInt(0,6);
				ward.push_back(allWards[i]);
				int j = randomInt(i,7);
				ward.push_back(allWards[j]);
				ward.push_back(allWards[randomInt(j,8)]);
				personnel.setJourney(ward);

				personnel.setJourney(ward);
				personnel.setEvents(events);

				Ward des = allWards[randomInt(0,9)];
				personnel.setStart(des);
				personnel.setEnd(des);

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
				int i= randomInt(0,6);
				ward.push_back(allWards[i]);
				int j = randomInt(i,7);
				ward.push_back(allWards[j]);
				ward.push_back(allWards[randomInt(j,8)]);
				visitor.setJourney(ward);

				visitor.setStart(allWards[8]);
				visitor.setEnd(allWards[8]);     //Về sau phải sửa cái này về khu A tức allWards[9]

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
                    ward.push_back(allWards[randomInt(1,9)]);
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
				 ofstream outf("D:\\Downloads\\BTL\\BTL-IT3100-nh-m-3T2D-bai_3\\data\\pedestrian.txt", ios::app);
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
};
//Bai_7:
vector<vector<double>> getImpact(int samples, double min, double max){
	vector<vector<double>> impacts;
    for (int i = 0; i < samples; i++) {
        vector<double> impact(6);
        for (int j = 0; j < 6; j++) {
            impact[j] = lround(randomDouble(min, max) * 100) / 100.0;
        }
        impacts.push_back(impact);
    }

    return impacts;
	};

vector<vector<double>> impactOfAGV(vector<Pedestrians> allPedestrians){
	vector<vector<double>> res;

	int children = 0, ALKW = 0, BFGMEN = 0, elder = 0, blinder = 0, others = 0;
	bool isOthers=true;
	for( Pedestrians pes: allPedestrians){
		if(pes.getAge()<12){
			children++;
			isOthers=false;} //1:visitor 2//patient 3:personnel
		if((pes.getID()<160&& pes.getVelocity() > 1)||(pes.getID()>160&&pes.getID()<350)){
			string workplace = pes.getStart().getNameOfWard();
            if (workplace == "A" || workplace == "L" || workplace == "K" || workplace == "W") ALKW++;
            else BFGMEN++;
			isOthers=false;

		}
		if(pes.getAge()>60)
			{ elder++;
			isOthers=false;
			}
		if (pes.getVelocity() < 0.58){ 
			blinder++;
			isOthers=false;}
		if(isOthers==true) others++;
	}
	vector<vector<double>> childrenImpacts = getImpact(children, 0.01, 0.99);
    vector<vector<double>> ALKWImpacts = getImpact(ALKW, -0.29, 0.99);
    vector<vector<double>> BFGMENImpacts = getImpact(BFGMEN, -0.99, 0.29);
    vector<vector<double>> elderImpacts = getImpact(elder, -0.99, 0.29);
    vector<vector<double>> blinderImpacts = getImpact(blinder, -0.99, 0.29);
    vector<vector<double>> othersImpacts = getImpact(others, -0.99, 0.99);
	for( Pedestrians pes: allPedestrians){
	//	vector<vector<double>> impactOfAGV= {{0},{0},{0},{0},{0},{0}};
		  AGVEvents agv_event;
        if (pes.getAge() < 12) {
            agv_event.setIntensity(childrenImpacts.back());
            childrenImpacts.pop_back();
        }
        else if((pes.getID()<160&& pes.getVelocity() > 1)||(pes.getID()>160&&pes.getID()<350)){
            string workplace = pes.getStart().getNameOfWard();
            if (workplace == "A" || workplace == "L" || workplace == "K" || workplace == "W") {
                agv_event.setIntensity(ALKWImpacts.back());
                ALKWImpacts.pop_back();
            }
            else {
                agv_event.setIntensity(BFGMENImpacts.back());
                BFGMENImpacts.pop_back();
            }
        }
        else if (pes.getAge() > 60) {
            agv_event.setIntensity(elderImpacts.back());
            elderImpacts.pop_back();
        }
        else if (pes.getVelocity() < 0.58) {
            agv_event.setIntensity(blinderImpacts.back());
            blinderImpacts.pop_back();
        }
        else {
            agv_event.setIntensity(othersImpacts.back());
            othersImpacts.pop_back();
        }
        pes.setImpactofAGV(agv_event);
        res.push_back(agv_event.getIntensity());
    }
return res;
}



int main(){
	genPedestrians();
}

		

	
	








	



