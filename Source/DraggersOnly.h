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
    DraggerContainer attackDragger, decayDragger, releaseDragger;
   

public:
    ADSRenv() : attackDragger(10, 10, 75, 12, hor),
                decayDragger(10, 22, 225, 12, hor),
                releaseDragger(210, 22, 90, 12, hor)
    {
        
        addAndMakeVisible(attackDragger);
        addAndMakeVisible(decayDragger);
        addAndMakeVisible(releaseDragger);
       
       
        setBounds(0, 0, 400, 300);
        
        decayDragger.setChildColor(juce::Colours::orange);
        releaseDragger.setChildColor(juce::Colours::violet);
        
        //sustainDragger.point.setHeightByInt(20);
        //now to set up the limits for cont2
        decayDragger.addPeer(&attackDragger, left);
        decayDragger.addPeer(&releaseDragger, right);
       
        
       
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
