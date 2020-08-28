/*
  ==============================================================================

    ADSR.h
    Created: 27 Aug 2020 4:10:21pm
    Author:  Hayden Setlik

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include "maximilian.h"
#include <stdio.h>
#include "DraggerClass.h"




class ADSR  : public juce::Component
{
public:
    //note: the constructor explicitly initializes the draggers with the correct vectors,
    // but the contents of the vectors are not yet set
    ADSR() : aDragger(aRectV[0], aRectV[1], aRectV[2], aRectV[3]),
             dDragger(dRectV[0], dRectV[1], dRectV[2], dRectV[3]),
             sDragger(aRectV[0], sRectV[1], sRectV[2], sRectV[3]),
             rDragger(aRectV[0], rRectV[1], rRectV[2], rRectV[3])
    {
        addAndMakeVisible(aDragger);
        addAndMakeVisible(dDragger);
        addAndMakeVisible(sDragger);
        addAndMakeVisible(rDragger);
        
        aDragger.setColor(juce::Colours::orange);
        
        dDragger.setColor(juce::Colours::lightblue);
        sDragger.setColor(juce::Colours::green);
        rDragger.setColor(juce::Colours::pink);
        
        setSize(400, 250);
        calculateGlobalPoints();
    }

    ~ADSR() override
    {
    }
    
    void calculateGlobalPoints()
    {
        peakPoint = juce::Point<float>(aTime, getHeight() - (getHeight() * 0.8));
        sustainStartPoint = juce::Point<float>(aTime + dTime, getHeight() - (getHeight() * sLevel));
        releaseStartTime = aTime + dTime + (getWidth() / 5.0);
        sustainEndPoint = juce::Point<float>(releaseStartTime, getHeight() - (getHeight() * sLevel));
        sustainCenterX = (sustainStartPoint.getX() + sustainEndPoint.getY()) / 2.0;
        sustainCenterPoint = juce::Point<float> (sustainCenterX, getHeight() - (getHeight() * sLevel));
        float spaceRemaining = getWidth() - releaseStartTime;
        float endX = (rTime * spaceRemaining) / 100.0;
        endPoint = juce::Point<float> (endX, getHeight());
        //now we actually set values for the rectangle vectors
        
        //setting max X values that add up to 0.85 so that the minimum sustain length is 15% of the window width
        int attackMax = getWidth() * 0.2;
        int decayMax = getWidth() * 0.25;
        int releaseMax = getWidth() * 0.4;
        //multiply those max values by the percentage to the left of its range of the drag point
        int xAttackCurrent = (aTime / 100) * attackMax;
        int xDecayCurrent = attackMax - (dDragger.getXValueFloat() * decayMax);
        int xReleaseEndCurrent = getWidth() - (rDragger.getXValueFloat() * releaseMax);
        //getting the sustain width from the remaing width left by those three
        int sustainWidth = getWidth() - (xDecayCurrent + xReleaseEndCurrent);
        // now to find the y values
        int sustainTopY = getHeight() - (getHeight() * 0.4);
        int peakY = getHeight() - (getHeight() * 0.8);
        //now to actually assign things to the vectors
        aRectV = {0, peakY, attackMax, getHeight() / 12};
        dRectV = {xAttackCurrent, getHeight() - (getHeight() / (int)(1 / sLevel)), decayMax, getHeight() / 12};
        sRectV = {xDecayCurrent, 0, sustainWidth, getHeight()};
        rRectV = {xDecayCurrent + sustainWidth,
            11 * (getHeight() / 12),
            xReleaseEndCurrent - (xDecayCurrent + sustainWidth),
            getHeight() / 12};
        //sending these back to the dragger objects
        aDragger.reInit(aRectV[0], aRectV[1], aRectV[2], aRectV[3]);
        dDragger.reInit(dRectV[0], dRectV[1], dRectV[2], dRectV[3]);
        sDragger.reInit(sRectV[0], sRectV[1], sRectV[2], sRectV[3]);
        rDragger.reInit(rRectV[0], rRectV[1], rRectV[2], rRectV[3]);
    }

    void paint (juce::Graphics& g) override
    {
        //recalculate the global points before doing anything
        calculateGlobalPoints();

    }
    
    void mouseDown(juce::MouseEvent &event)
    {
        repaint();
    }

    void resized() override
    {
        repaint();
    }

private:
    //envelope parameters to be controlled by GUI later
    //remember: these values are between 0 and 100
    double aTime = 25.0;
    double dTime = 35.0;
    double sTime = 100.0;
    double sLevel = 0.4;
    double rTime = 80.0;
    double timeTotal = aTime + dTime + sTime + rTime;
    double scaleFactor = timeTotal / getWidth();
    
    
    //initializing the points so the child components can be initialized explicitly
    juce::Point<float> peakPoint;
    juce::Point<float> sustainStartPoint;
    float releaseStartTime;
    juce::Point<float> sustainEndPoint;
    float sustainCenterX;
    juce::Point<float> sustainCenterPoint;
    juce::Point<float> endPoint;
    
    
    //each of these vectors contains four ints in order: topLeftX, topLeftY, width, height
    std::vector<int> aRectV = {0, 0, 0, 0};
    std::vector<int> dRectV = {0, 0, 0, 0};
    std::vector<int> sRectV = {0, 0, 0, 0};
    std::vector<int> rRectV = {0, 0, 0, 0};
    
    //the dragger objects themselves
    DragRangeRect aDragger;
    DragRangeRect dDragger;
    DragRangeRect sDragger;
    DragRangeRect rDragger;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSR)
};







