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
    ADSR()
    {
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
    }

    void paint (juce::Graphics& g) override
    {
        //recalculate the global points before doing anything
        calculateGlobalPoints();
        
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId)); // clear the background

        g.setColour (juce::Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
        
        

        g.setColour (juce::Colours::white);
         //creating the path that will draw the envelope line
        
        juce::Path envPath;
        auto const stroke = juce::PathStrokeType(3.0);
        auto* pStroke = &stroke;
        juce::AffineTransform transform;
        
        

        juce::Point<float> startPoint = juce::Point<float> (0, getHeight());
        
        //start the envelope line in the bottom left corner
        envPath.startNewSubPath(startPoint);
        //draw a line from the start point to the top of the attack
        envPath.lineTo(peakPoint);
        //draws the decay line
        envPath.lineTo(sustainStartPoint);
        //draws the sustain portion in a different color
        envPath.lineTo(sustainEndPoint);
        //changing the color back and drawing the release
        g.setColour (juce::Colours::white);
        envPath.lineTo(endPoint);
        //close the subPath
        envPath.closeSubPath();
        g.strokePath(envPath, *pStroke, transform);
        g.setColour (juce::Colours::lightblue);
        juce::Path sustainPath;
        sustainPath.startNewSubPath(sustainStartPoint);
        sustainPath.lineTo(sustainEndPoint);
        sustainPath.closeSubPath();
        g.strokePath(sustainPath, *pStroke, transform);
        
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
    double rTime = 100.0;
    double timeTotal = aTime + dTime + sTime + rTime;
    double scaleFactor = timeTotal / getWidth();
    double pATime = aTime * scaleFactor;
    double pDtime = dTime * scaleFactor;
    double pSTime = sTime * scaleFactor;
    double pRTime = rTime * scaleFactor;
    
    
    //initializing the points so the child components can be initialized explicitly
    juce::Point<float> peakPoint;
    juce::Point<float> sustainStartPoint;
    float releaseStartTime;
    juce::Point<float> sustainEndPoint;
    float sustainCenterX;
    juce::Point<float> sustainCenterPoint;
    juce::Point<float> endPoint;
    
    DragRangeRect aDragger;
    DragRangeRect dDragger;
    DragRangeRect sDragger;
    DragRangeRect rDragger;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSR)
};







