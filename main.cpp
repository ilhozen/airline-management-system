#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
using namespace std;

#define DONOTHING ;

class Seat
{
private:
    string name;
    bool isOccupied;

public:
    Seat() { isOccupied = false; }
    void setName(string _name) { name = _name; }
    void setIsOccupied(bool _isOccupied) { isOccupied = _isOccupied; };
    string getName() { return name; }
    bool getIsOccupied() { return isOccupied; };
};

class Passenger
{
private:
    string name;
    string surname;
    char gender;
    string seatName;

public:
    Passenger(string name, string surname, char gender)
        : name(name), surname(surname), gender(gender) {}
    string getname() const { return name; }
    string getsurname() const { return surname; }
    char getgender() const { return gender; }
    string getseatName() const { return seatName; }
    void setname(string n) { n = name; }
    void setsurname(string sn) { sn = surname; }
    void setgender(char g) { g = gender; }
    void setseatName(string _seatName) { seatName = _seatName; }

    friend ostream &operator<<(ostream &output, const Passenger &passenger)
    {
        output << "Name: " << passenger.name << ", Surname: " << passenger.surname << ", Gender: " << passenger.gender;
        return output;
    }

    bool operator==(const Passenger &other) const
    {
        return (this->name == other.name && this->surname == other.surname);
    }
};

string stringToUpper(string a)
{
    transform(a.begin(), a.end(), a.begin(), ::toupper);
    return a;
}

bool compareSeats(const Passenger a, const Passenger b)
{
    const string aSeatName = stringToUpper(a.getseatName());
    const string bSeatName = stringToUpper(b.getseatName());
    return (aSeatName < bSeatName);
}

bool isLetter(const char &c)
{
    if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
    {
        return true;
    }
    return false;
}

bool isNumber(const char &c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    return false;
}

bool isSpace(const char &c)
{
    if (c == ' ')
    {
        return true;
    }
    return false;
}

bool isValidFlightNo(const string &flightNo)
{
    if (flightNo.length() != 4 && flightNo.length() != 5 && flightNo.length() != 6 && flightNo.length() != 7)
    {
        return false;
    }

    if (!isLetter(flightNo[0]) || !isLetter(flightNo[1]))
    {
        return false;
    }

    if (!isSpace(flightNo[2]))
    {
        return false;
    }

    for (int i = 3; i < flightNo.length(); i++)
    {
        if (!isNumber(flightNo[i]))
        {
            return false;
        }
    }

    return true;
}

class Flight
{
private:
    string flightNo;
    string destination;
    int maxSeats;
    int numPassengers;
    vector<Passenger> passengers;
    Seat seats[10][4];

public:
    Flight(string flightNo, string destination)
        : flightNo(flightNo), destination(destination), maxSeats(40), numPassengers(0)
    {
        string row;

        for (int i = 0; i <= 9; i++)
        {
            char column = 'A';
            for (int j = 0; j <= 3; j++)
            {
                row = to_string(i + 1);
                seats[i][j].setName(row + column);
                column += 1;
            }
        }
    }

    string getFlightNo() const { return flightNo; }

    bool reserveSeat(Passenger &client)
    {
        int check = 1;
        bool foundSeat = false;
        if (numPassengers < maxSeats)
        {
            char available;
            string seatSelection;
            cout << "\n   Legend:" << endl;
            cout << "   X - Occupied Seat" << endl;
            cout << "   O - Vacant Seat" << endl
                 << endl;
            cout << "Seating Plan:" << endl;
            cout << "------------Front------------"
                 << endl;
            for (int i = 0; i <= 9; i++)
            {
                cout << "|";
                for (int j = 0; j <= 3; j++)
                {
                    if (seats[i][j].getIsOccupied())
                    {
                        available = 'X';
                    }
                    else
                    {
                        available = 'O';
                    }
                    if (i != 9)
                    {
                        cout << seats[i][j].getName() << "  " << available << " "
                             << "|";
                    }
                    else
                    {
                        cout << seats[i][j].getName() << " " << available << " "
                             << "|";
                    }
                }
                cout << endl;
            }
            cout << "-----------------------------\n" << endl;
            do
            {
                check = 1;
                cout << "Select Seat: ";
                cin >> seatSelection;
                for (int i = 0; i <= 9; i++)
                {
                    for (int j = 0; j <= 3; j++)
                    {
                        if (seats[i][j].getName() == seatSelection && seats[i][j].getIsOccupied() == false)
                        {
                            client.setseatName(seatSelection);
                            seats[i][j].setIsOccupied(true);
                            foundSeat = true;
                            break;
                        }
                        else if (seats[i][j].getName() == seatSelection && seats[i][j].getIsOccupied() == true)
                        {
                            cout << "Selected Seat is Already Occupied. Please try again." << endl;
                            check = 0;
                        }
                    }
                    if (foundSeat)
                    {
                        break;
                    }
                }
                if (!foundSeat)
                {
                    cout << "Seat not found please try again." << endl;
                    check = 0;
                }
            } while (check == 0);

            passengers.push_back(client);
            numPassengers++;
            return true;
        }
        else
            return false;
    }

    bool cancelSeat(Passenger &client)
    {
        for (Passenger p : passengers)
        {
            if (p == client)
            {
                for (int i = 0; i <= 9; i++)
                {
                    for (int j = 0; j <= 3; j++)
                    {
                        if (seats[i][j].getName() == p.getseatName())
                        {
                            seats[i][j].setIsOccupied(false);
                        }
                    }
                }
                passengers.erase(find(passengers.begin(), passengers.end(), p));
                numPassengers--;
                return true;
            }
        }
        return false;
    }

    int numberOfPassengers() { return numPassengers; }

    void printPassengers()
    {
        sort(passengers.begin(), passengers.end(), compareSeats);
        cout << "Passenger List for Flight " << flightNo << ":" << endl;
        cout << "Seat|Passenger Name |Gender" << endl;
        cout << "----|---------------|------" << endl;
        for (const Passenger &p : passengers)
        {
            cout << left << setw(4) << p.getseatName();
            cout << "|";
            cout << left << setw(15);
            if ((p.getname().length() + p.getsurname().length()) > 14)
            {
                cout << (p.getname() + " " + p.getsurname()).substr(0, 14) + ".|";
            }
            else
            {
                cout << p.getname() + " " + p.getsurname() << "|";
            }
            cout << left << p.getgender() << endl;
        }
    }

    bool isFlyingTo(const string &location)
    {
        if (location == destination)
        {
            return true;
        }
    }

    friend ostream &operator<<(ostream &output, const Flight &flight)
    {
        output << "Flight Number: " << flight.flightNo << endl
               << "Destination: " << flight.destination << endl
               << endl;
        return output;
    }

    bool operator==(const Flight &other) const
    {
        return (this->flightNo == other.flightNo);
    }
};

class FlightManager
{
private:
    vector<Flight> flights;

public:
    void addFlight(const Flight &flight)
    {
        flights.push_back(flight);
    }

    bool removeFlight(const string &flightNumber)
    {
        Flight flight(flightNumber, "");
        for (Flight f : flights)
        {
            if (f.getFlightNo() == flightNumber)
            {
                flights.erase(find(flights.begin(), flights.end(), flight));
                return true;
            }
        }
        return false;
    }

    int numberOfFlights()
    {
        return flights.size();
    }

    void listAllFlights()
    {
        int i = 1;
        for (Flight p : flights)
        {
            cout << "Flight " << i << endl;
            cout << p;
            i++;
        }
    }

    Flight *getFlightByNumber(const string &flightNumber)
    {
        for (Flight &f : flights)
        {
            if (f.getFlightNo() == flightNumber)
            {
                return &f;
                break;
            }
        }
        return nullptr;
    }

    void *getFlightByDestination(const string &destination)
    {
        for (Flight &f : flights)
        {
            if (f.isFlyingTo(destination))
            {
                return &f;
                break;
            }
        }
        return nullptr;
    }
};

int main(int argc, char **argv)
{

    FlightManager flightManager;
    char gender;
    int size, choice, check = 1;
    string name, surname, flightNumber, destination;
    do
    {
        cout << "--------------------------------------------------" << endl;
        cout << "|                  Flight Manager                |" << endl;
        cout << "|------------------------------------------------|" << endl;
        cout << "|1. Add a Flight                                 |" << endl;
        cout << "|------------------------------------------------|" << endl;
        cout << "|2. Remove a Flight                              |" << endl;
        cout << "|------------------------------------------------|" << endl;
        cout << "|3. List All Flights                             |" << endl;
        cout << "|------------------------------------------------|" << endl;
        cout << "|4. Select a Flight and Manage Passengers        |" << endl;
        cout << "|------------------------------------------------|" << endl;
        cout << "|5. Exit                                         |" << endl;
        cout << "|------------------------------------------------|" << endl;
        cout << "Selection: ";
        cin >> choice;
        cout << endl;
        switch (choice)
        {
        case 1:
        {
            cout << "Add Flight to Flight Manager" << endl;
            cout << "----------------------------\n" << endl;
            cin.ignore();
            do
            {
                cout << "Enter Flight Number: ";
                getline(cin, flightNumber);
                check = 1;
                if (isValidFlightNo(flightNumber))
                {
                    DONOTHING
                }
                else
                {
                    cout << "\nEntered flight number was invalid!" << endl;
                    cout << "A valid flight number consists of:" << endl;
                    cout << "2 letters followed by a space and 1-4 numbers." << endl;
                    cout << "Example: AB 1234\n" << endl;
                    check = 0;
                }
            } while (check == 0);
            check = 1;
            cout << "Enter Flight Destination: ";
            cin >> destination;
            Flight flight(flightNumber, destination);
            flightManager.addFlight(flight);
            cout << "\nFlight " << flightNumber << " has been added.\n" << endl;
            break;
        }
        case 2:
        {
            cout << "Remove Flight from Flight Manager" << endl;
            cout << "---------------------------------\n" << endl;
            cin.ignore();
            do
            {
                cout << "Enter Flight Number: ";
                getline(cin, flightNumber);
                check = 1;
                if (isValidFlightNo(flightNumber))
                {
                    DONOTHING
                }
                else
                {
                    cout << "Entered flight number was invalid!" << endl;
                    cout << "A valid flight number consists of:" << endl;
                    cout << "2 letters followed by a space and 1-4 numbers." << endl;
                    cout << "Example: AB 1234" << endl;
                    check = 0;
                }
            } while (check == 0);
            check = 1;
            if (flightManager.removeFlight(flightNumber))
            {
                cout << "Flight removed successfully\n" << endl;
            }
            else
            {
                cout << "Flight with given number does not exist.\n" << endl;
            }
            break;
        }
        case 3:
        {
            if (flightManager.numberOfFlights() != 0)
            {
                cout << "List of All Flights" << endl;
                cout << "------------------------\n" << endl;
                check = 1;
                flightManager.listAllFlights();
            }
            else
            {
                cout << "No flights in flight manager.\n" << endl;
            }
            break;
        }
        case 4:
        {
            cin.ignore();
            do
            {
                cout << "Enter Flight Number to Manage Passengers: ";
                getline(cin, flightNumber);
                check = 1;
                if (isValidFlightNo(flightNumber))
                {
                    DONOTHING
                }
                else
                {
                    cout << "Entered flight number was invalid!" << endl;
                    cout << "A valid flight number consists of:" << endl;
                    cout << "2 letters followed by a space and 1-4 numbers." << endl;
                    cout << "Example: AB 1234" << endl;
                    check = 0;
                }
            } while (check == 0);
            check = 1;
            Flight *selectedFlight = flightManager.getFlightByNumber(flightNumber);
            if (selectedFlight != nullptr)
            {
                int flightMenuChoice;
                do
                {
                    cout << "-------------------------------------------" << endl;
                    cout << "|               Flight Menu               |" << endl;
                    cout << "|-----------------------------------------|" << endl;
                    cout << "|1. Reserve a Ticket                      |" << endl;
                    cout << "|-----------------------------------------|" << endl;
                    cout << "|2. Cancel Reservation                    |" << endl;
                    cout << "|-----------------------------------------|" << endl;
                    cout << "|3. View Passenger List                   |" << endl;
                    cout << "|-----------------------------------------|" << endl;
                    cout << "|4. Back to Flight Management Menu        |" << endl;
                    cout << "|-----------------------------------------|" << endl;
                    cout << "Selection: ";
                    cin >> flightMenuChoice;
                    cout << endl;

                    switch (flightMenuChoice)
                    {
                    case 1:
                    {
                        do
                        {
                            cout << "Name of passenger: ";
                            cin >> name;
                            check = 1;
                            for (char i : name)
                            {
                                if (i >= 'a' && i <= 'z' || i >= 'A' && i <= 'Z')
                                {
                                    DONOTHING
                                }
                                else
                                {
                                    cout << "Name contains unexpected characters. Please try again.\n" << endl;
                                    check = 0;
                                    break;
                                }
                            }
                        } while (check == 0);
                        check = 1;
                        do
                        {
                            cout << "Surname of passenger: ";
                            cin >> surname;
                            check = 1;
                            for (char i : surname)
                            {
                                if (i >= 'a' && i <= 'z' || i >= 'A' && i <= 'Z')
                                {
                                    DONOTHING
                                }
                                else
                                {
                                    cout << "Surname contains unexpected characters. Please try again.\n" << endl;
                                    check = 0;
                                    break;
                                }
                            }
                        } while (check == 0);
                        check = 1;
                        cout << "Gender of passenger (M/F): ";
                        cin >> gender;

                        Passenger passenger(name, surname, gender);
                        if (selectedFlight->reserveSeat(passenger))
                        {
                            cout << "The seat has been reserved.\n" << endl;
                        }
                        else
                        {
                            cout << "The seat has not been reserved. The flight is full.\n" << endl;
                        }
                        break;
                    }
                    case 2:
                    {
                        cout << "Reservation Cancellation\n" << endl;
                        do
                        {
                            cout << "Name of Ticket Owner: ";
                            cin >> name;
                            check = 1;
                            for (char i : name)
                            {
                                if (i >= 'a' && i <= 'z' || i >= 'A' && i <= 'Z')
                                {
                                }
                                else
                                {
                                    cout << "Name contains unexpected characters. Please try again.\n" << endl;
                                    check = 0;
                                    break;
                                }
                            }
                        } while (check == 0);
                        check = 1;
                        do
                        {
                            cout << "Surname of Ticket Owner: ";
                            cin >> surname;
                            check = 1;
                            for (char i : surname)
                            {
                                if (i >= 'a' && i <= 'z' || i >= 'A' && i <= 'Z')
                                {
                                }
                                else
                                {
                                    cout << "Surname contains unexpected characters. Please try again.\n" << endl;
                                    check = 0;
                                    break;
                                }
                            }
                        } while (check == 0);
                        check = 1;
                        Passenger passenger(name, surname, ' ');
                        if (selectedFlight->cancelSeat(passenger))
                        {
                            cout << "Reservation canceled successfully.\n" << endl;
                        }
                        else
                        {
                            cout << "Failed to cancel reservation. Passenger was not found!\n" << endl;
                        }
                        break;
                    }
                    case 3:
                    {
                        selectedFlight->printPassengers();
                        break;
                    }
                    case 4:
                    {
                        cout << "Returning to Flight Manager Menu...\n" << endl;
                        break;
                    }
                    default:
                        cout << "Invalid choice. Please try again.\n" << endl;
                    }
                } while (flightMenuChoice != 4);
            }
            else
            {
                cout << "Flight with number " << flightNumber << " not found!\n" << endl;
                check = 0;
            }
            break;
        }
        case 5:
            cout << "Exiting...\n" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again.\n" << endl;
        }
    } while (choice != 5);

    return 0;
}