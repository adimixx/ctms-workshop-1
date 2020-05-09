//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_DECK_H
#define WORKSHOP1_DECK_H
#include <string>
#include <boost/optional.hpp>
using namespace std;

class Deck{
public:
    Deck(long id, string name, int level, int maxPassenger, long premiumId, double premiumValue, long vesselId)
            : Id(id), PremiumID(premiumId), VesselID(vesselId), Name(name), Level(level), MaxPassenger(maxPassenger),
              PremiumValue(premiumValue) {};
    Deck(){};

    long Id, VesselID;
    string Name;
    int Level, MaxPassenger;
    long PremiumID;
    double PremiumValue;
};
#endif //WORKSHOP1_DECK_H
