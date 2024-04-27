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
//Bai_6:
vector<vector<double>> impactOnEmo(Pedestrians p, int timeHorizon){
	//events  = p.getEvents( )
	vector<vector<double>> events=p.getEvents();
	
	//lambda = p.getLambda( )
	double lambda = p.getPersonality().getLambda();
	//times = p.getTimeDistances( ) 
	vector<int> times(19);
	for (int i = 0; i < 19; ++i) {
        times[i] = randomInt(4, 10);
    }

	vector<vector<double>> allEmotions={
		{p.getEmotion().getAnger()},
		{p.getEmotion().getHate()},
		{p.getEmotion().getFear()},
		{p.getEmotion().getSad()},
		{p.getEmotion().getSurprise()},
		{p.getEmotion().getPleasure()},
	};

	double ple = events[0][0] + allEmotions[0][0]*exp(-lambda) + allEmotions[0][0];
	double su = events[1][0] + allEmotions[1][0]*exp(-lambda) + allEmotions[1][0];
	double a = events[2][0] + allEmotions[2][0]*exp(-lambda) + allEmotions[2][0];
	double f = events[3][0] + allEmotions[3][0]*exp(-lambda) + allEmotions[3][0];
	double h = events[4][0] + allEmotions[4][0]*exp(-lambda) + allEmotions[4][0];
	double sa = events[5][0] + allEmotions[5][0]*exp(-lambda) + allEmotions[5][0];

	int lastTime = 0;
	int index = 0;
	int H=allEmotions[0].size();
	int idxIn=H+2;


	for(int i=0;i<=5;i++){
	allEmotions[i].resize(H+1+H-1-2+1,0); //H+1 là kích thước mảng sau khi thêm phần tử đầu
										//(H-1)-2+1 là kích thước mảng append thêm sau vòng for tiếp theo
	}

	allEmotions[0][H+1]=ple;
	allEmotions[1][H+1]=su;
	allEmotions[2][H+1]=a;
	allEmotions[3][H+1]=f;
	allEmotions[4][H+1]=h;
	allEmotions[5][H+1]=sa;

	for(int i=2;i<H;i++){
		ple = allEmotions[0][i];
		su = allEmotions[1][i];
		a = allEmotions[2][i];
		f = allEmotions[3][i];
		h = allEmotions[4][i];
		sa = allEmotions[5][i];
		if(i - lastTime == times[index]){
			index += 1;
			int lastTime  = i;
			ple += allEmotions[0][i]*exp(-lambda) + events[0][index] ;
			su += allEmotions[1][i]*exp(-lambda) + events[1][index] ;
			a += allEmotions[2][i]*exp(-lambda) + events[2][index] ;
			f += allEmotions[3][i]*exp(-lambda) + events[3][index] ;
			h += allEmotions[4][i]*exp(-lambda) + events[4][index] ;
			sa += allEmotions[5][i]*exp(-lambda) + events[5][index] ;
		};//do là điều kiện đúng hay sai thì allEmotions[a][index] cũng đều append thêm giá trị
		// do đó bản thân mảng phải có giới hạn chứ không thể giới hạn bằng vi phạm điều kiện if

		
		
		allEmotions[0][idxIn]=ple;	// allEmotions[0].append(p);
		allEmotions[1][idxIn]=su;	// allEmotions[1].append(su);
		allEmotions[2][idxIn]=a;	// allEmotions[2].append(a);
		allEmotions[3][idxIn]=f;	// allEmotions[3].append(f);
		allEmotions[4][idxIn]=h;	// allEmotions[4].append(h);
		allEmotions[5][idxIn]=sa;	// allEmotions[5].append(sa);


		idxIn++;
	}

	return allEmotions;
	};






int main(){
	genPedestrians();
}

		

	
	








	



