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
        startPoint = juce::Point<int>(dragControllers.getX(), dragControllers.getBottom());
        int peakX = dragControllers.pAttack->point.getX();
        int peakY = dragControllers.pAttack->point.getBottom();
        peakPoint = juce::Point<int>(peakX, peakY);
        int susX = dragControllers.pDecay->point.getX();
        int susY = dragControllers.pDecay->point.getY();
        sustainPoint = juce::Point<int>(susX, susY);
        int relX = dragControllers.pRelease->point.getX();
        int relY = dragControllers.pRelease->point.getY();
        releasePoint = juce::Point<int>(relX, relY);
        endPoint = juce::Point<int>(dragControllers.getRight(), dragControllers.getBottom());
      repaint();
    }
    void paint(juce::Graphics &g) override
    {
        g.setColour(juce::Colours::lightblue);
        juce::Path envPath;
        /*
        envPath.startNewSubPath(startPoint.toFloat());
        envPath.lineTo(peakPoint.toFloat());
        envPath.lineTo(sustainPoint.toFloat());
        envPath.lineTo(releasePoint.toFloat());
        envPath.lineTo(endPoint.toFloat());
        envPath.closeSubPath();
        auto stroke = juce::PathStrokeType(3.0f);
        juce::AffineTransform transform;
        g.strokePath(envPath, stroke, transform);
         */
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


