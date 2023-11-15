/*
Modify modelURL and serialPort to your settings
*/
const modelURL = 'https://teachablemachine.withgoogle.com/models/QdVJiJwTF/';
const serialPort = 'COM5';

let classifier;
let serial;
let video;
let flippedVideo;
let label;

function preload() {
    classifier = ml5.imageClassifier(modelURL + 'model.json');
    serial = new p5.SerialPort();
}

function setup() {
    serial = new p5.SerialPort();
    serial.open(serialPort);
    createCanvas(320, 260);
    video = createCapture(VIDEO);
    video.size(320, 240);
    video.hide();
    flippedVideo = ml5.flipImage(video);
    classifyVideo();
}

function draw() {
    background(0);
    image(flippedVideo, 0, 0);
    fill(255);
    textSize(16);
    textAlign(CENTER);
    text(label, width / 2, height - 4);
}

function classifyVideo() {
    flippedVideo = ml5.flipImage(video)
    classifier.classify(flippedVideo, gotResult);
    flippedVideo.remove();
}

function gotResult(error, results) {
    if (error) {
        console.error(error);
        return;
    }
    label = String(results[0].label);
  
    switch (label){
      case "abnormal_flood":
        state = 0;
        break;
      
      case "abnormal_open":
        state = 1;
        break;
        
      case "normal":
        state = 2;
        break;
    }
    // console.log(label);
    // serial.write(label);
    console.log(state);
    serial.write(state.toString());
    classifyVideo();
  
}