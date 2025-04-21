# 🌴 Resort Management System – C++ Project

This is a fully functional Resort Management System developed in C++. It supports both **room bookings** and **event management**, along with **payment validation**, **email and DOB checks**, and **admin operations** like editing and cancellations.

## ✨ Features

### ✅ Booking Functionalities:
- **Room booking** with stay duration and cost calculation
- **Event booking** with:
  - Event type selection (Birthday, Anniversary, Other)
  - Optional decoration (adds cost)
  - Email and DOB validation

### 💳 Payment Options:
- Supports **UPI**, **Card**, and **Cash**
- Validates UPI ID and card format
- Simulates QR scanning and receipt generation

### 🔁 Event Editing & Cancellation:
- **Editing** charges 25% of original price and rebooks
- **Cancellation** refunds 50% of the event amount

### 🧾 Other Key Features:
- Unique **booking ID** generation
- Confirmation emails (simulated)
- Admin-protected access to sensitive actions
- Input validation throughout

---

## 🛠️ Build Instructions
Make sure you have `g++` installed.

### Compile via Terminal or use the makefile

### Run the App

Option 1: From Terminal
Copy
./resort

Option 2: From VS Code
Press Ctrl + Shift + D
Choose "Run Resort Management"
Ensure "externalConsole": true is set in launch.json

⚠️ Known Issue
The app may crash if run inside the VS Code terminal/debugger due to input behavior.(espescially in the admin pannel refund section)
✅ It works perfectly as .exe or from system terminal.

📁 Project Structure
📁 ResortManagement/
├── resort_management.cpp     
├── resort.exe                 
├── Makefile                  
├── tasks.json               
├── launch.json               
└── README.md     

* NOTE : thus requires C++ extension along with the compiler like g++ (MinGW)
