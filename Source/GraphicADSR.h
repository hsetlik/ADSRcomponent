//
//  GraphicADSR.h
//  ADSRcomponent - App
//
//  Created by Hayden Setlik on 8/30/20.
//
#pragma once
#include <JuceHeader.h>

#include "DraggersOnly.h"

class GraphicADSR : public juce::Component,
                    public juce::ComponentListener
{
public:
    GraphicADSR()
    {
        addAndMakeVisible(dragControllers);
        dragControllers.pAttack->point.addComponentListener(this);
        dragControllers.pDecay->point.addComponentListener(this);
        dragControllers.pRelease->point.addComponentListener(this);
        
    }
    ~GraphicADSR() {}
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override {calculatePoints();}
    void calculatePoints()
    {
        
    }
    void paint(juce::Graphics &g) override
    {
        
    }
private:
    ADSRenv dragControllers;
    //points for the lines to be drawn to
    juce::Point<int> startPoint;
    juce::Point<int> peakPoint;
    juce::Point<int> sustainPoint;
    juce::Point<int> releasePoint;
    juce::Point<int> endPoint;
    //values to pass to the Maximilian envelope
    float attackMs;
    float decayMs;
    float sustainPercent;
    float releaseMs;
};


