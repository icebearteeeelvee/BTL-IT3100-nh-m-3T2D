
#define PEDESTRIAN_H



#include <vector>
#include <string>
using namespace std;

struct Point{
    double x,y;
};

class Ward{
    private: 
        string nameOfWard;
        Point enter ;
        Point exit;
        vector<Point> wallBound;
    public:
    Ward(){};
    Ward(string nameOfward, Point enter, Point exit, vector<Point> wallBound){};

    void setNameOfWard(){ this->nameOfWard= nameOfWard;}
    void setEnter(){ this->enter=enter;}
    void setExit(){ this->exit=exit;}
    void setWallBound(){this->wallBound=wallBound;}

    string getNameOfWard() {return this->nameOfWard ;}
    Point getEnter(){ return this->enter;}
    Point getExit(){ return this->exit;}
    vector<Point> getWallBound(){ return this->wallBound;}
    
};

class Emotion{
    private:
        double pleasure;
        double surprise;
        double anger;
        double fear;
        double hate;
        double sad;
        public:
    Emotion() : pleasure(0.75), surprise(0.5), anger(-0.2), fear(-0.2), hate(-0.4), sad(-0.4) {}

    double getPleasure() const { return pleasure; }
    double getSurprise() const { return surprise; }
    double getAnger() const { return anger; }
    double getFear() const { return fear; }
    double getHate() const { return hate; }
    double getSad() const { return sad; }
};

class Pesonality{
    private:
    double lambda;
    double positiveEmotionThreshold;
    double negativeEmotionThreshold;

public:
    double getLambda() const { return lambda; }
    double getPositiveEmotionThreshold() const { return positiveEmotionThreshold; }
    double getNegativeEmotionThreshold() const { return negativeEmotionThreshold; }

    void setLambda(double lambda) { this -> lambda = lambda; }
    void setPositiveEmotionThreshold(double positiveEmotionThreshold) { this -> positiveEmotionThreshold = positiveEmotionThreshold; }
    void setNegativeEmotionThreshold(double negativeEmotionThreshold) { this -> negativeEmotionThreshold = negativeEmotionThreshold; }
};

class Event {
private:
    double time;
    vector<double> intensity;
public:
    Event();
    Event(double time, vector<double> intensity);
    double getTime() const { return time; }
    vector<double> getIntensity() const { return intensity; }

    void setTime(double time) { this -> time = time; }
    void setIntensity(vector<double> intensity) { this -> intensity = intensity; }
};

enum Walkability{noDisability, crutches, sticks, wheelchairs , blind};

class AGVEvents : Event{};
class Pedestrians{
    private:
    int ID;
    Ward start,end;
    vector<Ward> journey;
    double velocity;
    Pesonality personality;
    Emotion emotion;
    vector<Event> events;
    double walkingTime, distance,age;
    AGVEvents impactOfAGV ;
    Point tempPoint; 

    public:
    Pedestrians(){};
    Pedestrians(int ID, Ward start, Ward end, double velocity, Pesonality personality,
     Emotion emotion , vector<Event> &events, double walkingTime,
     double distance, double age, AGVEvents impactOfAGV , Point tempPoint){};
    
 int getID() const { return ID; }
  void setID(int id) { ID = id; }

  const Ward& getStart() const { return this->start; }
  void setStart(const Ward& newStart) { this->start = newStart; }

  const Ward& getEnd() const { return this->end; }
  void setEnd(const Ward& newEnd) { this->end = newEnd; }

  const vector<Ward>& getJourney() const { return this->journey; }
  void setJourney(const vector<Ward>& newJourney) {this-> journey = newJourney; }

  double getVelocity() const { return this-> velocity; }
  void setVelocity(double newVelocity) {this-> velocity = newVelocity; }
 
  const Pesonality& getPersonality() const { return this-> personality; }
  void setPersonality(const Pesonality& newPersonality) { this->personality = newPersonality; }

  const Emotion& getEmotion() const { return this->emotion; }
  void setEmotion(const Emotion& newEmotion) {this-> emotion = newEmotion; }

  vector<vector<double>> getEvents() { 
        vector<vector<double>> eventPerEmotion(6,vector<double>(20));
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 20; j++) {
                eventPerEmotion[i][j] = events[j].getIntensity()[i];
            }
        }
        return eventPerEmotion;
    }

  void setEvents(const vector<Event>& newEvents) {this-> events = newEvents; }

  double getWalkingTime() const { return this->walkingTime; }
  void setWalkingTime(double newWalkingTime) { this->walkingTime = newWalkingTime; }

  double getDistance() const { return this->distance; }
  void setDistance(double newDistance) { this->distance = newDistance; }

  double getAge() const { return this->age; }
  void setAge(double newAge) { this->age = newAge; }

  
  const AGVEvents& getImpactofAGV() const { return this->impactOfAGV; }
  void setImpactofAGV(const AGVEvents& newImpactofAGV) {this-> impactOfAGV = newImpactofAGV; }

  
  const Point& getTempPoint() const { return this->tempPoint; }
  void setTempPoint(const Point& newTempPoint) { this->tempPoint = newTempPoint; }
};

class Patient : public Pedestrians {
    Walkability walkability;
};

class Visitor : public Pedestrians {
    Walkability walkability;
};

class Personnel : public Pedestrians {
    Walkability walkability;
};
