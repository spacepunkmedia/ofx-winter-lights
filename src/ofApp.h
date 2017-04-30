#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxSyphon.h"

class Line {
public:
    ofPoint a;
    ofPoint b;
    
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofxKinect kinect;
    
        ofxCvGrayscaleImage grayImage;
        ofxCvContourFinder contourFinder;
    
        vector < ofPoint > drawnPoints;
        vector < Line > lines;
    
        //These are used for setting up an array of mapped values
        //I did it this way so that a calculation doesn't have to be made
        //Everytime I want a mapped value (i can just look it up)
        vector < int > xMap;
        vector < int > yMap;
    
        ofColor lineColor;
        int hue;
    
		
        int threshold;
        int lineWidth;
    
        ofxSyphonServer mainOutputSyphonServer;
        bool	bSmooth;
};
