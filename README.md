# Document-Search
Winning solution for Adobe Bootcamp Hackathon 2019

Document Search Engine created as part of Adobe Bootcamp Hackathon (Winning solution). 
Backend has been developed using C++ employing Ternary-Search tree & KMP algorithm with Query Optimization and Support for regex/wildcard search.

General Instructions to Run the Project

Please follow the following steps to run the project –
Installations
1.	Open CPP_Miniproject.sln in Visual Studio IDE.
2.	Install Boost Package
a.	In Solution Explorer of VS, Right click on CPP_Miniproject -> Manage Nuget Packages
b.	Install boost-vc141
3.	Install nodejs from https://nodejs.org/dist/v10.16.3/node-v10.16.3-x64.msi
4.	Go to client directory from project root.
5.	Run command npm i

For running
1.	Build the Solution in VS (Ctrl+Shift+B)
2.	Run the Solution (Ctrl + F5)
a.	Wait till the output window displays ~ Waiting for Connections
3.	Client Side – 
a.	Open cmd prompt at client directory
b.	Execute npm start (A Chrome window should open at localhost:8080)
c.	Open another cmd prompt at client directory
d.	Execute npm run electron
4.	The UI should pop-up now.
5.	Enter Directory (Window pop-up will appear). This is the directory to keep test-files.
a.	Choose test directory under project root folder.
6.	Search bar should appear, and you are good to go. 
