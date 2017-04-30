#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    kinect.init(true); //sets the kinect to IR mode
    kinect.open(); //opens the first availble kinect
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    //setting up openCV
    grayImage.allocate(kinect.width, kinect.height);
    
    threshold = 80;
    lineWidth = 1;
    
    hue = 0;
    lineColor.setHsb(hue, 255, 255);
    
    for (int i = 0; i <= kinect.width; i++){
        int addValue = ofGetWidth()/kinect.width;
        xMap.push_back(addValue * i);
    }
    for (int i = 0; i <= kinect.height; i++){
        int addValue = ofGetHeight()/kinect.height;
        yMap.push_back(addValue * i);
    }
    
    ofSetFrameRate(60);
    ofBackground(0, 0, 0);
    
    //Syphon settings
    bSmooth = false;
    ofSetWindowTitle("winter-lights-festival");
    
    mainOutputSyphonServer.setName("Screen Output");

}

//--------------------------------------------------------------
void ofApp::update(){
    
    //ofSetLineWidth(lineWidth);
    
    //ofBackground(100, 100, 100);
    
    //Updating the kinect
    kinect.update();
    
    //Checks tos ee if there is a new frame from the kinect, applies that image to the OpenCV
    if(kinect.isFrameNew()){
        grayImage.setFromPixels(kinect.getPixels());
        grayImage.threshold(threshold);
        
        grayImage.flagImageChanged();
        
        contourFinder.findContours(grayImage, 5, (kinect.width*kinect.height)/2, 5, false);
    }
    
    //Updates the ofPoints class and Line class with data from the IR light for drawing
    if(contourFinder.nBlobs > 0){
        
        //resets the elapsed time everytime a 'blob' is detected//a line is made
        ofResetElapsedTimeCounter();
        
        int x = ofGetWidth()-xMap[contourFinder.blobs[0].centroid.x];
        int y = yMap[contourFinder.blobs[0].centroid.y];
        
        for (auto point : drawnPoints){
            ofPoint mouse;
            mouse.set(x, y);
            float dist = (mouse - point).length();
            if (dist < 30){
                Line lineTemp;
                lineTemp.a = mouse;
                lineTemp.b = point;
                lines.push_back(lineTemp);
            }
        }
        drawnPoints.push_back(ofPoint(x, y));
    }
    
    //Cycles the HUE of the lineColor ofColor
    /*hue += 1;
    if(hue > 255){hue = 0;};
    lineColor.setHsb(hue, 100, 255);
    */
    
    //Checks the ammount of time since a line was added, if its over X seconds
    //then the vectors are cleared and the elapsed time is reset
    if(ofGetElapsedTimef() > 10){
        drawnPoints.clear();
        lines.clear();
        ofResetElapsedTimeCounter();
    }
    
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //clears the background for compositing later if you so choose
    //glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*
    //draw the kinect image
    kinect.draw(0, 0, kinect.width, kinect.height);
    
    //draw the openCV Grayscale Image, and then draw the Countour on top of it
    grayImage.draw(kinect.width, 0, kinect.width, kinect.height);
    contourFinder.draw(kinect.width, 0, kinect.width, kinect.height);
     */
    
    //ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    ofSetColor(lineColor);
    
    //Draw the lines on the screen as stored in the Line class 'lines'
    for (auto line : lines){
        ofDrawLine(line.a, line.b);
    }
    
    mainOutputSyphonServer.publishScreen();
    
    /*
     ofNoFill();
    ofSetColor(0, 0, 0, 30);
    ofDrawRectangle(0, 0, kinect.width, kinect.height);
     */
    
    //Report information about the kinect onto the screen
    stringstream reportBlobPos;
    for (int i = 0; i < contourFinder.nBlobs; i++){

        int blobX = contourFinder.blobs[i].centroid.x;
        int blobY = contourFinder.blobs[i].centroid.y;
        
        //ofSetColor(255, 0, 0);
        //ofFill();
        //ofDrawCircle(blobX, blobY, 5);
    
        reportBlobPos << "x of blob_" << i << " " << blobX << " y of blob_" << i << " " << blobY << endl;
    }
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "threshold " << threshold << " (press: +/-)" << endl << "line width " << lineWidth << " (press: l/k)" << endl
    << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate() << endl << reportBlobPos.str();
    ofDrawBitmapString(reportStr.str(), 20, kinect.height + 50);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case '+':
            threshold ++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold --;
            if (threshold < 0) threshold = 0;
            break;
        case 'r':
            lines.clear();
            drawnPoints.clear();
            break;
        case 'l':
            lineWidth++;
            break;
        case 'k':
            lineWidth--;
            break;
        case 's':
            bSmooth = !bSmooth;
            break;
    }
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
