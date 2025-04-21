#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <regex>
#include <algorithm>
#include <set>
using namespace std;

class Booking {
public:
    string name, email, roomType, dob, paymentMethod, upiID, cardNumber;
    int numGuests, quantity, stayDays;
    float totalCost, refundAmount;
    bool isPaid;
};

class Event {
public:
    string name, email, dob, eventType, foodOption, decoration;
    float totalCost;
};

class Resort {
    string name;
    int standardRooms, deluxeRooms, suiteRooms;
    float standardPrice, deluxePrice, suitePrice;
    int roomsBooked;
    map<int, Booking> bookings;
    map<int, Event> eventBookings;
    set<int> usedBookingIDs;

public:
    Resort(string n, int std, int del, int suite, float sP, float dP, float suP) {
        name = n;
        standardRooms = std;
        deluxeRooms = del;
        suiteRooms = suite;
        standardPrice = sP;
        deluxePrice = dP;
        suitePrice = suP;
        roomsBooked = 0;
        srand(time(0));
    }

    void displayInfo() {
        cout << "\n Welcome to " << name << "✨ " << endl;
        cout << "Where the vibe sets with the sun.\n";
        cout << "\nAvailable Rooms:\n";
        cout << "Standard Rooms: " << standardRooms << " (₹" << standardPrice << " per day)\n";
        cout << "Deluxe Rooms: " << deluxeRooms << " (₹" << deluxePrice << " per day)\n";
        cout << "Suite Rooms: " << suiteRooms << " (₹" << suitePrice << " per day)\n";
    }

    int generateUniqueBookingID() {
        int bookingNo;
        do {
            bookingNo = rand() % 90000 + 10000;
        } while (usedBookingIDs.count(bookingNo));
        usedBookingIDs.insert(bookingNo);
        return bookingNo;
    }

    bool isValidUPI(const string& upi) {
        const regex pattern(R"(^[\w.-]+@[\w.-]+$)");
        return regex_match(upi, pattern);
    }

    bool isValidCard(const string& card) {
        return card.length() == 16 && all_of(card.begin(), card.end(), ::isdigit);
    }

    void processPayment(Booking& b) {
        int option;
        do {
            cout << "\nSelect Payment Method:\n1. UPI\n2. Card\n3. Cash\nChoice: ";
            cin >> option;
            cin.ignore();

            switch (option) {
                case 1: {
                    string upi;
                    cout << "📷 Simulating QR Code Scan...\n";
                    cout << "Enter your UPI ID: ";
                    getline(cin, upi);
                    if (isValidUPI(upi)) {
                        b.paymentMethod = "UPI";
                        b.upiID = upi;
                        b.isPaid = true;
                    } else {
                        cout << "❌ Invalid UPI ID format.\n";
                    }
                    break;
                }
                case 2: {
                    string card;
                    cout << "Enter 16-digit Card Number: ";
                    getline(cin, card);
                    if (isValidCard(card)) {
                        b.paymentMethod = "Card";
                        b.cardNumber = card;
                        b.isPaid = true;
                    } else {
                        cout << "❌ Invalid card number. Must be 16 digits.\n";
                    }
                    break;
                }
                case 3:
                    b.paymentMethod = "Cash";
                    b.isPaid = true;
                    break;
                default:
                    cout << "❌ Invalid payment method.\n";
            }
        } while (!b.isPaid);

        cout << "\n🧾 Payment Receipt\n-------------------------\n";
        cout << "Payment Method: " << b.paymentMethod << "\n";
        if (b.paymentMethod == "UPI") cout << "UPI ID: " << b.upiID << "\n";
        if (b.paymentMethod == "Card") cout << "Card: **** **** **** " << b.cardNumber.substr(12, 4) << "\n";
        cout << "Amount Paid: Rs" << b.totalCost << "\n";
        cout << "-------------------------\n";
    }
int bookRoom(const string& name, const string& email, int guests,
                 string roomType, int quantity, const string& dob, int stayDays) {
        transform(roomType.begin(), roomType.end(), roomType.begin(), ::tolower);
        float cost = 0;
        bool success = false;

        if (roomType == "standard" && quantity <= standardRooms) {
            standardRooms -= quantity;
            cost = quantity * standardPrice * stayDays;
            success = true;
        } else if (roomType == "deluxe" && quantity <= deluxeRooms) {
            deluxeRooms -= quantity;
            cost = quantity * deluxePrice * stayDays;
            success = true;
        } else if (roomType == "suite" && quantity <= suiteRooms) {
            suiteRooms -= quantity;
            cost = quantity * suitePrice * stayDays;
            success = true;
        } else {
            cout << "❌ Invalid room type or insufficient availability!\n";
            return -1;
        }

        if (success) {
            int bookingNo = generateUniqueBookingID();
            Booking b = {name, email, roomType, dob, "", "", "", guests, quantity, stayDays, cost, 0.0f, false};
            processPayment(b);
            bookings[bookingNo] = b;
            roomsBooked += quantity;

            cout << "\n✅ Booking Confirmed!\nBooking No: " << bookingNo << "\n";
            cout << "Name: " << name << "\nEmail: " << email << "\nTotal: Rs" << cost << "\n";

            cout << "\n📧 Email Sent to " << email << "\n--------------------------\n";
            cout << "Dear " << name << ",\nYour booking is confirmed.\nBooking No: " << bookingNo;
            cout << "\nRoom: " << roomType << ", Guests: " << guests;
            cout << "\nStay: " << stayDays << " days, Total: ₹" << cost;
            cout << "\n--------------------------\n";

            return bookingNo;
        }

        return -1;
    }

    void checkIn(int bookingNo, const string& dob, const string& email) {
        auto it = bookings.find(bookingNo);
        if (it != bookings.end()) {
            Booking& b = it->second;
            if (b.dob == dob && b.email == email) {
                cout << "\n✅ Welcome " << b.name << "! Check-in successful.\n";
            } else {
                cout << "❌ DOB or Email mismatch.\n";
            }
        } else cout << "❌ Booking not found.\n";
    }

    void displayAllBookings() {
        if (bookings.empty()) {
            cout << "\nNo room bookings.\n";
        } else {
            for (auto& [id, b] : bookings) {
                cout << "\nBooking No: " << id << "\nName: " << b.name << "\nEmail: " << b.email;
                cout << "\nGuests: " << b.numGuests << "\nRoom: " << b.roomType;
                cout << "\nStay: " << b.stayDays << " days\nPaid: Rs" << b.totalCost;
                cout << "\nPayment Mode: " << b.paymentMethod << "\n";
            }
        }

        if (eventBookings.empty()) {
            cout << "\nNo event bookings.\n";
        } else {
            for (auto& [id, e] : eventBookings) {
                cout << "\nEvent Booking No: " << id << "\nName: " << e.name;
                cout << "\nEmail: " << e.email << "\nEvent: " << e.eventType;
                cout << "\nFood: " << e.foodOption << ", Decoration: " << e.decoration;
                cout << "\nTotal: Rs" << e.totalCost << "\n";
            }
        }
    }

    void cancelEvent(int bookingNo, const string& dob, const string& email) {
        auto it = eventBookings.find(bookingNo);
        if (it != eventBookings.end()) {
            Event& e = it->second;
            if (e.dob == dob && e.email == email) {
                float refund = e.totalCost * 0.5;
                cout << "\n❌ Event cancelled. Refund: ₹" << refund << "\n";
                eventBookings.erase(it);
            } else cout << "❌ Invalid DOB or Email.\n";
        } else cout << "❌ Event not found.\n";
    }
void editEvent(int bookingNo, const string& dob, const string& email) {
        auto it = eventBookings.find(bookingNo);
        if (it != eventBookings.end()) {
            Event& e = it->second;
            if (e.dob == dob && e.email == email) {
                float refund = e.totalCost * 0.25;
                cout << " Partial refund (25%): Rs" << refund << "\n";
                eventBookings.erase(it);
                bookEvent(); // Rebook
            } else cout << "❌ DOB or Email mismatch.\n";
        } else cout << "❌ Event not found.\n";
    }

    // Book Event Function is quite long, implemented below in full...
    void bookEvent() {
        string name, email, dob, eventType, decoration, food, upiID, cardNumber, paymentMode;
        float cost = 0;
        int bookingNo = generateUniqueBookingID();
        cin.ignore();

        do {
            cout << "Enter Name: ";
            getline(cin, name);
            if (name.empty()) cout << "❌ Name cannot be blank.\n";
        } while (name.empty());

        regex emailPattern(R"((\w+)(\.?\w+)*@(\w+)(\.\w+)+)");
        do {
            cout << "Enter Email: ";
            getline(cin, email);
            if (email.empty()) cout << "❌ Email cannot be blank.\n";
            else if (!regex_match(email, emailPattern)) cout << "❌ Invalid email format. Try again.\n";
        } while (email.empty() || !regex_match(email, emailPattern));

        regex dobPattern(R"(^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-\d{4}$)");
        do {
            cout << "DOB (dd-mm-yyyy): ";
            getline(cin, dob);
            if (dob.empty()) cout << "❌ DOB cannot be blank.\n";
            else if (!regex_match(dob, dobPattern)) cout << "❌ Invalid DOB format. Use dd-mm-yyyy.\n";
        } while (dob.empty() || !regex_match(dob, dobPattern));

        do {
            cout << "Choose Event Type (Birthday/Anniversary/Other): ";
            getline(cin, eventType);
            if (eventType.empty()) {
                cout << "❌ Event type cannot be blank.\n";
                continue;
            }

            string typeLower = eventType;
            transform(typeLower.begin(), typeLower.end(), typeLower.begin(), ::tolower);

            if (typeLower == "birthday") {
                eventType = "Birthday";
                cost = 10000;
            } else if (typeLower == "anniversary") {
                eventType = "Anniversary";
                cost = 15000;
            } else if (typeLower == "other") {
                cout << "Enter Custom Event Type: ";
                getline(cin, eventType);
                if (eventType.empty()) {
                    cout << "❌ Custom Event type cannot be blank.\n";
                    continue;
                }
                cost = 8000;
            } else {
                cout << "❌ Invalid event type. Try again.\n";
                eventType = "";
            }
        } while (eventType.empty());

        do {
            cout << "Add Decoration? (Yes/No): ";
            getline(cin, decoration);
            if (decoration.empty()) {
                cout << "❌ Decoration choice cannot be blank.\n";
                continue;
            }

            string decorLower = decoration;
            transform(decorLower.begin(), decorLower.end(), decorLower.begin(), ::tolower);
            if (decorLower == "yes") {decoration = "Yes";
                cost += 5000;
            } else if (decorLower == "no") {
                decoration = "No";
            } else {
                cout << "❌ Invalid input. Type Yes or No.\n";
                decoration = "";
            }
        } while (decoration.empty());

        // Food option
        do {
            cout << "Preferred Food Option (Veg/Non-Veg/Mix): ";
            getline(cin, food);
            if (food.empty()) cout << "❌ Food option cannot be blank.\n";
        } while (food.empty());
        cost += 3000;

        // Payment method
        do {
            cout << "\nChoose Payment Mode (UPI/Card/Cash): ";
            getline(cin, paymentMode);
            transform(paymentMode.begin(), paymentMode.end(), paymentMode.begin(), ::tolower);

            if (paymentMode == "upi") {
                do {
                    cout << "\n📱 Scan the QR code to pay (simulated)...";
                    cout << "\nEnter your UPI ID: ";
                    getline(cin, upiID);
                    if (upiID.empty()) cout << "❌ UPI ID cannot be blank.\n";
                    else if (!regex_match(upiID, regex(R"([a-zA-Z0-9_.]+@[a-zA-Z]+)"))) {
                        cout << "❌ Invalid UPI ID format. It should be like username@bank\n";
                        upiID = "";
                    }
                } while (upiID.empty());
            } else if (paymentMode == "card") {
                do {
                    cout << "Enter 16-digit Card Number: ";
                    getline(cin, cardNumber);
                    if (cardNumber.length() != 16 || !all_of(cardNumber.begin(), cardNumber.end(), ::isdigit))
                        cout << "❌ Invalid Card Number. Must be 16 digits.\n";
                } while (cardNumber.length() != 16 || !all_of(cardNumber.begin(), cardNumber.end(), ::isdigit));
            } else if (paymentMode == "cash") {
                cout << "💵 Please collect cash payment at the counter.\n";
            } else {
                cout << "❌ Invalid payment mode. Choose UPI, Card, or Cash.\n";
                paymentMode = "";
            }
        } while (paymentMode.empty());

        // Save booking
        Event e = {name, email, dob, eventType, food, decoration, cost};
        eventBookings[bookingNo] = e;

        // 🧾 Final Receipt
        cout << "\n✅ Event Booking Confirmed!\n";
        cout << "----------------------------------------\n";
        cout << "Booking No     : " << bookingNo << "\n";
        cout << "Name           : " << name << "\n";
        cout << "Email          : " << email << "\n";
        cout << "DOB            : " << dob << "\n";
        cout << "Event Type     : " << eventType << "\n";
        cout << "Food Option    : " << food << "\n";
        cout << "Decoration     : " << decoration << "\n";
        cout << "Payment Method : " << (paymentMode == "upi" ? "UPI" : (paymentMode == "card" ? "Card" : "Cash")) << "\n";
        if (paymentMode == "upi") cout << "UPI ID         : " << upiID << "\n";
        if (paymentMode == "card") cout << "Card Number    : ************" << cardNumber.substr(12, 4) << "\n";
        cout << "----------------------------------------\n";
        cout << "TOTAL PAID     : Rs" << cost << "\n";
        cout << "----------------------------------------\n";
    }
void adminPanel() {
        string pass;
        cout << "Enter admin password: ";
        cin.ignore();
        getline(cin, pass);
        if (pass != "admin123") {
            cout << "❌ Access denied.\n";
            return;
        }

        int opt;
        do {
            cout << "\n1. Edit Room\n2. Cancel Room\n3. View All\n4. Edit Event\n5. Cancel Event\n6. Exit\nChoice: ";
            cin >> opt;
            cin.ignore();
            int bNo;
            string dob, email;

            if (opt == 1 || opt == 2) {
                cout << "Room Booking No: ";
                cin >> bNo;
                cin.ignore();
                cout << "DOB: "; getline(cin, dob);
                cout << "Email: "; getline(cin, email);
            } else if (opt == 4 || opt == 5) {
                cout << "Event Booking No: ";
                cin >> bNo;
                cin.ignore();
                cout << "DOB: "; getline(cin, dob);
                cout << "Email: "; getline(cin, email);
            }

            switch (opt) {
                case 1: editBooking(bNo, dob, email); break;
                case 2: cancelBooking(bNo, dob, email); break;
                case 3: displayAllBookings(); break;
                case 4: editEvent(bNo, dob, email); break;
                case 5: cancelEvent(bNo, dob, email); break;
                case 6: cout << "Exiting admin...\n"; break;
                default: cout << "Invalid option.\n";
            }
        } while (opt != 6);
    }

    void cancelBooking(int bookingNo, const string& dob, const string& email) {
        auto it = bookings.find(bookingNo);
        if (it != bookings.end()) {
            Booking& b = it->second;
            if (b.dob == dob && b.email == email) {
                if (b.roomType == "standard") standardRooms += b.quantity;
                else if (b.roomType == "deluxe") deluxeRooms += b.quantity;
                else if (b.roomType == "suite") suiteRooms += b.quantity;

                float refund = b.totalCost * 0.5;
                cout << "\n❌ Booking cancelled. Refund: Rs" << refund << "\n";
                bookings.erase(it);
            } else cout << "❌ Invalid DOB or Email.\n";
        } else cout << "❌ Booking not found.\n";
    }

    void editBooking(int bookingNo, const string& dob, const string& email) {
        auto it = bookings.find(bookingNo);
        if (it != bookings.end()) {
            Booking& b = it->second;
            if (b.dob == dob && b.email == email) {
                if (b.roomType == "standard") standardRooms += b.quantity;
                else if (b.roomType == "deluxe") deluxeRooms += b.quantity;
                else if (b.roomType == "suite") suiteRooms += b.quantity;

                string newRoom;
                int newQty, newDays;
                cout << "New room type: ";
                getline(cin, newRoom);
                cout << "New quantity: ";
                cin >> newQty;
                cout << "New days: ";
                cin >> newDays;
                cin.ignore();

                float refund = b.totalCost * 0.25;
                cout << " Partial refund (25%): Rs" << refund << "\n";

                bookings.erase(it);
                bookRoom(b.name, b.email, b.numGuests, newRoom, newQty, b.dob, newDays);
            } else cout << "❌ DOB or Email mismatch.\n";
        } else cout << "❌ Booking not found.\n";
    }
};

// Validation Helpers
bool isValidEmail(const string& email) {
    const regex pattern(R"(^[\w\.-]+@[\w\.-]+\.\w+$)");
    return regex_match(email, pattern);
}
bool isValidDOB(const string& dob) {
    const regex pattern(R"(^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[012])-\d{4}$)");
    return regex_match(dob, pattern);
}

// Main
int main() {
    Resort twilight("Twilight Trails", 80, 50, 40, 5000, 9000, 12000);
    int ch;

    do {
        cout << "\n--- Twilight Trails ---\n";
        cout << "1. Show Info\n2. Book Room\n3. Check-in\n4. Book Event\n5. Admin Panel\n6. Exit\nChoice: ";
        cin >> ch;

        switch (ch) {
            case 1: twilight.displayInfo(); break;
            case 2: {
                string name, email, dob, room;
                int guests, qty, days;
                cin.ignore();

                do {
                    cout << "Enter your Name: "; getline(cin, name);
                    if (name.empty()) cout << "❌ Required!\n";
                } while (name.empty());

                do {
                    cout << "Enter your Email: "; getline(cin, email);
                    if (!isValidEmail(email)) cout << "❌ Invalid email.\n";
                } while (!isValidEmail(email));

                do {
                    cout << "No of Guests: "; cin >> guests;
                    if (guests <= 0) cout << "❌ Must be > 0\n";
                } while (guests <= 0);
                cin.ignore();

                do {
                    cout << "DOB (dd-mm-yyyy): "; getline(cin, dob);
                    if (!isValidDOB(dob)) cout << "❌ Invalid format.\n";
                } while (!isValidDOB(dob));

                do {
                    cout << "Room Type(standard/deluxe/suite): "; getline(cin, room);
                    if (room.empty()) cout << "❌ Required!\n";
                } while (room.empty());

                do {
                    cout << "No of Rooms: "; cin >> qty;
                    if (qty <= 0) cout << "❌ Must be > 0\n";
                } while (qty <= 0);

                do {
                    cout << "No of Days for stay: "; cin >> days;
                    if (days <= 0) cout << "❌ Must be > 0\n";
                } while (days <= 0);

                twilight.bookRoom(name, email, guests, room, qty, dob, days);
                break;
            }

            case 3: {
                int bNo;
                string dob, email;
                cout << "Booking No: "; cin >> bNo; cin.ignore();
                cout << "DOB: "; getline(cin, dob);
                cout << "Email: "; getline(cin, email);
                twilight.checkIn(bNo, dob, email);
                break;
            }

            case 4: twilight.bookEvent(); break;
            case 5: twilight.adminPanel(); break;
            case 6: cout << "\nGoodbye, from Twilight Trails 🌅\n"; break;
            default: cout << "❌ Invalid option.\n";
        }
    } while (ch != 6);

    return 0;
}