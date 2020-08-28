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
    DraggerGroup() : Dragger1(NULL, NULL, NULL, NULL)
    {
        addAndMakeVisible(Dragger1);
        setSize(400, 300);
        
        int dragger1X = 15;
        int dragger1Y = 15;
        int dragger1W = getWidth() / 2;
        int dragger1H = 25;
        Dragger1.reInit(dragger1X, dragger1Y, dragger1W, dragger1H);
        
    }
    ~DraggerGroup() override
    {
        
    }
    
    void resized() override
    {
        Dragger1.resized();
    }
private:
    DragRangeRect Dragger1;
};


