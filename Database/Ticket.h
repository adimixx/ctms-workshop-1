//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_TICKET_H
#define WORKSHOP1_TICKET_H
#include <string>
#include <boost/optional.hpp>
using namespace std;

class Ticket{
public:
    Ticket(long id, long receiptId, long packageId, long deckId, long ticketTypeId, string dateIssued, string name,
           string phone, string email, double price, bool checkIn, boost::optional<long> checkInUserId)
            : ID(id), receiptID(receiptId), packageID(packageId), deckID(deckId), ticketTypeID(ticketTypeId),
              dateIssued(dateIssued), name(name), phone(phone), email(email), price(price), checkIn(checkIn),
              checkInUserID(checkInUserId) {};
    Ticket(){};

    long ID;
    long receiptID;
    long packageID;
    long deckID;
    long ticketTypeID;
    string dateIssued;
    string name;
    string phone;
    string email;
    double price;
    bool checkIn;
    boost::optional<long> checkInUserID;
};
#endif //WORKSHOP1_TICKET_H
