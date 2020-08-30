//
//  GraphicADSR.h
//  ADSRcomponent - App
//
//  Created by Hayden Setlik on 8/30/20.
//
#pragma once
#include <JuceHeader.h>

#include "DraggersOnly.h"

class GraphicADSR : public juce::Component
{
public:
    GraphicADSR()
    {
        addAndMakeVisible(dragControllers);
        
    }
    ~GraphicADSR() {}
private:
    ADSRenv dragControllers;
};


