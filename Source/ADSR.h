/*
  ==============================================================================

    ADSR.h
    Created: 27 Aug 2020 4:10:21pm
    Author:  Hayden Setlik

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include <stdio.h>
#include "DraggerClass.h"




class ADSR  : public juce::Component
{
public:
    //note: the constructor explicitly initializes the draggers with the correct vectors,
    // but the contents of the vectors are not yet set
    ADSR() : aDragger(aRectV[0], aRectV[1], aRectV[2], aRectV[3])

    {
        
        
        
        setSize(400, 250);
    }

    ~ADSR() override
    {
    }
    
    void calculateGlobalPoints()
    {
        
    }

    void paint (juce::Graphics& g) override
    {
        //recalculate the global points before doing anything
        
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }
    
    void mouseDown(juce::MouseEvent &event)
    {
        //repaint();
        for(int i = 0; i < 4; ++i)
        {
            printf("%d ", aRectV[i]);
        }
        printf("\n");
    }

    void resized() override
    {
        aDragger.resized();
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
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSR)
};







