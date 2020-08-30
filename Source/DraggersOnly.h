//
//  DraggersOnly.h
//  ADSRcomponent - App
//
//  Created by Hayden Setlik on 8/28/20.
//

#pragma once
#include <JuceHeader.h>
#include "SustainDragger.h"

class ADSRenv : public juce::Component
{
private:
    DraggerContainer attackDragger, decayDragger;

public:
    ADSRenv() : attackDragger(10, 10, 75, 12, hor),
                decayDragger(10, 22, 225, 12, hor)
                
    {
        //put the two sliders on and change their color to tell them apart
        addAndMakeVisible(attackDragger);
        addAndMakeVisible(decayDragger);
       
        setBounds(0, 0, 400, 300);
        
        decayDragger.setChildColor(juce::Colours::orange);
        
        //sustainDragger.point.setHeightByInt(20);
        //now to set up the limits for cont2
        decayDragger.addPeer(&attackDragger, left);
       
        
       
    }
    ~ADSRenv()
    {
    }
    void resized() override
    {
        attackDragger.resized();
        decayDragger.resized();
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
        //decayDragger.checkLimitUpdates();
    }

};
