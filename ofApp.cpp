#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(0);
	ofEnableDepthTest();

	this->fbo1.allocate(ofGetWidth(), ofGetHeight());
	this->fbo2.allocate(ofGetWidth(), ofGetHeight());
	this->shader.load("shader/shader.vert", "shader/shader.frag");

	this->box2d.init();
	this->box2d.setGravity(0, 50);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->circle_list.size() - 1; i > -1; i--) {

		this->life_list[i] -= 0.75;

		if (this->life_list[i] < 0) {

			this->circle_list.erase(this->circle_list.begin() + i);
			this->radius_list.erase(this->radius_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);
		}
	}

	if (ofGetFrameNum() % 10 == 0) {

		auto radius = ofRandom(10, 30);

		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(0.5, 0.83, 0.1);
		circle->setup(this->box2d.getWorld(), ofRandom(ofGetWidth() * 0.5 - radius, ofGetWidth() * 0.5 + radius), radius, radius);
		circle->setRotation(ofRandom(360));
		this->circle_list.push_back(circle);
		this->radius_list.push_back(radius);
		this->life_list.push_back(255);
	}

	this->box2d.update();


	this->fbo1.begin();
	ofClear(0);

	for (int i = 0; i < this->circle_list.size(); i++) {

		ofDrawCircle(glm::vec2(this->circle_list[i]->getPosition()), this->radius_list[i]);
	}

	this->fbo1.end();

	this->fbo2.begin();
	ofClear(0);

	for (int x = 0; x < ofGetWidth(); x += 120) {

		ofDrawRectangle(x, 0, 60, ofGetHeight());
	}

	this->fbo2.end();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniformTexture("tex1", this->fbo1.getTexture(), 1);
	this->shader.setUniformTexture("tex2", this->fbo2.getTexture(), 2);

	ofDrawRectangle(glm::vec2(0, 0), ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}