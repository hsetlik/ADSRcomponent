//
//  DraggersOnly.h
//  ADSRcomponent - App
//
//  Created by Hayden Setlik on 8/28/20.
//

#pragma once
#include <JuceHeader.h>
#include "DraggerClass.h"

class DraggerGroup : public juce::Component
{
public:
    DraggerGroup() : Dragger1(NULL, NULL, NULL, NULL),
                     Dragger2(NULL, NULL, NULL, NULL)
    {
        addAndMakeVisible(Dragger1);
        addAndMakeVisible(Dragger2);
        setSize(400, 300);
        
        updateDragger1();
        updateDragger2();
        Dragger2.setNodeColor(juce::Colours::orange);
    }
    void updateDragger1()
    {
        printf("Dragger 1 updating...\n");
        draggerHeight = (float)getHeight() * 0.12f;
        dragger1X = getWidth() / 10;
        dragger1Y = getWidth() / 10;
        dragger1W = getWidth() / 2;
        dragger1H = (int)draggerHeight;
        Dragger1.reInit(dragger1X, dragger1Y, dragger1W, dragger1H);
        
    }
    void updateDragger2()
    {
        printf("Dragger 2 updating...\n");
        dragger2X = (Dragger1.getXSetting() + dragger1X) * -1;
        dragger2Y = Dragger1.getY() + (getHeight() / 6);
        dragger2W = getWidth() / 2;
        dragger2H = (int)draggerHeight;
        Dragger2.reInit(dragger2X, dragger2Y, dragger2W, dragger2H);
    }
    ~DraggerGroup() override
    {
        
    }
    void mouseDown(const juce::MouseEvent &event) override
    {
        mouseIsDown = true;
    }
    void mouseUp(const juce::MouseEvent &event) override
    {
        mouseIsDown = false;
    }
    
    void mouseDrag(const juce::MouseEvent &event) override
    {

    }
    void resized() override
    {
        Dragger1.resized();
    }
private:
    DragPointContainer Dragger1, Dragger2;
    float draggerHeight;
    int dragger1X;
    int dragger1Y;
    int dragger1W;
    int dragger1H;
    
    int dragger2X;
    int dragger2Y;
    int dragger2W;
    int dragger2H;
    
    bool mouseIsDown = false;
};


