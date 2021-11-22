const { app, BrowserWindow, ipcMain } = require('electron');
const net = require('net');
const fs = require('fs');

let win, socket;

const PORT = '1234'; 
const HOST = '127.0.0.1';

function setUpSocket() {
    socket = new net.Socket();
    socket.connect(PORT, HOST, function(event) {
        console.log('Connected to CPP Server ' + HOST + ":" + PORT);
    });

    socket.on('error', function(exp) {
        if (win)
            win.close();
        console.log(exp);
    });

    socket.on('data', function(data) {
        win.webContents.send('results', data.toString());
    });
}

function setUpIPC() {
    ipcMain.on('data', (event, data) => {
        socket.write(data, (event) => console.log(event));
    });
}

function setUp() {   
    setUpSocket();
    setUpIPC();
}

function cleanUp() {
    socket.destroy();

    //remove indexed directory
    fs.writeFile('app-data.json', JSON.stringify({}), error => { if (error) console.log(error); });
}

function createWindow() {
    // Create the browser window.
    win = new BrowserWindow({
        width: 1000,
        height: 600,
        webPreferences: {
            nodeIntegration: true  //to enable window.require in React App
        }
    });

    setUp();

    // and load the index.html of the app.
    win.loadURL('http://localhost:8080');

    win.on('closed', () => cleanUp());
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow);

// Quit when all windows are closed.
app.on('window-all-closed', () => {
    // On macOS it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('activate', () => {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (win === null) {
        createWindow();
    }
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.