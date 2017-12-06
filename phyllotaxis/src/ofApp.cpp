#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    gui.setup();
    gui.setPosition(5, 40);
    gui.add(degree.setup("degree", 137.5, 130.00, 140.00));
    gui.add(spread.setup("spread", 11, 2, 40));
    gui.add(extrude.setup("extrude", 0.3, 0.01, 0.90));
    light.setup();
    light.setPosition(-100, 200,0);
//    light.setAmbientColor(ofColor(ofGetMouseX()/5,ofGetMouseY()/2.8,0));
    light.setDiffuseColor(ofColor(ofGetMouseX()/5,ofGetMouseY()/2.8,0));
    
    masterColor = ofFloatColor::black;
    secondColor = ofFloatColor::white;
    ofEnableDepthTest();
    for(int i = 0;  i < nCubes;i++){
        children.push_back(ofBoxPrimitive(1,5,1));
        }
    
    mySound.load("spirit passage.mp3");
    nBandsToGet = 6; // i do not care about all the frequencies
    
    fftSmoothed = new float[8192];
    for (int i = 0; i < 8192; i++){
        fftSmoothed[i] = 0;
    }
    nBandsToGet = 64;
    mySound.setLoop(true);
    mySound.setVolume(0.2);
    

    sphere.setPosition(0, 0, 0);

   

}

//--------------------------------------------------------------
void ofApp::update(){
    float rad = ofDegToRad(degree);
    for (int i = 0;  i < nCubes;i++) {
        ofVec3f pos;
        if (selectedType == "simple") {
            pos = ofxPhyllotaxis::simple(i, rad, spread);
        }

        if (selectedType == "conical") {
            pos = ofxPhyllotaxis::conical(i, rad, spread, extrude);
        }

        if (selectedType == "apple") {
            pos = ofxPhyllotaxis::apple(i, rad, spread, nCubes);
        }
        children[i].setPosition(pos);
    }
    ofSoundUpdate();
    float * val = ofSoundGetSpectrum(nBandsToGet);
    for (int i = 0;i < nBandsToGet; i++){
        fftSmoothed[i] *= 0.96f;
        if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
    }
    cout << ofToString(fftSmoothed[0]*2) << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    maybeDrawGui();
    camera.begin();

    secondMaterial.setEmissiveColor(masterColor);
    for (int i = 0;  i < nCubes;i++) {
        float lerp = ofMap(i, 0, nCubes, 0.0, ofGetMouseX()/30);
        auto interpolatedColor = masterColor.getLerped(secondColor, lerp);
        secondMaterial.setEmissiveColor(interpolatedColor);
        secondMaterial.begin();
        children[i].draw();
        secondMaterial.end();
        
//        sphere.set(fftSmoothed[i]*150, 300);
//        sphere.setPosition(ofGetWidth()*.2, ofGetHeight()*.75, 0);
        sphere.setRadius(fftSmoothed[i]*40);
        sphere.draw();
        sphere.enableColors();
    }
    camera.end();
    
    
}

void ofApp::maybeDrawGui(){
    if(!hideGui){
        ofDisableDepthTest();
        gui.draw();
        ofPushStyle();
        string displayGui = "press 'g' to toggle the gui, up and down to change presets";
        ofDrawBitmapString(displayGui, 5, 10);
        string types = "press 1, 2, 3 to try different implementation";
        ofDrawBitmapString(types, 5, 20);
        string currentType = "current: "+ selectedType;
        ofDrawBitmapString(currentType, 5, 30);
        ofPopStyle();
        ofEnableDepthTest();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch(key){
        case 'g':
            hideGui = !hideGui;
            break;
        case 49:
            selectedType = "conical";
            break;
        case 50:
            selectedType = "apple";
            break;
        case 51:
            selectedType = "simple";
            break;
        default:
            break;
        case 52:
            mySound.play();
            break;
        case 53:
            mySound.stop();
            break;
    }
}

