const fs = window.require('fs');

export const readData = () => {
    return JSON.parse(fs.readFileSync('app-data.json'));
}

export const writeData = (data) => {
    fs.writeFile('app-data.json', JSON.stringify(data), error => { if (error) console.log(error); });
}