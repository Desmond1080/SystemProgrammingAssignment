# SystemProgrammingAssignment

 # EventManageTest

A simple C++ project for event management, using [vcpkg](https://github.com/microsoft/vcpkg) for dependency management.  
Currently depends on `nlohmann-json`.

---

## 📦 Requirements
- Visual Studio 2019 (16.10+) or Visual Studio 2022  
- [vcpkg](https://github.com/microsoft/vcpkg)  

---

## ⚡ Setup

1. Clone and bootstrap vcpkg (if not already installed):

   ```sh
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat

2. Enable vcpkg integration in Visual Studio (one-time setup):
   
   ```sh
   .\vcpkg integrate install

3. Clone this repository:

   ```sh
   git clone https://github.com/YOUR-USERNAME/EventManageTest.git
   cd EventManageTest

4. Open the solution (EventManageTest.sln) in Visual Studio.

    Dependencies will be restored automatically using vcpkg.json.


